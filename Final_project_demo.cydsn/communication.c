/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "communication.h"

//MACROS and global variables 
#define ARRAYSIZE 256
#define BUFFSIZE 4096

/*******************************************************************************
* Function Name: 
* void DMA_init(int32_t buffer_1[], int32_t buffer_2[],
              int32_t buffer_3[], int32_t buffer_4[])
* This function takes in the ping pong global buffers from main as arguments
* It runs the DMA initialization sequence and loads the buffer and returns 
* the values to main.
*******************************************************************************/

void DMA_init(int32_t buffer_1[], int32_t buffer_2[],
              int32_t buffer_3[], int32_t buffer_4[]){ //main buffer
    
    /*Start DMA initialization sequence and trasnfer*/
    cy_stc_dma_channel_config_t channelConfig =
    {
        .descriptor = &DMA_1_Descriptor_1,
        .preemptable = DMA_1_PREEMPTABLE,
        .priority = DMA_1_PRIORITY,
        .enable = false,
        .bufferable = DMA_1_BUFFERABLE
    };
    /*descriptor for carrying sine wave data*/
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_1, &DMA_1_Descriptor_1_config);
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_2, &DMA_1_Descriptor_2_config);
    
    /*descriptor for carrying square wave data*/
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_3, &DMA_1_Descriptor_3_config);
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_4, &DMA_1_Descriptor_4_config);
    
    Cy_DMA_Channel_Init(DMA_1_HW, DMA_1_DW_CHANNEL, &channelConfig);
    
    /*Transfer ADC channel data using 4 separate DMA descriptors*/
    
    /*ping pong buffer 1 and buffer 2 reading data from channel 0 of ADC (Sine wave)*/
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_1, (uint32_t*)&(SAR->CHAN_RESULT[0]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_1, buffer_1);
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_2,(uint32_t*)&(SAR->CHAN_RESULT[0]) );
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_2, buffer_2);
    
    /*ping pong buffer 3 and buffer 4 reading data from channel 1 of ADC (Square wave)*/
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_3,(uint32_t*)&(SAR->CHAN_RESULT[1]) );
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_3, buffer_3);
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_4,(uint32_t*)&(SAR->CHAN_RESULT[1]) );
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_4, buffer_4);
    
    /*Enable DMA channel*/
    Cy_DMA_Channel_Enable(DMA_1_HW, DMA_1_DW_CHANNEL);
    
    /*Enable DMA */
    
    //Cy_DMA_Enable(DMA_1_HW);
    /*Trigger DMA*/
    //Cy_TrigMux_SwTrigger(TRIG0_OUT_CPUSS_DW0_TR_IN0, CY_TRIGGER_TWO_CYCLES);
    
    
    
}

