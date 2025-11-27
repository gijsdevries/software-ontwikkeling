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

  while(1)
  {
    USART2_BUFFER();
  }
}
