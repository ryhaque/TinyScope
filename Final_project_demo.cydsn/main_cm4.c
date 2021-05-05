/******************************************************************************
* File Name: main_cm4.c

* Description: Building a TinyScope oscilloscope using the EmWin Graphics library 
  and the Newhaven Display 240x320 TFT Display. This file is the main and it makes
  calls to the other files to run the code

 *******************************************************************************/
#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "graphicaldisplay.h"
#include "communication.h"
#include "commandargparser.h"

/*MACROS AND GLOBAL VARIABLES*/

#define XSIZE 320 // Width of LCD screen in pixels
#define YSIZE 240 // Height of LCF screen in pixels
#define XDIVISIONS 10 // Number of x-axis divisions for grid
#define YDIVISIONS 8 // Number of y-axis divisions for grid
#define XMARGIN 5 // Margin around screen on x-axis
#define YMARGIN 4 // Margin around screen on y-axis
#define MAXPOINTS XSIZE-2*XMARGIN // Maximum number of points in wave for plotting

/*xscale value*/
int Ps = 1; //pixels/uS - 0.128(default value) increment 1 (Ps<1)

#define PI 3.14159265
#define ARRAYSIZE 256
//#define BUFFSIZE 4096

/*MACROS for CAP*/
int32_t data;
char str[100];
char mode[10];
char trigger_slope[10] = "positive\0";
char startNstop[10];
int trigger_level = 1000; //default value: 1V
int trigger_channel = 1; //default value
int xscale = 1000; //default value (in uS/div)
int yscale = 1000; //default value (in mV/div)

/*Macros for buffers*/
int32_t buffer_1[ARRAYSIZE], buffer_2[ARRAYSIZE]; //sine ping pong buffers
int32_t buffer_3[ARRAYSIZE], buffer_4[ARRAYSIZE]; //sawtooth ping pong buffers
int32_t sine_buff[MAXPOINTS];
int32_t st_buff[MAXPOINTS];
int flag_buff=0; //to print sine wave
int stflag = 0;  //to print saw tooth
int bindex1 = 0; //for ping pong buffer 1 and 2
int bindex2 = 0; //for ping pong buffer 3 and 4
int buffcount = 0; //for sinewave
int stcount = 0; //for sawtooth wave

/*MACROS for potentiometer values*/
uint8_t pot1_val;
uint8_t pot2_val;



/*******************************************************************************
* Function Name: void DMA_ISR() - data loaded into large buff from ping pong buff
*******************************************************************************/

void DMA_ISR(){
    /* DMA ISR in charge of putting data from DMA ping pong buffers
       into larger  buffers
    */
    if(flag_buff!=1){ //for sine wave
        if(bindex1 == 0){
            for(int i=0;i<ARRAYSIZE;i+=(int)Ps){ //256
                sine_buff[buffcount] = buffer_1[i];
                buffcount++;
            }
            bindex1 = 1; //Set flag to other buffer to fill since buffer 1 is full
        }else{
            for(int i=0;i<MAXPOINTS-ARRAYSIZE;i+=(int)Ps){ 
                sine_buff[buffcount] = buffer_2[i]; 
                buffcount++;
            }
            if(buffcount>=MAXPOINTS){
                buffcount = 0;
                //sine buffer full so set a flag to print values
                flag_buff = 1;    
            }
            bindex1 = 0;   //Set flag to buffer 1 then wait
        }    
    }
    if(stflag!=1){ //for saw tooth wave
        if(bindex2 == 0){
            for(int i=0;i<ARRAYSIZE;i+=(int)Ps){ //256
                st_buff[stcount] = buffer_3[i]; //y values
                stcount++;
            }
            bindex2 = 1; //Set flag to other buffer to fill since buffer 3 is full
        }else{
            for(int i=0;i<MAXPOINTS-ARRAYSIZE;i+=(int)Ps){ 
                st_buff[stcount] = buffer_4[i]; //y value
                stcount++;
            }
            if(stcount>=MAXPOINTS){
                stcount = 0;
                //saw tooth buffer full so set a flag to print values
                stflag = 1;    
            }
            bindex2 = 0;   //Set flag to buffer 3 then wait
        }    
    }
    /*Clear the DMA inturrupt*/
    Cy_DMA_Channel_ClearInterrupt(DW0, 0);
    //NVIC_ClearPendingIRQ(DMA_ISR_cfg.intrSrc);
}

