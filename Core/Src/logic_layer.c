#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <stdio.h>


void clearScreenToVGA(clearscreen_struct CS_struct)
{
  UB_VGA_FillScreen(CS_struct.color);
}

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
