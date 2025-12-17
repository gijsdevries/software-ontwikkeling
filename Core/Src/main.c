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
  SystemCoreClockUpdate();
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_RED);
  USART2_Init();
  
  text_struct textStruct = {
    .x_lup = 1,
    .y_lup = 1,
    .color = VGA_COL_BLACK,
    .text = "!@#$%^&*() 123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .fontname = "arial",
    .fontsize = 1,
    .fontstyle = "cursief"
  };

  textToVGA(textStruct);

  text_struct textStruct2 = {
    .x_lup = 1, 
    .y_lup = 100,
    .color = VGA_COL_BLACK,
    .text = "!@#$%^&*() 123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .fontname = "arial",
    .fontsize = 1, 
    .fontstyle = "vet"
  };

  textToVGA(textStruct2);

  while (1)
  {
    USART2_BUFFER();
  }
}
