//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

//#include "main.h"
//#include "stm32_ub_vga_screen.h"
//#include <math.h>
//#include "front_layer.h"
//#include "io_layer.h"
#include "logic_layer.h"

//TEMPORARY
#include "global.h"
#include "stdio.h"
#include <stdlib.h>

int main(void)
{
  //SystemInit(); // System speed to 168MHz
  //UB_VGA_Screen_Init(); // Init VGA-Screen
  //UB_VGA_FillScreen(VGA_COL_BLUE);

  //allocate bitmap
  unsigned char (*bitmap)[240][3] = malloc(sizeof(unsigned char[320][240][3]));
  if (bitmap == NULL) {
	  return -1;
  }
  clearscreen_struct clearScreenStruct;
  clearScreenStruct.color = WHITE;

  clearScreenToBitmap(clearScreenStruct, bitmap);

  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      unsigned char r = bitmap[x][y][0];
      unsigned char g = bitmap[x][y][1];
      unsigned char b = bitmap[x][y][2];

      printf("(%3d,%3d,%3d) ", r, g, b);
    }
   printf("\n");
  }

  //free memory
  free(bitmap);

/*  UB_VGA_SetPixel(160, 120, VGA_COL_GREEN);

  while(1)
  {
   
  }*/
}
