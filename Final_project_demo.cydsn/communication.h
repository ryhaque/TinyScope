/* ========================================
 * Communication header file for DMA init()
 * ========================================
*/

#include "project.h"
#include "stdlib.h"
#include "stdio.h"

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#define ARRAYSIZE 256

/*Function prototypes*/
    
void DMA_init(int32_t buffer_1[], int32_t buffer_2[],
              int32_t buffer_3[], int32_t buffer_4[]);

//void printDMA();

#endif  // Avoid multiple inclusion

/*************************** End of file ****************************/
