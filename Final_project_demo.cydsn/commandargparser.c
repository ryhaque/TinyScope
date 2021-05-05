/* ========================================
 ** Command argument parser source file
 * ========================================
*/

#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "commandargparser.h"

/*This function is in charge of taking the string inputted by puttY
  and parsing that string till we get the correct value
  it takes as arguments global variables passed in from main*/

void command_parser(char* str,char mode[],int* trigger_level, char trigger_slope[],
    int* trigger_channel,int* xscale, int* yscale, char startNstop[]){
    //printf("\ncommand argument string = %s\r\n",str);
    
    /*check if start and stop are active*/
    if(strstr(str,"start") != NULL){
        sscanf(str,"%s", startNstop);
        //respond
        printf("\nProgram %s\r\n",startNstop);
    }
    else if(strstr(str,"stop") != NULL){
        sscanf(str,"%s", startNstop);
        //respond
        printf("\nProgram %s\r\n",startNstop);
    }
    /*check if set mode is free or trigger
      in free run mode a free running signal is displayed
      in trigger mode sampled signal is aligned so waveform is more stable
    */
    else if(strstr(str,"set mode") != NULL){
        sscanf(str,"set mode %s", mode);
        //respond
        printf("\nMode set to %s running\r\n",mode);
    }
    /*trigger level set 0-3v in steps of 100mV, Default=1V*/
    else if(strstr(str,"set trigger_level") != NULL){
        sscanf(str,"set trigger_level %d", trigger_level);
        
        int nutrigger_level = (int)trigger_level;
        sscanf(str,"set trigger_level %d", &nutrigger_level);
        //response
        printf("\nTrigger level set to %d mV\r\n",nutrigger_level);
    }
    /*trigger slope positive - rising edge when signal crosses trigger level (default)
      trigger slope negative - falling edge when signal crosses trigger level
    */
    else if(strstr(str,"set trigger_slope") != NULL){
        sscanf(str,"set trigger_slope %s", trigger_slope);
        //response
        printf("\nTrigger slope set to %s\r\n",trigger_slope);
    }
    /*trigger_channel - Display waveform ch 1(default) or ch 2
      works only in stopped state*/
    else if(strstr(str,"set trigger_channel") != NULL){
        sscanf(str,"set trigger_channel %d", trigger_channel);
        
        int nutrigger_channel = (int)trigger_channel;
        sscanf(str,"set trigger_channel %d", &nutrigger_channel);
        //response
        printf("\nTrigger channel set to %d\r\n",nutrigger_channel);
    }
    /*xscale: horizontal scale of waveform - uS/div -1000 default value*/
     else if(strstr(str,"set xscale") != NULL){
        sscanf(str,"set xscale %d", xscale);
        
        int nuxscale = (int)xscale;
        sscanf(str,"set xscale %d", &nuxscale);
        if(nuxscale==100||nuxscale==200||nuxscale==500||
            nuxscale==1000||nuxscale==2000||nuxscale==5000||nuxscale==10000)
        {
            //response to puTTy
            printf("\nSuccessful! Xscale set to %d uS/div\r\n",nuxscale);
        }else{
            printf("invalid xscale value\r\n");
        }    
    }
    /*yscale: vertical scale of waveform - mV/div -1000 default value*/
    else if(strstr(str,"set yscale") != NULL){
        sscanf(str,"set yscale %d", yscale);
        
        int nuyscale = (int)yscale;
        sscanf(str,"set yscale %d", &nuyscale);
        if(nuyscale == 500|| nuyscale==1000|| nuyscale==1500|| nuyscale==2000){
            //response 
            printf("\nSuccessful! Yscale set to %d mV/div\r\n",nuyscale);
        }else{
            printf("invalid yscale value\r\n");
        }    
    }
}