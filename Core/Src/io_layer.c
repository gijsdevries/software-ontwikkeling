/**
 * @file io_layer.c
 * @brief I/O-laag voor VGA-schermbewerkingen.
 * @details Dit bestand bevat functies voor het direct manipuleren van pixels op het VGA-scherm,
 *          zoals het instellen van een individuele pixelkleur.
 */

#include "io_layer.h"

/**
 * @brief Stelt één pixel in op het scherm met een specifieke kleur.
 * @param xp De x-coördinaat van de pixel.
 * @param yp De y-coördinaat van de pixel.
 * @param color De kleur van de pixel (8-bit).
 * @retval None
 */

void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp>=VGA_DISPLAY_X) xp=0;
  if(yp>=VGA_DISPLAY_Y) yp=0;

  // Write pixel to ram
  VGA_RAM1[(yp*(VGA_DISPLAY_X+1))+xp]=color;
}

