#include "stm32f4xx.h"
#include "front_layer.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include "bitmap_defines.h"
#include <string.h>

#include <stdlib.h>

volatile char uart_buf[UART_BUF_SIZE];
volatile uint16_t uart_head = 0;
volatile uint16_t uart_tail = 0;
volatile uint8_t command_ready = 0;

char *buffer = NULL;    // dynamische buffer
uint16_t idx = 0;

// Parsing en checking functies
void Buffer_Check()
{
    char cmd_var;

    for (int i = 0; i < NUM_COMMANDS; i++) //Controleert hoeveel commando's erin de define staan
    {
        //Is er een match in het eerste woord van de commando en de define code
        if (strncmp(buffer, commands[i].name, strlen(commands[i].name)) == 0)
        {
            cmd_var = commands[i].code;
            Buffer_to_struct(cmd_var);
            return;
        }
    }

    USART2_SendString("ERROR: Onbekend commando\n");
    USART2_SendString("Herzie het woord voor de eerste komma\n");
}

void Buffer_to_struct(char cmd_val)

{
    uint8_t take_index = 0; // Take_index zodat de plek in de buffer makkelijk gereset kan worden
    char errors = 0;
    char arg_diff;

    take_int(&take_index); // Skip het eerste woord


    switch (cmd_val) // Vul juiste struct en start juiste functie op basis van de in X functie (Piotr) gevonden commando
    {
        case LIJN: // Vul lijn struct en roep lijn functie aan
            {
                line_struct lijn;

				arg_diff = Argument_checker(LIJN_ARGS);
				if (arg_diff != 0)
					return;

				lijn.x_1 = take_int(&take_index);
				errors += check_coord(lijn.x_1, VGA_DISPLAY_X, "X_1");

                lijn.y_1 = take_int(&take_index);
                errors += check_coord(lijn.y_1, VGA_DISPLAY_Y, "Y_1");

                lijn.x_2 = take_int(&take_index);
                errors += check_coord(lijn.x_2, VGA_DISPLAY_X, "X_2");

                lijn.y_2 = take_int(&take_index);
                errors += check_coord(lijn.y_2, VGA_DISPLAY_Y, "Y_2");

                lijn.color = take_color(&take_index);
                if (lijn.color == -1) errors++;

                lijn.weight = take_int(&take_index); // ERROR HANDLING IN LOGIC LAYER

                if(errors > 0)
                {
                    USART2_SendString("Totaal aantal errors: ");
                    USART2_SendChar(errors);
                    USART2_SendString("\n");

                    return;
                }

                lineToVGA(lijn);
            }
            break;

        case RECHTHOEK: // Vul rechthoek struct en roep rechthoek functie aan
            {
                rectangle_struct rechthoek;

				arg_diff = Argument_checker(RECHTHOEK_ARGS);
				if (arg_diff != 0)
					return;

				rechthoek.x = take_int(&take_index);
				errors += check_coord(rechthoek.x, VGA_DISPLAY_X, "X");

                rechthoek.y = take_int(&take_index);
                errors += check_coord(rechthoek.y, VGA_DISPLAY_Y, "Y");

                rechthoek.width = take_int(&take_index);
                errors += check_coord((rechthoek.x + rechthoek.width), VGA_DISPLAY_X, "Breedte");

                rechthoek.height = take_int(&take_index);
                errors += check_coord((rechthoek.y + rechthoek.height), VGA_DISPLAY_Y, "Hoogte");

                rechthoek.color = take_color(&take_index);
                if (rechthoek.color == -1) errors++;

                rechthoek.filled = take_int(&take_index);
                if ((rechthoek.filled != 0) && (rechthoek.filled != 1))
                {
                    errors++;
                    USART2_SendString("Opvulling kan alleen 0 of 1 zijn \n");
                }

                if(errors > 0)
                {
                    USART2_SendString("Totaal aantal errors: ");
                    USART2_SendChar(errors);
                    USART2_SendString("\n");

                    return;
                }

                //LOGIC LAYER FUNCTIE
                rectangleToVGA(rechthoek);
            }
            break;

        case TEKST: // Vul text struct en roep text functie aan
            {
			text_struct text;
			int letter_w, letter_h;

            arg_diff = Argument_checker(TEKST_ARGS);
			if (arg_diff != 0)
				return;

            text.x_lup = take_int(&take_index);
			errors += check_coord(text.x_lup, VGA_DISPLAY_X, "text.x_lup");

            text.y_lup = take_int(&take_index);
			errors += check_coord(text.y_lup, VGA_DISPLAY_Y, "text.y_lup");

            text.color = take_color(&take_index);
            if (text.color == -1) errors++;

            text.text = take_word(&take_index); // Bij het pakken van een string gebruik primaire commando, deze moet na alle logica weer vrij gegeven worden
            if (text.text == NULL || strlen(text.text) == 0)
			{
				USART2_SendString("Tekst mag niet leeg zijn\n");
				errors++;
			}

            text.fontname = take_word(&take_index);
            if (text.fontname == NULL ||(strcmp(text.fontname, "arial") != 0 && strcmp(text.fontname, "consolas") != 0))
			{
				USART2_SendString("Ongeldige fontnaam (arial/consolas)\n");
				errors++;
			}

            text.fontsize = take_int(&take_index);
            if (text.fontsize != KLEIN && text.fontsize != GROOT)
            {
                USART2_SendString("Fontgrootte moet 1 (klein) of 2 (groot) zijn\n");
                errors++;
            }

            text.fontstyle = take_word(&take_index);
            if (text.fontstyle == NULL || (strcmp(text.fontstyle, "normaal") != 0 && strcmp(text.fontstyle, "vet") != 0 && strcmp(text.fontstyle, "cursief") != 0))
		    {
			   USART2_SendString("Fontstijl moet normaal, vet of cursief zijn\n");
			   errors++;
		    }

            if (!errors)
            {
                if (text.fontsize == GROOT) {
                    letter_w = SIZE_BIG_LETTER_X;
                    letter_h = SIZE_BIG_LETTER_Y;
                } else {
                    letter_w = SIZE_SMALL_LETTER_X;
                    letter_h = SIZE_SMALL_LETTER_Y;
                }

                errors += check_coord( text.x_lup + strlen(text.text) * letter_w, VGA_DISPLAY_X,"tekst breedte");

                errors += check_coord(text.y_lup + letter_h, VGA_DISPLAY_Y,"tekst hoogte");
            }

            if (errors > 0)
            {
                USART2_SendString("Totaal aantal errors: ");
                USART2_SendChar(errors + '0');
                USART2_SendString("\n");
                return;
            }

            textToVGA(text);


                // Geef geheugen vrij
                free(text.text);
                free(text.fontname);
            }
            return;

        case BITMAP: // Vul bitmap struct en roep bitmap functie aan
            {
                bitmap_struct bitmap;

            arg_diff = Argument_checker(BITMAP_ARGS);
			if (arg_diff != 0)
				return;

			bitmap.bm_nr = take_int(&take_index);
			errors += check_coord(bitmap.bm_nr, BITMAP_AMOUNT, "bitmap.bm_nr");

            bitmap.x_lup = take_int(&take_index);
			errors += check_coord(bitmap.x_lup, VGA_DISPLAY_X, "bitmap.x_lup");
			errors += check_coord(bitmap.x_lup + MAX_BITMAP_ARRAY, VGA_DISPLAY_X, "bitmap.x_lup");

			bitmap.y_lup = take_int(&take_index);
			errors += check_coord(bitmap.y_lup, VGA_DISPLAY_Y, "bitmap.y_lup");
			errors += check_coord(bitmap.y_lup + MAX_BITMAP_ARRAY, VGA_DISPLAY_X, "bitmap.x_lup");

			if (bitmap.bm_nr < 0 || bitmap.bm_nr > BITMAP_AMOUNT)
			{
			    USART2_SendString("De Bitmap functie die is ingevuld bestaat niet\n");
			    errors++;
			}

             if(errors > 0)
             {
                 USART2_SendString("Totaal aantal errors: ");
                 USART2_SendChar(errors);
                 USART2_SendString("\n");

                 return;
             }
             bitmapToVGA(bitmap);
            }
            return;

        case CLEARSCHERM: // Vul clearscherm struct en roep clearscherm functie aan
        {
            clearscreen_struct clearscherm;

            arg_diff = Argument_checker(CLEARSCHERM_ARGS);
			if (arg_diff != 0)
					return;

            clearscherm.color = take_color(&take_index);

                if (clearscherm.color == -1)
                    return;

                //LOGIC LAYER FUNCTIE
                clearScreenToVGA(clearscherm);
            }
            return;
    }
    return;
}

