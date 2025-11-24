#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
      UB_VGA_SetPixel(x1, y1, VGA_COL_RED);

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

void lineToVGA(line_struct lineStruct) {
  int x_1 = lineStruct.x_1; int x_2 = lineStruct.x_2; 
  int y_1 = lineStruct.y_1; int y_2 = lineStruct.y_2;
  
  drawLine(x_1, y_1, x_2, y_2);
/*
  int dx = x_2 - x_1;
  int dy = y_2 - y_1;

  if (dx > dy) { //dx is bigger
    int division = dx / dy;
    if (division < 2)
      division = 2;

    for (int lineRange = x_1; lineRange < x_2; lineRange++) {
      if (lineRange % division == 0) {
        y_1++;
      }
      UB_VGA_SetPixel(lineRange, y_1, lineStruct.color);
    }
  }

  else if (dx < dy) { //dy is bigger
    UB_VGA_FillScreen(VGA_COL_RED);
  }

  else { //difference the same
    for (int lineRange = x_1; lineRange < x_2; lineRange++)
      UB_VGA_SetPixel(lineRange, y_1++, lineStruct.color);
  }*/
}
