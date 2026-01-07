/** \file
 * io_layer.h
 * @brief Dit bestand bevat de declaraties voor de I/O laag.
*/

#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "stm32_ub_vga_screen.h"

void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color);

#endif // IO_LAYER_H
