#include "stm32f4xx.h"
#include "front_layer.h"
#include <string.h>

char *buffer = NULL;   // Dynamische buffer
uint8_t idx = 0;

void USART2_BUFFER()
{
    char UART_karakter = USART2_ReceiveChar(); // Ontvang karakter vanaf USART2

    char *new_buffer = realloc(buffer, idx + 2); // Pas buffer dynamisch aan
    // Als er geen karakter is ingevuld doe niks
    if (new_buffer == NULL) {
        return;
    }
    // Pas buffer aan
    buffer = new_buffer;
    buffer[idx++] = UART_karakter;
    // Als enter is ingevuld sluit string af en start parsing
    if (UART_karakter == '\n') {
        buffer[idx] = '\0';

        //USART2_SendString(buffer);
        Buffer_Check();

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}


void Buffer_Check()
{
	char cmd_var;
    for (int i = 0; i < NUM_COMMANDS; i++) //Controleert hoeveel commando's erin de define staan
    {
    	//Is er een match in het eerste woord van de commando en de define code
        if (strncmp(buffer, commands[i].name, strlen(commands[i].name)) == 0)
        {
        	cmd_var = commands[i].code;
            USART2_SendChar(cmd_var);
            USART2_SendChar('\n');
            return;
        }
    }

    USART2_SendString("ERROR: Onbekend commando\n");
    USART2_SendString("Herzie het het woord voor de eerste komma\n");
}


void USART2_Init(void) {
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 = TX, PA3 = RX
    GPIOA->MODER |= (2 << (2*2)) | (2 << (3*2));
    GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));

    // Baudrate 115200
    uint32_t pclk1 = SystemCoreClock / 4; // APB1
    USART2->BRR = pclk1 / 115200;

    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void USART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void USART2_SendString(char *str) {
    while (*str) USART2_SendChar(*str++);
}

char USART2_ReceiveChar(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}
