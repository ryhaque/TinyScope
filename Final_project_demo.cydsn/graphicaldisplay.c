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

#include "graphicaldisplay.h"
#define PI 3.14159265

/* Arrays to hold channel waveform data for drawing */
GUI_HMEM ch1_spline;
GUI_HMEM ch2_spline;

/*******************************************************************************
Dislay startup screen
*******************************************************************************/
void ShowStartupScreen(void)
{
    /* Set font size, foreground and background colors */
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetBkColor(GUI_BLUE);
    GUI_SetColor(GUI_LIGHTMAGENTA);
    GUI_Clear();
  
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_DispStringAt("TinyScope", 160, 120);
    
    GUI_SetFont(GUI_FONT_24B_1);
    GUI_SetColor(GUI_LIGHTGREEN);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("CSE 121/L Final Project", 160, 200);
}

/*******************************************************************************
Generate a sine wave for demo
*******************************************************************************/
  void sineWave(int period, // period in us
	            int sampleInterval, // Samping interval in us
	            int samples, // total samples
                int amplitude, // Amplitude of wave
	            int *data){ // Wave data
  double rad, f;
  for (int i=0; i<samples; i++){
    rad = (double) (i * sampleInterval) * ((double) (2.0*PI/period));
    f = sin(rad);
    data[i] = (int) (f*128.0 + 128.0);
    if (data[i] > 255) data[i] = 255;
    if (data[i] < 0) data[i] = 0;
    data[i] = data[i] * amplitude/256;
  }
}
/*******************************************************************************
Generate a square wave for demo
*******************************************************************************/

 void squareWave(int period, // period in us
	            int sampleInterval, // Samping interval in us
	            int samples, // total samples
                int amplitude, // Amplitude of wave
	            int *data){ // Wave data

  for (int i=0; i<samples; i++){
    if ((i *sampleInterval) % period < period/2){
     data[i] = 0;
    }
    else {
      data[i] = amplitude;  
    }
 }    
}
                
/*******************************************************************************
// Draw the background for the oscilloscope screen
*******************************************************************************/

void drawBackground(int w, int h, // width and height of screen
		    int xmargin, int ymargin){ // Margin around the image

   GUI_SetBkColor(GUI_BLACK);
   GUI_SetColor(GUI_DARKCYAN);
   GUI_FillRect(0, 0, w, h);
   GUI_SetPenSize(1);
   GUI_SetColor(GUI_LIGHTGRAY);
   GUI_DrawRect(xmargin, ymargin, w-xmargin, h-ymargin);
   GUI_SetLineStyle(GUI_LS_DOT);
}

/*******************************************************************************
// Draw the X/Y grid
*******************************************************************************/
void drawGrid(int w, int h, // width and height of screen
		    int xdiv, int ydiv,// Number of x and y divisions
		    int xmargin, int ymargin){ // Margin around the image

    int xstep = (w-xmargin*2)/xdiv;
    int ystep = (h-ymargin*2)/ydiv;
 
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetLineStyle(GUI_LS_DOT);
    for (int i=1; i<ydiv; i++){
        GUI_DrawLine(xmargin, ymargin+i*ystep, w-xmargin, ymargin+i*ystep);
    }
    for (int i=1; i<xdiv; i++){
      GUI_DrawLine(xmargin+i*xstep, ymargin, xmargin+i*xstep, h-ymargin);
    }
}

/*******************************************************************************
// Display x and y scale settings
*******************************************************************************/

void printScaleSettings(int xscale, int yscale, int xposition, int yposition) { 
 char str[20];
 
 GUI_SetBkColor(GUI_DARKCYAN); // Set background color
 GUI_SetFont(GUI_FONT_16B_1); // Set font
 GUI_SetColor(GUI_LIGHTGRAY); // Set foreground color
 if (xscale >= 1000)
    sprintf(str, "Xscale: %0d ms/div", xscale/1000);
 else
    sprintf(str, "Xscale: %0d us/div", xscale);
 GUI_DispStringAt(str, xposition, yposition);
 int yscaleVolts = yscale/1000;
 int yscalemVolts = yscale % 1000;
 if (yscale >= 1000)
   sprintf(str, "Yscale: %0d V/div", yscaleVolts);
 else
   sprintf(str, "Yscale: %0d.%0d V/div", yscaleVolts, yscalemVolts/100);
 GUI_DispStringAt(str, xposition, yposition + 20);
  
}

/*******************************************************************************
// Display frequency
*******************************************************************************/

void printFrequency(int xfreq, int yfreq, int xposition, int yposition) { 
 char str[20];
 
 GUI_SetBkColor(GUI_DARKCYAN); // Set background color
 GUI_SetFont(GUI_FONT_16B_1); // Set font
 GUI_SetColor(GUI_LIGHTGRAY); // Set foreground color
 
 sprintf(str, "Ch 1 Freq: %0d kHz", xfreq);
 GUI_DispStringAt(str, xposition, yposition);
 sprintf(str, "Ch 2 Freq: %0d kHz", yfreq);
 GUI_DispStringAt(str, xposition, yposition + 20);
}

/*******************************************************************************
// Plot waveform
*******************************************************************************/

void plotWave(int channel, // Channel to be plotted: 0 or 1
              int xpos, int ypos, // X and Y coordinates
              uint32_t color){ // Color of wave
                
    GUI_SetPenSize(4);
    GUI_SetColor(color);
    if (channel == 0)
     GUI_SPLINE_Draw(ch1_spline, xpos, ypos);
    else
     GUI_SPLINE_Draw(ch2_spline, xpos, ypos);
}