/*******************************************************************************
* Function Name: void KeyBoardISR(void) - forms str from chars passed to puTTY
* and sends it off to Command arg parser
*******************************************************************************/
void KeyBoardISR(){
    //i=0;
    /* check Rx fifo status and read byte if non empty */
    uint32_t rxStatus = Cy_SCB_UART_GetRxFifoStatus(UART_1_HW);
    
    if(rxStatus & CY_SCB_UART_RX_NOT_EMPTY){
        data = Cy_SCB_UART_Get(UART_1_HW);
        char c = (char)data;
        //printf("\ndata=%c\r\n",c);
        strncat(str,&c,sizeof(c));
        
        if(c =='\n' || c=='\r'){
            //printf("\n\nnew line character\r\n");
            //printf("\nstr=%s\r\n",str);
            command_parser(str, mode, &trigger_level, trigger_slope, 
                &trigger_channel, &xscale, &yscale,startNstop);
            memset(str,'\0',sizeof(str));
        }
        //Clear RX status
        Cy_SCB_UART_ClearRxFifoStatus(UART_1_HW,CY_SCB_UART_RX_NOT_EMPTY); 
    }
    
    NVIC_ClearPendingIRQ(UART_1_INT_cfg.intrSrc);
}
/*******************************************************************************
* Function Name: void printDMA(void) - debugging purposes to print ping pong buffs
*******************************************************************************/
void printDMA(){
    
    for(int i=0;i<ARRAYSIZE;i++){
        
        printf("buffer_1[%d]: 0X%X\r\n",i,buffer_1[i]);
        printf("buffer_2[%d]: 0X%X\r\n",i,buffer_2[i]);
        printf("buffer_3[%d]: 0X%X\r\n",i,buffer_3[i]);
        printf("buffer_4[%d]: 0X%X\r\n",i,buffer_4[i]);
        printf("\r\n");
    }
}
/*******************************************************************************
* Function Name: void Get_Pot_Value(int ch) - takes in SAR ADC channel as argument
* and updates potentiometer offset value
*******************************************************************************/

void Get_Pot_Value(int ch){
    //Get potentiometer values from respective channel
    uint32_t ADC_val = ADC_1_GetResult32(ch); //---- 00000000 0000 (use only 8 MSB)
    //printf("ADC val %u\r\n", ADC_val);
    
    if(ch == 2){ //pot1 value (8 MSB)
        pot1_val = ((1 << 8) - 1) & (ADC_val >> (5-1));
        //printf("pot1 value %u\r\n", pot1_val);
    }
    if(ch == 3){ //pot2 value (8 MSB)
        pot2_val = ((1 << 8) - 1) & (ADC_val >> (5-1));
        //printf("pot2 value %u\r\n", pot2_val);
    }     
}
/*******************************************************************************
* Function Name: 
* void draw_waves(int32_t WaveX[], int32_t WaveY[], uint32_t color, int offset)
*******************************************************************************/
void draw_waves(int32_t WaveX[], int32_t WaveY[], uint32_t color, int offset){
    
    GUI_SetPenSize(2); //set line thickness
    GUI_SetColor(color); //set color
    GUI_SetLineStyle(GUI_LS_SOLID); //solid or dashed line
    
    for(int i=1;i<MAXPOINTS;i++){
        GUI_DrawLine(WaveX[i-1], WaveY[i-1]+offset,WaveX[i], WaveY[i]+offset); 
    }
    
}

