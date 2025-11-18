#include "io_layer.h"
#include "stm32_ub_vga_screen.h"

unsigned char bitline[320];

void fill_line(void) {
    for (int x = 0; x < 320; x++) {
        bitline[x] = VGA_COL_RED;
    }
}

int plot_VGA(void)
{
	fill_line();
	for(int y = 0; y < 240; y++)
	    for (int x = 0; x < 320; x++) {
	    	uint8_t color = bitline[x];
	        UB_VGA_SetPixel(x, y, color);
	    }
}
