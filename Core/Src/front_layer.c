#include "stm32f4xx.h"
#include "front_layer.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
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

        Buffer_to_struct(RECHTHOEK);

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
}

void Buffer_to_struct(char cmd_val)
{
    uint8_t take_index = 0; // Take_index zodat de plek in de buffer makkelijk gereset kan worden

    take_int(&take_index); // Skip het eerste woord

    switch (cmd_val) // Vul juiste struct en start juiste functie op basis van de in X functie (Piotr) gevonden commando
    {
        case LIJN: // Vul lijn struct en roep lijn functie aan
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

        case RECHTHOEK: // Vul rechthoek struct en roep rechthoek functie aan
        {
            rectangle_struct rechthoek;

            rechthoek.x = take_int(&take_index);
            rechthoek.y = take_int(&take_index);
            rechthoek.width = take_int(&take_index);
            rechthoek.height = take_int(&take_index);
            rechthoek.color = take_color(&take_index);
            rechthoek.filled = take_int(&take_index);

            //LOGIC LAYER FUNCTIE

            char buffer[200];
			snprintf(buffer, sizeof(buffer),
					 "RECHTHOEK -> x=%d, y=%d, width=%d, height=%d, color=%d, filled=%d",
					 rechthoek.x, rechthoek.y, rechthoek.width,
					 rechthoek.height, rechthoek.color, rechthoek.filled);
			USART2_SendString(buffer);
			USART2_SendString("\r\n");
        }
        break;

        case TEXT: // Vul text struct en roep text functie aan
        {
            text_struct text;

            text.x_lup = take_int(&take_index);
            text.y_lup = take_int(&take_index);
            text.color = take_int(&take_index);
            text.text = take_word(&take_index); // Bij het pakken van een string gebruik primaire commando, deze moet na alle logica weer vrij gegeven worden
            text.fontname = take_word(&take_index); // Zelfde hier
            text.fontsize = take_int(&take_index);
            text.fontstyle = take_int(&take_index);

            //LOGIC LAYER FUNCTIE

            // Geef geheugen vrij
            free(text.text);
            free(text.fontname);
        }
        break;

        case BITMAP: // Vul bitmap struct en roep bitmap functie aan
        {
            bitmap_struct bitmap;

            bitmap.x_lup = take_int(&take_index);
            bitmap.y_lup = take_int(&take_index);
            bitmap.bm_nr = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;

        case CLEARSCHERM: // Vul clearscherm struct en roep clearscherm functie aan
        {
            clearscreen_struct clearscherm;

            clearscherm.color = take_int(&take_index);

            //LOGIC LAYER FUNCTIE
        }
        break;
    }
}

int take_color(uint8_t *take_index)
{
	char* color_arg = take_word(take_index);
	//If-tree voor het bepalen van kleurwaardes
	if (strcmp(color_arg, "zwart") == 0)
		return VGA_COL_BLACK;
	if (strcmp(color_arg, "blauw") == 0)
		return VGA_COL_BLUE;
	if (strcmp(color_arg, "groen") == 0)
		return VGA_COL_GREEN;
	if (strcmp(color_arg, "rood") == 0)
		return VGA_COL_RED;
	if (strcmp(color_arg, "wit") == 0)
		return VGA_COL_WHITE;

	if (strcmp(color_arg, "cyaan") == 0)
		return VGA_COL_CYAN;
	if (strcmp(color_arg, "magenta") == 0)
		return VGA_COL_MAGENTA;
	if (strcmp(color_arg, "geel") == 0)
		return VGA_COL_YELLOW;

	if (strcmp(color_arg, "lichtblauw") == 0)
		return VGA_COL_LIGHT_BLUE;
	if (strcmp(color_arg, "lichtgroen") == 0)
		return VGA_COL_LIGHT_GREEN;
	if (strcmp(color_arg, "lichtcyaan") == 0)
		return VGA_COL_LIGHT_CYAN;
	if (strcmp(color_arg, "lichtrood") == 0)
		return VGA_COL_LIGHT_RED;
	if (strcmp(color_arg, "lichtmagenta") == 0)
		return VGA_COL_LIGHT_MAGENTA;
	if (strcmp(color_arg, "bruin") == 0)
		return VGA_COL_BROWN;
	if (strcmp(color_arg, "grijs") == 0)
		return VGA_COL_GREY;
}


int take_int(uint8_t *take_index)
{
	char* argument = take_word(take_index); // Pak het woord uit de buffer
	int int_argument = atoi(argument); // Converteer woord naar int
	free(argument); // Geef geheugen vrij

	return int_argument; // Geef int terug
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
