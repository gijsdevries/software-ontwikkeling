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

        Buffer_to_struct();

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct()
{
    char *new_tempBuffer = realloc(tempBuffer, idx + 1);
    tempBuffer = new_tempBuffer;

    uint8_t j = 0; // Teller voor de tempBuffer
    uint8_t word_started = 0; // Om te weten of we binnen een woord zitten

    // Loop door de buffer heen
    for(uint8_t i = 0; i < idx; i++)
    {
    	// Als er een komma gevonden is of het einde van de string is bereikt
        if(buffer[i] == ',' || i == idx - 1)
        {
            // Voeg laatste karakter toe als buffer eindigt zonder komma
            if (i == idx - 1 && buffer[i] != ',')
            {
                tempBuffer[j++] = buffer[i];
            }

            // Skip spaties
            while(j > 0 && tempBuffer[j - 1] == ' ')
                j--;

            tempBuffer[j] = '\0';
            USART2_SendString(tempBuffer);
            USART2_SendString("\r\n");

            j = 0;
            word_started = 0;
            continue;
        }

        // Skip spaties
        if(!word_started && buffer[i] == ' ')
            continue;

        // We zijn nu in een woord, ook spaties binnen woorden meenemen
        word_started = 1;
        tempBuffer[j++] = buffer[i];
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
