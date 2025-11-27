#include "stm32f4xx.h"
#include "front_layer.h"
#include <string.h>

char *buffer = NULL;   // Dynamische buffer
char *tempBuffer = NULL;
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

        Buffer_to_struct(buffer, idx);

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct(char *buffer, uint8_t idx)
{
    char *new_tempBuffer = realloc(tempBuffer, idx + 1); // Maak buffer dynamic
    tempBuffer = new_tempBuffer;

    uint8_t j = 0; // Tel-variabele voor tempBuffer

    // Loop daar de voledige buffer
    for(uint8_t i = 0; i < idx; i++)
    {
    	if(buffer[i] == ' ') // Sla spaties over
    		continue;

        if(buffer[i] == ',') // Wanneer een komma gevonden is
        {
            tempBuffer[j] = '\0'; // Sluit de opgeslagen string af
            USART2_SendString(tempBuffer);
            USART2_SendString("\r\n");
            j = -1; // Start op -1 zodat de gevonden komma overgeslagen wordt
        }

        tempBuffer[j++] = buffer[i]; // Schrijf karakter uit buffer naar tempBuffer
    }

    // Check voor het laatste woord in de string, deze heeft namelijk geen komma
    if(j > 0)
    {
        tempBuffer[j] = '\0';
        USART2_SendString(tempBuffer);
        USART2_SendString("\r\n");
    }
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