char Argument_checker(char Argument_goal)
{
	char argAmount = Argument_counter();
	char arg_diff = 0;

	if(argAmount > Argument_goal)
	{
		arg_diff = argAmount - Argument_goal;
		USART2_SendChar(arg_diff);
		USART2_SendString(" argument(en) te veel. \n");
	}
 	if(argAmount < Argument_goal)
	{
		arg_diff = Argument_goal - argAmount;
		USART2_SendChar(arg_diff);
		USART2_SendString(" argument(en) te weinig. \n");
	}

 	return arg_diff;
}

char Argument_counter()
{
    int8_t idx_check = 0;
    char argAmount = 0;

    // 1. Skip het commando woord ("lijn")
    take_int(&idx_check);
    // 2. Parse parameters op basis van buffer
    char *arg[MAX_ARG];

    for (char argCounter = 0; argCounter < MAX_ARG; argCounter++)
        arg[argCounter] = take_word(&idx_check);

    // Doe nu checks
    for(char argNum = 0; argNum < MAX_ARG; argNum++)
    {
        if (arg[argNum] != NULL)
        {
            argAmount++;
        }
        free(arg[argNum]);
    }

    return argAmount;
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
    if (*take_index >= idx) return NULL; // Alles al gelezen

    // Skip spaties voor het woord
    while (*take_index < idx && buffer[*take_index] == ' ')
        (*take_index)++;

    uint8_t start = *take_index;
    uint8_t len = 0;

    // Zoek het einde van het woord (komma of einde buffer)
    while (*take_index < idx && buffer[*take_index] != ',')
    {
        (*take_index)++;
        len++;
    }

    // Trim spaties, newline (\n) en carriage return (\r) van het einde
    while (len > 0 &&
            (buffer[start + len - 1] == ' ' ||
             buffer[start + len - 1] == '\n' ||
             buffer[start + len - 1] == '\r'))
    {
        len--;
    }

    // Allocate geheugen voor het woord
    char* word = malloc(len + 1);
    if (!word) return NULL;

    for (uint8_t j = 0; j < len; j++)
        word[j] = buffer[start + j];

    word[len] = '\0'; // Sluit af

    // Skip de komma als die er is
    if (*take_index < idx && buffer[*take_index] == ',') (*take_index)++;

    return word;
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

    USART2_SendString("De kleur die ingevuld is bestaat niet\n");
    return -1;
}

