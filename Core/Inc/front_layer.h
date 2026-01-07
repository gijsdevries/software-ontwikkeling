/** \file
 * front layer h 
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <global.h>

// Toevoegen bovenaan bij de parsing/UART sectie
#define UART_BUF_SIZE 1024  // of groter, afhankelijk van maximale commando lengte
                             
extern volatile char uart_buf[UART_BUF_SIZE];
extern volatile uint16_t uart_head;
extern volatile uint16_t uart_tail;
extern volatile uint8_t command_ready;   // flag om te weten dat er een compleet commando is
extern char *buffer;     // dynamische buffer
extern uint16_t idx;

// Prototypes voor parsing en checking
void Buffer_Check();
void Buffer_to_struct(char cmd_var);
char Argument_checker(char Argument_goal);
char Argument_counter();
int take_int(uint8_t *take_index);
char* take_word(uint8_t *take_index);
int take_color(uint8_t *take_index);
static uint8_t check_coord(int val, int max_val, const char* argument_name);

// Prototypes voor UART
void USART2_Init();
char USART2_ReceiveChar();
void USART2_BUFFER();
void USART2_SendChar(char c);
void USART2_SendString();

// Argument defines
#define MAX_ARG 7
#define LIJN_ARGS 6
#define RECHTHOEK_ARGS 6
#define TEKST_ARGS 7
#define BITMAP_ARGS 3
#define CLEARSCHERM_ARGS 1
#define BITMAP_AMOUNT 6
#define MAX_BITMAP_ARRAY 32

//De functienaam koppelen aan de define code
typedef struct {
  const char *name;
  char code;
} Eerste_woord;

// Aantal commando types
static Eerste_woord commands[] = {
  {"lijn",        LIJN},
  {"rechthoek",   RECHTHOEK},
  {"tekst",       TEKST},
  {"bitmap",      BITMAP},
  {"clearscherm", CLEARSCHERM}
};

// Bereken aantal commando's
#define NUM_COMMANDS (sizeof(commands)/sizeof(commands[0]))


#endif
