///--------------------------------------------------------------
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

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "logic_layer.h"
#include "global.h"

int main(void)
{
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_RED);

  rectangle_struct rectangleStruct = {
    .x = 200,
    .y = 201,
    .width = 10,
    .height = 20,
    .color = VGA_COL_GREEN,
    .filled = 1,
  };

  rectangleToVGA(rectangleStruct);

  rectangleStruct.x = 10,
  rectangleStruct.y = 1,
  rectangleStruct.width = 150,
  rectangleStruct.height = 101,
  rectangleStruct.color = VGA_COL_LIGHT_GREEN,
  rectangleStruct.filled = 0,

  rectangleToVGA(rectangleStruct);

  line_struct lineStruct = {
    .x_1 = 115,
    .y_1 = 10,
    .x_2 = 140,
    .y_2 = 220,
    .color = VGA_COL_LIGHT_MAGENTA,
    .weight = 10,
  };

  lineToVGA(lineStruct);

  lineStruct.x_1 = 1,
  lineStruct.y_1 = 1,
  lineStruct.x_2 = 230,
  lineStruct.y_2 = 231,
  lineStruct.color = VGA_COL_BLUE,
  lineStruct.weight = 2,

  lineToVGA(lineStruct);

  while(1)
  {

  }
}
