#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void lineToVGA(line_struct lineStruct) {
  int x_1 = lineStruct.x_1; int x_2 = lineStruct.x_2; 
  int y_1 = lineStruct.y_1; int y_2 = lineStruct.y_2;
  
  int dx = x_2 - x_1;
  int dy = y_2 - y_1;

  if (dx > dy) { //dx is bigger
    UB_VGA_FillScreen(VGA_COL_YELLOW);
  }
  else if (dx < dy) { //dy is bigger
    UB_VGA_FillScreen(VGA_COL_RED);
  }
  else { //difference the same
    for (int lineRange = x_1; lineRange < x_2; lineRange++)
     UB_VGA_SetPixel(lineRange, y_1++, lineStruct.color);
  }
}
