///--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "logic_layer.h"
#include "global.h"
#include "bitmap_defines.h"

uint8_t letter[5][5] = {
    {0,1,1,1,0},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1}
};

int main(void)
{
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_CYAN);

  clearscreen_struct clearScreenStruct;
  clearScreenStruct.color = VGA_COL_BLACK;

  clearScreenToVGA(clearScreenStruct);

  letterToVGA(5,   1, VGA_COL_WHITE, lc_a, 5);
  letterToVGA(10,  1, VGA_COL_WHITE, lc_b, 5);
  letterToVGA(15,  1, VGA_COL_WHITE, lc_c, 5);
  letterToVGA(20,  1, VGA_COL_WHITE, lc_d, 5);
  letterToVGA(25,  1, VGA_COL_WHITE, lc_e, 5);
  letterToVGA(30,  1, VGA_COL_WHITE, lc_f, 5);
  letterToVGA(35,  1, VGA_COL_WHITE, lc_g, 5);
  letterToVGA(40,  1, VGA_COL_WHITE, lc_h, 5);
  letterToVGA(45,  1, VGA_COL_WHITE, lc_i, 5);
  letterToVGA(50,  1, VGA_COL_WHITE, lc_j, 5);
  letterToVGA(55,  1, VGA_COL_WHITE, lc_k, 5);
  letterToVGA(60,  1, VGA_COL_WHITE, lc_l, 5);
  letterToVGA(65,  1, VGA_COL_WHITE, lc_m, 5);
  letterToVGA(70,  1, VGA_COL_WHITE, lc_n, 5);
  letterToVGA(75,  1, VGA_COL_WHITE, lc_o, 5);
  letterToVGA(80,  1, VGA_COL_WHITE, lc_p, 5);
  letterToVGA(85,  1, VGA_COL_WHITE, lc_q, 5);
  letterToVGA(90,  1, VGA_COL_WHITE, lc_r, 5);
  letterToVGA(95,  1, VGA_COL_WHITE, lc_s, 5);
  letterToVGA(100, 1, VGA_COL_WHITE, lc_t, 5);
  letterToVGA(105, 1, VGA_COL_WHITE, lc_u, 5);
  letterToVGA(110, 1, VGA_COL_WHITE, lc_v, 5);
  letterToVGA(115, 1, VGA_COL_WHITE, lc_w, 5);
  letterToVGA(120, 1, VGA_COL_WHITE, lc_x, 5);
  letterToVGA(125, 1, VGA_COL_WHITE, lc_y, 5);
  letterToVGA(130, 1, VGA_COL_WHITE, lc_z, 5);

  while(1)
  {

  }
}
