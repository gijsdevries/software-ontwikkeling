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

        Buffer_Check();

        idx = 0;

        free(buffer);
        buffer = NULL;

        //parse
    }
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
            if (arg_diff != 0)return;

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
			if (arg_diff != 0)return;

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

            arg_diff = Argument_checker(TEKST_ARGS);
			if (arg_diff != 0)return;

            text.x_lup = take_int(&take_index);
            text.y_lup = take_int(&take_index);
            text.color = take_color(&take_index);
            text.text = take_word(&take_index); // Bij het pakken van een string gebruik primaire commando, deze moet na alle logica weer vrij gegeven worden
            text.fontname = take_word(&take_index); // Zelfde hier
            text.fontsize = take_int(&take_index);
            text.fontstyle = take_int(&take_index);

                //LOGIC LAYER FUNCTIE TODO

                // Geef geheugen vrij
                free(text.text);
                free(text.fontname);
            }
            break;

        case BITMAP: // Vul bitmap struct en roep bitmap functie aan
            {
                bitmap_struct bitmap;

            arg_diff = Argument_checker(BITMAP_ARGS);
			if (arg_diff != 0)return;

            bitmap.x_lup = take_int(&take_index);
            bitmap.y_lup = take_int(&take_index);
            bitmap.bm_nr = take_int(&take_index);

                //LOGIC LAYER FUNCTIE TODO
            }
            break;

        case CLEARSCHERM: // Vul clearscherm struct en roep clearscherm functie aan
        {
            clearscreen_struct clearscherm;

            arg_diff = Argument_checker(CLEARSCHERM_ARGS);
			if (arg_diff != 0)return;

            clearscherm.color = take_color(&take_index);

                if (clearscherm.color == -1)
                    return;

                //LOGIC LAYER FUNCTIE
                clearScreenToVGA(clearscherm);
            }
            break;
    }
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

void Buffer_Check()
{
    char cmd_var;

    for (int i = 0; i < NUM_COMMANDS; i++) //Controleert hoeveel commando's erin de define staan
    {
        //Is er een match in het eerste woord van de commando en de define code
        if (strncmp(buffer, commands[i].name, strlen(commands[i].name)) == 0)
        {
            cmd_var = commands[i].code;
            //USART2_SendChar(cmd_var);
            //USART2_SendChar('\n');
            Buffer_to_struct(cmd_var);

            return;
        }
    }

    USART2_SendString("ERROR: Onbekend commando\n");
    USART2_SendString("Herzie het woord voor de eerste komma\n");

}

static uint8_t check_coord(int val, int max_val, const char* argument_name) {
    if (val < 0 || val > max_val) {
        USART2_SendString(argument_name);
        USART2_SendString(" coordinaat out of range\n");
        return 1;
    }
    return 0;
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
