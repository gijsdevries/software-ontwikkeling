#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <global.h>

//Prototypes voor UART
char USART2_ReceiveChar();
void USART2_SendString();
void USART2_SendChar(char c);
void USART2_Init();

//Prototypes voor buffer
void USART2_BUFFER();
void Buffer_Check();
void Argument_counter();
void Clearscherm_check();

#define MAX_ARG 6

//De naam van het eerste woorde koppelen aan de define code
typedef struct {
    const char *name;
    char code;
} Eerste_woord;

//Alle mogelijke eerste woorden in het commando
static Eerste_woord commands[] = {
    {"lijn",        LIJN},
    {"rechthoek",   RECHTHOEK},
    {"tekst",       TEKST},
    {"bitmap",      BITMAP},
    {"clearscherm", CLEARSCHERM}
};

//Berekend de waarde van het aantal commando's
#define NUM_COMMANDS (sizeof(commands)/sizeof(commands[0]))
void Buffer_to_struct(char cmd_var);
char* take_word(uint8_t *take_index);
int take_int(uint8_t *take_index);
int take_color(uint8_t *take_index);
static uint8_t check_coord(int val, int max_val, const char* argument_name);

#endif
