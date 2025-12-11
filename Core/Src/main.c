#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "global.h"
#include "stdio.h"
#include "logic_layer.h"
#include "global.h"
#include "string.h"
#include "bitmap_defines.h"



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
    .color = VGA_COL_CYAN,
    .text = "!@#$%^&*() 123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .fontname = "arial",
    .fontsize = 2,
    .fontstyle = "normaal"
  };

  textToVGA(textStruct);

  text_struct textStruct2 = {
    .x_lup = 1, 
    .y_lup = 100,
    .color = VGA_COL_BLACK,
    .text = "!@#$%^&*() 123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .fontname = "arial",
    .fontsize = 1, 
    .fontstyle = "normaal"
  };

  textToVGA(textStruct2);

  while(1)
  {
    USART2_BUFFER();
  }
}
