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

        Buffer_to_struct(0);

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct(void)
{
    char* w;

    while ((w = take_word()) != NULL)
    {
        USART2_SendString(w);
        USART2_SendString("\r\n");
    }
}


char* take_word(void)
{
    static uint8_t i = 0;
    if (i >= idx) return NULL; // Ga terug als de volledige zin al is uitgelezen

    while (i < idx && buffer[i] == ' ') // Skip de spaties vóór het woord
        i++;

    uint8_t start = i;
    uint8_t len = 0;

    while (i < idx && buffer[i] != ',') // Onthoud startlocatie woord en zoek eindlocatie
    {
        i++;
        len++;
    }

    while (len > 0 && buffer[start + len - 1] == ' ') // Kort eindlocatie in om eindspaties te negeren
        len--;

    char* word = malloc(len + 1); // Maak word even groot als nodig (dynamic)
    if (!word) return NULL; // Ga terug als word geen lengte heeft

    for (uint8_t j = 0; j < len; j++) // Vul word nu met het gevonden woord op basis van de eerder gevonden lengte
        word[j] = buffer[start + j];
    word[len] = '\0'; // Sluit het woord af

    if (i < idx && buffer[i] == ',') i++; // Skip de volgende komma zodat die niet in het volgende woord wordt meegenomen

    return word;
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
