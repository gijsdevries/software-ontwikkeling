#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

void rectangleToVGA(rectangle_struct rectangleStruct) {
  int x = rectangleStruct.x; int w = rectangleStruct.width; 
  int y = rectangleStruct.y; int h = rectangleStruct.height;

  if (rectangleStruct.filled) {
    for (int yy = y; yy < y + h; yy++) {
      for (int xx = x; xx < x + w; xx++) {
        UB_VGA_SetPixel(xx, yy, rectangleStruct.color);
      }
    }
  } else {
    // Top edge
    for (int xx = x; xx < x + w; xx++)
      UB_VGA_SetPixel(xx, y, rectangleStruct.color);

    // Bottom edge
    for (int xx = x; xx < x + w; xx++)
      UB_VGA_SetPixel(xx, y + h - 1, rectangleStruct.color);

    // Left edge
    for (int yy = y; yy < y + h; yy++)
      UB_VGA_SetPixel(x, yy, rectangleStruct.color);

    // Right edge
    for (int yy = y; yy < y + h; yy++)
      UB_VGA_SetPixel(x + w - 1, yy, rectangleStruct.color);
  }
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
    for (int i = 0; i < lineStruct.weight; i++)
      if (dx > dy)
        UB_VGA_SetPixel(x1, y1 + i, lineStruct.color);
      else
        UB_VGA_SetPixel(x1 + i, y1, lineStruct.color);

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