/*******************************************************************************
* Function Name: 
* void calc_PS()
*******************************************************************************/
void calc_PS(){
    //Ps = 4*(XSIZE/(XDIVISIONS*xscale)); 
    Ps = ((4*32)/(xscale/*/1000*/)); //pix/uS
    if(Ps<1 || Ps==1){
        Ps = 1;
    }
}



/*******************************************************************************
* Function Name: int main(void)
*******************************************************************************/
int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    //UART for printing and debugging purpose
    UART_1_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    
    /*Initialize and enable interrupt*/
    //i=0;
    Cy_SysInt_Init(&UART_1_INT_cfg, KeyBoardISR);
    NVIC_EnableIRQ(UART_1_INT_cfg.intrSrc);
    
    //Initialize ADC
    ADC_1_Start();
    ADC_1_StartConvert();
    
    //Initialize DMA and enable DMA inturrupts
    DMA_init(buffer_1,buffer_2,buffer_3,buffer_4);
    //DMA_init();
    
    /*DMA ISR*/
    Cy_SysInt_Init(&DMA_ISR_cfg, DMA_ISR);
    NVIC_EnableIRQ(DMA_ISR_cfg.intrSrc);
    Cy_DMA_Channel_SetInterruptMask (DW0, 0, CY_DMA_INTR_MASK); // Unmask DMA Channel 0 interrupt
    
    //enable dma
    Cy_DMA_Enable(DMA_1_HW);
    
    //print the DMA in the communications functions
    printDMA();
    
    /* Initialize EmWin Graphics */
    GUI_Init();
   
    /* Display background */
    drawBackground(XSIZE, YSIZE, XMARGIN, YMARGIN);
    
    /* Display scale settings */
    //printScaleSettings(1000, 500, 200, 15);
    
    /* Create a sine wave arrays*/
    int32_t sineWaveX[MAXPOINTS] = {0};
    int32_t sineWaveY[MAXPOINTS] = {0};
    /*Create saw tooth wave arrays*/
    int32_t sawToothWaveX[MAXPOINTS] = {0};
    int32_t sawToothWaveY[MAXPOINTS] = {0};
      
    
    for(;;)
    {   
     
        if(strstr(startNstop,"start")!=NULL){
            //printf("I am in start\r\n");
            
            if(flag_buff>0 /*&& trigger_channel ==1*/){
                Get_Pot_Value(2);
                /* Display frequency */
                printFrequency(pot1_val, pot2_val, 10, 15);
                calc_PS();
                //load sine values
                for(int i=0; i<MAXPOINTS; i++){
                    sineWaveX[i] = XMARGIN+i;
                    sineWaveY[i] = (((YSIZE/YDIVISIONS)*3300)*sine_buff[i])/(yscale*4096); //ADC->mv    
                } 
                //draw sine wave
                draw_waves(sineWaveX, sineWaveY, GUI_LIGHTGREEN, pot1_val);
                //clear
                draw_waves(sineWaveX, sineWaveY, GUI_DARKCYAN, pot1_val);
                /* Display grid*/
                drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
                flag_buff = 0;
            }
            if(stflag>0 /*&& trigger_channel == 2*/){
                Get_Pot_Value(3);
                /* Display frequency */
                printFrequency(pot1_val, pot2_val, 10, 15);
            
                //load saw tooth values
                for(int i=0; i<MAXPOINTS; i++){
                    sawToothWaveX[i] = XMARGIN+i;
                    sawToothWaveY[i] = (((YSIZE/YDIVISIONS)*3300)*st_buff[i])/(yscale*4096); //ADC->mv    
                } 
                //draw sawtooth wave
                draw_waves(sawToothWaveX, sawToothWaveY, GUI_WHITE,pot2_val);
                //clear
                draw_waves(sawToothWaveX, sawToothWaveY, GUI_DARKCYAN,pot2_val); //erase
                /* Display grid*/
                drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
                stflag = 0;
            }
            
        }
        
        
                
    }
    
}

/* [] END OF FILE */
