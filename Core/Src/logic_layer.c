#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void lineToVGA(line_struct lineStruct) {
  uint16_t xp,yp;

  for (uint16_t lineRangeX = lineStruct.x_1; lineRangeX < lineStruct.x_2; lineRangeX++) {
    for (uint16_t lineRangeY = lineStruct.y_1; lineRangeY < lineStruct.y_2; lineRangeY++) { 
      UB_VGA_SetPixel(lineRangeX, lineRangeY, lineStruct.color);
  }}
}
