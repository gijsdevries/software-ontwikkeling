#include "stm32f4xx.h"
#include "front_layer.h"
#include "logic_layer.h"
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

        Buffer_to_struct(CLEARSCHERM);

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct(int cmd_val)
{
    char* argument = NULL;
    uint8_t take_index = 0;

    take_int(&take_index);

    switch (cmd_val)
    {
        case LIJN:
        {
            line_struct lijn;

            lijn.x_1 = take_int(&take_index);
            lijn.y_1 = take_int(&take_index);
            lijn.x_2 = take_int(&take_index);
            lijn.y_2 = take_int(&take_index);
            lijn.color = take_int(&take_index);
            lijn.weight = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;

        case RECHTHOEK:
        {
            rectangle_struct rechthoek;

            rechthoek.x = take_int(&take_index);
            rechthoek.y = take_int(&take_index);
            rechthoek.width = take_int(&take_index);
            rechthoek.height = take_int(&take_index);
            rechthoek.color = take_int(&take_index);
            rechthoek.filled = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;

        case TEXT:
        {
            text_struct text;

            text.x_lup = take_int(&take_index);
            text.y_lup = take_int(&take_index);
            text.color = take_int(&take_index);
            text.text = take_word(&take_index);
            text.fontname = take_word(&take_index);
            text.fontsize = take_int(&take_index);
            text.fontstyle = take_int(&take_index);

            //LOGIC LAYER FUNCTIE

            free(text.text);
            free(text.fontname);
        }
        break;

        case BITMAP:
        {
            bitmap_struct bitmap;

            bitmap.x_lup = take_int(&take_index);
            bitmap.y_lup = take_int(&take_index);
            bitmap.bm_nr = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;

        case CLEARSCHERM:
        {
            clearscreen_struct clearscherm;

            clearscherm.color = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;
    }
}

int take_int(uint8_t *take_index)
{
	char* argument = take_word(take_index);
	int int_argument = atoi(argument);
	free(argument);
	return int_argument;
}

char* take_word(uint8_t *take_index)
{
    if (*take_index >= idx) return NULL; // Ga terug als de volledige zin al is uitgelezen

    while (*take_index < idx && buffer[*take_index] == ' ') // Skip de spaties vóór het woord
        (*take_index)++;

    uint8_t start = *take_index;
    uint8_t len = 0;

    while (*take_index < idx && buffer[*take_index] != ',') // Onthoud startlocatie woord en zoek eindlocatie
    {
        (*take_index)++;
        len++;
    }

    while (len > 0 && buffer[start + len - 1] == ' ') // Kort eindlocatie in om eindspaties te negeren
        len--;

    char* word = malloc(len + 1); // Maak word even groot als nodig (dynamic)
    if (!word) return NULL; // Ga terug als word geen lengte heeft

    for (uint8_t j = 0; j < len; j++) // Vul word nu met het gevonden woord op basis van de eerder gevonden lengte
        word[j] = buffer[start + j];
    word[len] = '\0'; // Sluit het woord af

    if (*take_index < idx && buffer[*take_index] == ',') (*take_index)++; // Skip de volgende komma zodat die niet in het volgende woord wordt meegenomen

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
