#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void lineToVGA(line_struct lineStruct) {
  uint16_t xp,yp;

  for(yp=0;yp<VGA_DISPLAY_Y;yp++) {
    for(xp=0;xp<VGA_DISPLAY_X;xp++) {
      if (xp == lineStruct.x_1 && yp == lineStruct.y_1) 
        UB_VGA_SetPixel(xp, yp, lineStruct.color);
      if (xp == lineStruct.x_2 && yp == lineStruct.y_2) {
        UB_VGA_SetPixel(xp, yp, lineStruct.color);
        for (uint8_t i = 0; i < 50; i++) {
          UB_VGA_SetPixel(xp - i, yp, lineStruct.color);
          UB_VGA_SetPixel(xp, yp - i, lineStruct.color);
        }
      }
    }
  }
}