static uint8_t check_coord(int val, int max_val, const char* argument_name) {
    if (val < 0 || val > max_val) {
        USART2_SendString(argument_name);
        USART2_SendString(" coordinaat out of range\n");
        return 1;
    }
    return 0;
}


// UART ontvang en zend functies
void USART2_Init(void) {
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 = TX, PA3 = RX
    GPIOA->MODER |= (2 << (2*2)) | (2 << (3*2));
    GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));

    // Baudrate 115200 (check APB1 klok)
    uint32_t pclk1 = SystemCoreClock / 4;
    USART2->BRR = pclk1 / 115200;

    // Transmit enable, Receive enable, USART enable, Interrupt aanzetten bij ontvangen character
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

    // Enable NVIC (Nested Vector Interrupt Controller)
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE)
    {
        char c = USART2->DR;
        uint16_t next = (uart_head + 1) % UART_BUF_SIZE;
        if(next != uart_tail)
        {
            uart_buf[uart_head] = c;
            uart_head = next;
        }
    }
}

char USART2_ReceiveChar(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}

void USART2_BUFFER(void)
{
	static char UART_Flag = 1;

	if (UART_Flag == 1)
	{
		//UART_Flag = 0;
		while(uart_tail != uart_head) // zolang er chars in de ringbuffer
		{
			char c = uart_buf[uart_tail];
			uart_tail = (uart_tail + 1) % UART_BUF_SIZE;

			// Dynamische buffer aanmaken of vergroten
			if(buffer == NULL)
			{
				buffer = malloc(1);
				idx = 0;
			}
			else
			{
				char* tmp = realloc(buffer, idx + 1);
				if(tmp == NULL) continue; // memory fail
				buffer = tmp;
			}

			buffer[idx++] = c;

			if((c == '\n') && (UART_Flag == 1))  // einde commando
			{
				UART_Flag = 0;

				buffer[idx] = '\0'; // sluit string
				Buffer_Check();     // parse en teken meteen
				free(buffer);       // opruimen
				buffer = NULL;
				idx = 0;

				UART_Flag = 1;
				USART2_SendString("UART Ready!!!\n");
			}
		}
	}
}

void USART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void USART2_SendString(char *str) {


    while (*str) USART2_SendChar(*str++);
}
