/**
 * @file main.c
 * @date 30.03.2016
 * @version 1.0
 * @author UB
 * @mod_by J.F. van der Bent
 * @brief VGA_core DMA LIB 320x240, 8bit kleur
 * @details Dit bestand bevat de hoofdprogrammalogica voor het initialiseren van het STM32F4-systeem,
 *          het VGA-scherm en de USART2-communicatie.
 */

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "logic_layer.h"
#include "global.h"


int main(void)
{
  SystemCoreClockUpdate();
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_RED);
  USART2_Init();

  USART2_SendString("\r\nWELKOM\r\n");

  while (1)
  {
    USART2_BUFFER();
  }
}
