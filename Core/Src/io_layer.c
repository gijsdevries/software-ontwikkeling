/** \file
 * io_layer
*/

#include "io_layer.h"

//--------------------------------------------------------------
// put one Pixel on the screen with one color
// Important : the last Pixel+1 from every line must be black
//--------------------------------------------------------------

void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp>=VGA_DISPLAY_X) xp=0;
  if(yp>=VGA_DISPLAY_Y) yp=0;

  // Write pixel to ram
  VGA_RAM1[(yp*(VGA_DISPLAY_X+1))+xp]=color;
}

