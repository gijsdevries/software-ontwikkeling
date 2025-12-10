#include "stm32f4xx.h"
#include "front_layer.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include <string.h>
#include <stdlib.h>  // alleen nodig voor atoi

// ---------------- UART Ringbuffer ----------------
#define UART_BUF_SIZE 256
volatile char uart_buf[UART_BUF_SIZE];
volatile uint16_t uart_head = 0;
volatile uint16_t uart_tail = 0;
volatile uint8_t uart_ready = 1; // ready flag

// ---------------- Parsing Buffer ----------------
#define PARSE_BUF_SIZE 128
char buffer[PARSE_BUF_SIZE];
uint8_t idx = 0;

// ---------------- Parsing en checking functies ----------------
void Buffer_Check(void)
{
    char cmd_var;

    for (int i = 0; i < NUM_COMMANDS; i++)
    {
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
    uint8_t take_index = 0;
    char errors = 0;
    char arg_diff;

    take_int(&take_index); // skip commando

    switch (cmd_val)
    {
        case LIJN:
        {
            line_struct lijn;

            arg_diff = Argument_checker(LIJN_ARGS);
            if (arg_diff != 0) return;

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

            lijn.weight = take_int(&take_index);

            if (errors > 0)
            {
                USART2_SendString("Totaal aantal errors: ");
                USART2_SendChar(errors);
                USART2_SendString("\n");
                return;
            }

            lineToVGA(lijn);
        }
        break;

        case RECHTHOEK:
        {
            rectangle_struct rechthoek;

            arg_diff = Argument_checker(RECHTHOEK_ARGS);
            if (arg_diff != 0) return;

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
                USART2_SendString("Opvulling kan alleen 0 of 1 zijn\n");
            }

            if (errors > 0)
            {
                USART2_SendString("Totaal aantal errors: ");
                USART2_SendChar(errors);
                USART2_SendString("\n");
                return;
            }

            rectangleToVGA(rechthoek);
        }
        break;

        case TEKST:
        {
            text_struct text;

            arg_diff = Argument_checker(TEKST_ARGS);
            if (arg_diff != 0) return;

            text.x_lup = take_int(&take_index);
            text.y_lup = take_int(&take_index);
            text.color = take_color(&take_index);

            take_word_to_buffer(&take_index, text.text, sizeof(text.text));
            take_word_to_buffer(&take_index, text.fontname, sizeof(text.fontname));

            text.fontsize = take_int(&take_index);
            text.fontstyle = take_int(&take_index);

            // TODO: LOGIC LAYER FUNCTIE
        }
        break;

        case BITMAP:
        {
            bitmap_struct bitmap;

            arg_diff = Argument_checker(BITMAP_ARGS);
            if (arg_diff != 0) return;

            bitmap.x_lup = take_int(&take_index);
            bitmap.y_lup = take_int(&take_index);
            bitmap.bm_nr = take_int(&take_index);

            // TODO: LOGIC LAYER FUNCTIE
        }
        break;

        case CLEARSCHERM:
        {
            clearscreen_struct clearscherm;

            arg_diff = Argument_checker(CLEARSCHERM_ARGS);
            if (arg_diff != 0) return;

            clearscherm.color = take_color(&take_index);
            if (clearscherm.color == -1) return;

            clearScreenToVGA(clearscherm);
        }
        break;
    }
}

// ---------------- Argument Checker ----------------
char Argument_checker(char Argument_goal)
{
    char argAmount = Argument_counter();
    char arg_diff = 0;

    if (argAmount > Argument_goal)
    {
        arg_diff = argAmount - Argument_goal;
        USART2_SendChar(arg_diff);
        USART2_SendString(" argument(en) te veel.\n");
    }
    if (argAmount < Argument_goal)
    {
        arg_diff = Argument_goal - argAmount;
        USART2_SendChar(arg_diff);
        USART2_SendString(" argument(en) te weinig.\n");
    }

    return arg_diff;
}

char Argument_counter(void)
{
    int8_t idx_check = 0;
    char argAmount = 0;

    take_int(&idx_check); // skip command

    char tmp[MAX_ARG][32] = {0};

    for (char i = 0; i < MAX_ARG; i++)
        take_word_to_buffer(&idx_check, tmp[i], sizeof(tmp[i]));

    for (char i = 0; i < MAX_ARG; i++)
    {
        if (tmp[i][0] != '\0') argAmount++;
    }

    return argAmount;
}

