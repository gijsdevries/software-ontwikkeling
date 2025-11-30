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

        Buffer_to_struct(3);

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct(int cmd_val)
{
	uint8_t take_index = 0;

	line_struct lijn;
	rectangle_struct rechthoek;
	text_struct text;
	bitmap_struct bitmap;
	clearscreen_struct clearscherm;

	char* argument;

	argument = take_word(&take_index);
	free(argument);

    switch (cmd_val)
    {
		case LIJN:
		{
			argument = take_word(&take_index);
			lijn.x_1 = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			lijn.y_1 = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			lijn.x_2 = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			lijn.y_2 = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			lijn.color = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			lijn.weight = atoi(argument);
			free(argument);

			//FUNCTIE LOGIC LAYER
		}
		break;

		case RECHTHOEK:
		{
			argument = take_word(&take_index);
			rechthoek.x = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			rechthoek.y = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			rechthoek.width = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			rechthoek.height = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			rechthoek.color = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			rechthoek.filled = atoi(argument);
			free(argument);

			//FUNCTIE LOGIC LAYER
		}
		break;

		case TEXT:
		{
			argument = take_word(&take_index);
			text.x_lup = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			text.y_lup = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			text.color = atoi(argument);
			free(argument);

			text.text = take_word(&take_index);

			text.fontname = take_word(&take_index);

			argument = take_word(&take_index);
			text.fontsize = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			text.fontstyle = atoi(argument);
			free(argument);

			//FUCNTIE LOGIC LAYER

            free(text.text);
            free(text.fontname);
		}

		case BITMAP:
		{
			argument = take_word(&take_index);
			bitmap.x_lup = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			bitmap.y_lup = atoi(argument);
			free(argument);

			argument = take_word(&take_index);
			bitmap.bm_nr = atoi(argument);
			free(argument);

            char buffer[200];
            snprintf(buffer, sizeof(buffer),
                     "x_lup=%d, y_lup=%d, bm_nr=%d",
                     bitmap.x_lup, bitmap.y_lup, bitmap.bm_nr);

            USART2_SendString(buffer);
            USART2_SendString("\r\n");
		}
		break;


    }
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
