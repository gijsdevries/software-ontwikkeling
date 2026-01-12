/**
 * @file front_layer.h
 * @brief Dit bestand bevat de declaraties voor de front-laag,
 *        inclusief UART-communicatie, commando-parsing en argumentcontrole.
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <global.h>

/**
 * @brief Grootte van de UART ontvangstbuffer.
 */
#define UART_BUF_SIZE 1024

/**
 * @brief UART ontvangstbuffer.
 */
extern volatile char uart_buf[UART_BUF_SIZE];

/**
 * @brief Hoofdindex van de UART ontvangstbuffer.
 */
extern volatile uint16_t uart_head;

/**
 * @brief Staartindex van de UART ontvangstbuffer.
 */
extern volatile uint16_t uart_tail;

/**
 * @brief Vlag die aangeeft of een compleet commando is ontvangen.
 */
extern volatile uint8_t command_ready;

/**
 * @brief Dynamische buffer voor commando-parsing.
 */
extern char *buffer;

/**
 * @brief Index voor de dynamische buffer.
 */
extern uint16_t idx;

// Prototypes voor parsing en checking
/**
 * @brief Controleert de UART-buffer op nieuwe gegevens en verwerkt deze.
 */
void Buffer_Check();

/**
 * @brief Parsed het commando uit de buffer en converteert het naar een gestructureerd formaat.
 * @param cmd_var De te verwerken commando-variabele.
 */
void Buffer_to_struct(char cmd_var);

/**
 * @brief Controleert of een specifiek argument aanwezig en geldig is.
 * @param Argument_goal Het te controleren argument.
 * @return Het gecontroleerde argument.
 */
char Argument_checker(char Argument_goal);

/**
 * @brief Telt het aantal argumenten in het huidige commando.
 * @return Het aantal argumenten.
 */
char Argument_counter();

/**
 * @brief Extraheert een integerwaarde uit de commandobuffer.
 * @param take_index Pointer naar de huidige index in de buffer.
 * @return De integerwaarde.
 */
int take_int(uint8_t *take_index);

/**
 * @brief Extraheert een woord (string) uit de commandobuffer.
 * @param take_index Pointer naar de huidige index in de buffer.
 * @return Pointer naar het woord.
 */
char* take_word(uint8_t *take_index);

/**
 * @brief Extraheert een kleurwaarde uit de commandobuffer.
 * @param take_index Pointer naar de huidige index in de buffer.
 * @return De kleurwaarde.
 */
int take_color(uint8_t *take_index);

/**
 * @brief Controleert of een coördinaatwaarde binnen het geldige bereik ligt.
 * @param val De te controleren coördinaatwaarde.
 * @param max_val De maximaal toegestane waarde voor de coördinaat.
 * @param argument_name De naam van het argument voor foutrapportage.
 * @return 1 als de coördinaat geldig is, 0 anders.
 */
static uint8_t check_coord(int val, int max_val, const char* argument_name);

// Prototypes voor UART
/**
 * @brief Initialiseert de USART2-peripheral voor UART-communicatie.
 */
void USART2_Init();

/**
 * @brief Verwerkt de USART2 ontvangstbuffer.
 */
void USART2_BUFFER();

/**
 * @brief Verstuurt een enkel teken via USART2.
 * @param char c Het te versturen teken.
 */
void USART2_SendChar(char c);

/**
 * @brief Verstuurt een string via USART2.
 */
void USART2_SendString();

/**
 * @brief Zet een int om in een str en verstuur via UART.
 */
void USART2_SendCharUser(char c);


// Argument defines
/**
 * @brief Maximaal aantal argumenten toegestaan voor een commando.
 */

#define MAX_ARG 7

/**
 * @brief Aantal argumenten voor het "lijn" commando.
 */
#define LIJN_ARGS 6

/**
 * @brief Aantal argumenten voor het "rechthoek" commando.
 */
#define RECHTHOEK_ARGS 6

/**
 * @brief Aantal argumenten voor het "tekst" commando.
 */
#define TEKST_ARGS 7

/**
 * @brief Aantal argumenten voor het "bitmap" commando.
 */
#define BITMAP_ARGS 3

/**
 * @brief Aantal argumenten voor het "clearscherm" commando.
 */
#define CLEARSCHERM_ARGS 1

/**
 * @brief Aantal argumenten voor het "cirkel" commando.
 */
#define CIRKEL_ARGS 4

/**
 * @brief Aantal argumenten voor het "wait" commando.
 */
#define WAIT_ARGS 1

/**
 * @brief Aantal bitmaps.
 */
#define BITMAP_AMOUNT 6

/**
 * @brief Maximale grootte van de bitmap-array.
 */
#define MAX_BITMAP_ARRAY 32

/**
 * @brief Structuur om commandonamen te koppelen aan hun corresponderende define-codes.
 */
typedef struct {
  const char *name; /**< De naam van het commando. */
  char code;        /**< De code geassocieerd met het commando. */
} Eerste_woord;

/**
 * @brief Array van ondersteunde commando's en hun codes.
 */
static Eerste_woord commands[] = {
  {"lijn",        LIJN},
  {"rechthoek",   RECHTHOEK},
  {"tekst",       TEKST},
  {"bitmap",      BITMAP},
  {"clearscherm", CLEARSCHERM},
  {"cirkel", 	  CIRKEL},
  {"wacht",		  WAIT}
};

/**
 * @brief Berekent het aantal ondersteunde commando's.
 */
#define NUM_COMMANDS (sizeof(commands)/sizeof(commands[0]))


#endif
