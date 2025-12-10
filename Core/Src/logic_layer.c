#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>

/*
void textToVGA(text_struct textstruct)
{
  printf("hello world");
}
*/

void clearScreenToVGA(clearscreen_struct CS_struct)
{
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

const char *char_to_bin(unsigned char c)
{
    static char bin[8];
    for (int i = 0; i < 7; i++) {
        bin[6 - i] = (c & (1 << i)) ? '1' : '0';
    }
    bin[8] = '\0';
    return bin;
}

/*
int letterToVGANEW(int x_lup, int y_lup, int color, uint8_t *letter, int font_size)
{
    for (int y = 0; y < 14; y++) // Loop door alle y coördinaten heen
    {
      //get binary
      //char *bin_value = char_to_bin(letter[y]);
      char *bin_value = "11110000"; 
        for (int x = 0; x < 8; x++) // Loop door alle x coördinaten heen
        {
            if (bin_value[x]) // Als een pixel hoog is
            {
            	UB_VGA_SetPixel(x_lup + x, y_lup + y, color); // Pas de pixel aan naar de gegeven kleur
            }
        }
    }
}
*/

int letterToVGA(int x_lup, int y_lup, int color, uint8_t bitmap[5][5], int font_size)
{
    for (int y = 0; y < font_size; y++) // Loop door alle y coördinaten heen
    {
        for (int x = 0; x < font_size; x++) // Loop door alle x coördinaten heen
        {
            if (bitmap[y][x]) // Als een pixel hoog is
            {
            	UB_VGA_SetPixel(x_lup + x, y_lup + y, color); // Pas de pixel aan naar de gegeven kleur
            }
        }
    }
}
