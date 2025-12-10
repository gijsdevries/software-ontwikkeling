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

int main(void)
{
    SystemCoreClockUpdate();
    SystemInit();
    UB_VGA_Screen_Init();
    UB_VGA_FillScreen(VGA_COL_RED);
    USART2_Init();

    while(1)
    {
        // verwerk alle ontvangen UART-karakters
        while (uart_tail != uart_head)
        {
            char c = uart_buf[uart_tail];
            uart_tail = (uart_tail + 1) % UART_BUF_SIZE;
            USART2_ProcessChar(c);
        }

        // hier kun je andere taken doen, VGA loopt soepel
    }
}
