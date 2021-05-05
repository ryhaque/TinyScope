/* ========================================
 * graphical display header file
 * ========================================
*/

#include "project.h"
#include "GUI.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

#ifndef _GRAPHICAL_DISPLAY_H
#define _GRAPHICAL_DISPLAY_H
    
/*Function prototypes*/

/*******************************************************************************
Dislay startup screen
*******************************************************************************/
void ShowStartupScreen(void);

/*******************************************************************************
Generate a sine wave for demo
*******************************************************************************/
void sineWave(int period, // period in us
	            int sampleInterval, // Samping interval in us
	            int samples, // total samples
                int amplitude, // Amplitude of wave
	            int *data); // Wave data

/*******************************************************************************
Generate a square wave for demo
*******************************************************************************/

 void squareWave(int period, // period in us
	            int sampleInterval, // Samping interval in us
	            int samples, // total samples
                int amplitude, // Amplitude of wave
	            int *data); // Wave data

/*******************************************************************************
// Draw the background for the oscilloscope screen
*******************************************************************************/

void drawBackground(int w, int h, // width and height of screen
		    int xmargin, int ymargin); // Margin around the image


/*******************************************************************************
// Draw the X/Y grid
*******************************************************************************/
void drawGrid(int w, int h, // width and height of screen
		    int xdiv, int ydiv,// Number of x and y divisions
		    int xmargin, int ymargin); // Margin around the image


/*******************************************************************************
// Display x and y scale settings
*******************************************************************************/

void printScaleSettings(int xscale, int yscale, int xposition, int yposition);

/*******************************************************************************
// Display frequency
*******************************************************************************/

void printFrequency(int xfreq, int yfreq, int xposition, int yposition);

/*******************************************************************************
// Plot waveform
*******************************************************************************/

void plotWave(int channel, // Channel to be plotted: 0 or 1
              int xpos, int ypos, // X and Y coordinates
              uint32_t color); // Color of wave


#endif  // Avoid multiple inclusion

/*************************** End of file ****************************/



