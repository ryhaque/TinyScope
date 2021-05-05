/* ========================================
 * Command argument parser header file
 * ========================================
*/

#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#ifndef _COMMAND_ARG_PARSER_H
#define _COMMAND_ARG_PARSER_H

/*Function prototypes*/
    
void command_parser(char* str,char mode[],int* trigger_level, char trigger_slope[],
    int* trigger_channel,int* xscale, int* yscale, char startNstop[]);
    
#endif  // Avoid multiple inclusion


