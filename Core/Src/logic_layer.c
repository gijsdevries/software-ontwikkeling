#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void lineToVGA(line_struct lineStruct) {
  int x1 = lineStruct.x_1; int x2 = lineStruct.x_2; 
  int y1 = lineStruct.y_1; int y2 = lineStruct.y_2;

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    UB_VGA_SetPixel(x1, y1, lineStruct.color);

    if (x1 == x2 && y1 == y2)
      break;

    int e2 = err * 2;

    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }

}