// ---------------- Take Functions ----------------
int take_int(uint8_t *take_index)
{
    char tmp[16] = {0};
    take_word_to_buffer(take_index, tmp, sizeof(tmp));
    return atoi(tmp);
}

void take_word_to_buffer(uint8_t *take_index, char *dest, uint8_t size)
{
    if (*take_index >= idx) { dest[0] = '\0'; return; }

    while (*take_index < idx && buffer[*take_index] == ' ')
        (*take_index)++;

    uint8_t start = *take_index;
    uint8_t len = 0;

    while (*take_index < idx && buffer[*take_index] != ',')
    {
        (*take_index)++;
        len++;
        if (len >= size-1) break; // buffer overflow check
    }

    while (len > 0 &&
           (buffer[start + len - 1] == ' ' ||
            buffer[start + len - 1] == '\n' ||
            buffer[start + len - 1] == '\r'))
    {
        len--;
    }

    for (uint8_t i = 0; i < len; i++)
        dest[i] = buffer[start + i];
    dest[len] = '\0';

    if (*take_index < idx && buffer[*take_index] == ',') (*take_index)++;
}

// ---------------- Take Color ----------------
int take_color(uint8_t *take_index)
{
    char tmp[32];
    take_word_to_buffer(take_index, tmp, sizeof(tmp));

    if (strcmp(tmp, "zwart") == 0) return VGA_COL_BLACK;
    if (strcmp(tmp, "blauw") == 0) return VGA_COL_BLUE;
    if (strcmp(tmp, "groen") == 0) return VGA_COL_GREEN;
    if (strcmp(tmp, "rood") == 0) return VGA_COL_RED;
    if (strcmp(tmp, "wit") == 0) return VGA_COL_WHITE;
    if (strcmp(tmp, "cyaan") == 0) return VGA_COL_CYAN;
    if (strcmp(tmp, "magenta") == 0) return VGA_COL_MAGENTA;
    if (strcmp(tmp, "geel") == 0) return VGA_COL_YELLOW;
    if (strcmp(tmp, "lichtblauw") == 0) return VGA_COL_LIGHT_BLUE;
    if (strcmp(tmp, "lichtgroen") == 0) return VGA_COL_LIGHT_GREEN;
    if (strcmp(tmp, "lichtcyaan") == 0) return VGA_COL_LIGHT_CYAN;
    if (strcmp(tmp, "lichtrood") == 0) return VGA_COL_LIGHT_RED;
    if (strcmp(tmp, "lichtmagenta") == 0) return VGA_COL_LIGHT_MAGENTA;
    if (strcmp(tmp, "bruin") == 0) return VGA_COL_BROWN;
    if (strcmp(tmp, "grijs") == 0) return VGA_COL_GREY;

    USART2_SendString("De kleur die ingevuld is bestaat niet\n");
    return -1;
}

// ---------------- Check Coord ----------------
static uint8_t check_coord(int val, int max_val, const char* argument_name)
{
    if (val < 0 || val > max_val)
    {
        USART2_SendString(argument_name);
        USART2_SendString(" coordinaat out of range\n");
        return 1;
    }
    return 0;
}

// ---------------- UART ----------------
void USART2_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER |= (2 << (2*2)) | (2 << (3*2));
    GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));

    uint32_t pclk1 = SystemCoreClock / 4;
    USART2->BRR = pclk1 / 115200;

    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE)
    {
        char c = USART2->DR;
        uint16_t next = (uart_head + 1) % UART_BUF_SIZE;
        if (next != uart_tail)
        {
            uart_buf[uart_head] = c;
            uart_head = next;
        }
    }
}

void USART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void USART2_SendString(char *str)
{
    while (*str) USART2_SendChar(*str++);
}

// ---------------- Process char from main-loop ----------------
void USART2_ProcessChar(char c)
{
    if (idx < PARSE_BUF_SIZE-1)
        buffer[idx++] = c;

    if (c == '\n')
    {
        buffer[idx] = '\0';
        Buffer_Check();
        idx = 0;
    }
}
