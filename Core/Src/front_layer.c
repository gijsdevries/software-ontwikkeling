/**
 * @file front_layer.c
 * @brief Behandelt de communicatie via UART en het parsen van commando's.
 *
 * Deze laag ontvangt commando's via UART, Parsed deze, en roept de
 * corresponderende functies in de logic_layer aan om te tekenen op het VGA-scherm.
 */

#include "stm32f4xx.h"
#include "front_layer.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include "bitmap_defines.h"
#include <string.h>
#include <stdlib.h>

volatile char uart_buf[UART_BUF_SIZE]; ///< Ringbuffer voor UART-ontvangst.
volatile uint16_t uart_head = 0;       ///< Schrijfpointer voor de UART-ringbuffer.
volatile uint16_t uart_tail = 0;       ///< Leespointer voor de UART-ringbuffer.

char *buffer = NULL;    ///< Dynamische buffer voor het opslaan van een volledig commando.
uint16_t idx = 0;       ///< Index voor de dynamische buffer.

// Functie declaraties voor interne functies
void Buffer_Check(void);
void Buffer_to_struct(char cmd_val);
char Argument_checker(char Argument_goal);
char Argument_counter(void);
int take_int(uint8_t *take_index);
char* take_word(uint8_t *take_index);
int take_color(uint8_t *take_index);
static uint8_t check_coord(int val, int max_val, const char* argument_name);


/**
 * @brief Controleert en identificeert het commando in de buffer.
 *
 * Vergelijkt het eerste woord in de buffer met een lijst van bekende commando's.
 * Als een match wordt gevonden, wordt de bijbehorende functie aangeroepen om
 * het commando verder te verwerken. Anders wordt een foutmelding verzonden.
 */
void Buffer_Check()
{
  char cmd_var;

  /*

  USART2_SendString("buffer: ");

  for (int j = 0; j < 255; j++) {
    if (buffer[j] == ',')
      break;
    USART2_SendChar(buffer[j]);
  }

  USART2_SendString("\r\n");
  */

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

  USART2_SendString("ERROR: Onbekend commando\r\n");
  USART2_SendString("Herzie het woord voor de eerste komma\r\n");
}

/**
 * @brief Parsed de argumenten uit de buffer en vult de juiste datastructuur.
 *
 * @param cmd_val De code van het commando dat verwerkt moet worden.
 *
 * Afhankelijk van het commando worden de argumenten uit de buffer gehaald,
 * geconverteerd naar de juiste datatypes, en opgeslagen in een struct.
 * Na validatie wordt de bijbehorende tekenfunctie in de logic_layer aangeroepen.
 */
void Buffer_to_struct(char cmd_val)
{
  uint8_t take_index = 0; // Take_index zodat de plek in de buffer makkelijk gereset kan worden
  char errors = 0;
  char arg_diff;

  take_int(&take_index); // Skip het eerste woord

  switch (cmd_val) // Vul juiste struct, start juiste functie en error handling
  {
    case LIJN: // Vul lijn struct en roep lijn functie aan
      {
        USART2_SendString("Lijn command ontvangen\r\n");

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

        lijn.weight = take_int(&take_index);
        if (lijn.weight <= 0)
        {
          USART2_SendString("Dikte moet een positief getal zijn\r\n");
          errors++;
        }

        // Error report
        if(errors > 0)
        {
          USART2_SendString("Totaal aantal errors: ");
          USART2_SendChar(errors + '0');
          USART2_SendString("\r\n");

          return;
        }

        //LOGIC LAYER FUNCTIE
        lineToVGA(lijn);
      }
      break;

    case RECHTHOEK: // Vul rechthoek struct en roep rechthoek functie aan
      {
        USART2_SendString("RECHTHOEK command ontvangen\r\n");
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
          USART2_SendString("Opvulling kan alleen 0 of 1 zijn \r\n");
        }

        // Error report
        if(errors > 0)
        {
          USART2_SendString("Totaal aantal errors: ");
          USART2_SendChar(errors);
          USART2_SendString("\r\n");

          return;
        }

        //LOGIC LAYER FUNCTIE
        rectangleToVGA(rechthoek);
      }
      break;

    case TEKST: // Vul text struct en roep text functie aan
      {
        USART2_SendString("TEKST command ontvangen\r\n");

        text_struct text;

        text.text = NULL;
        text.fontname = NULL;
        text.fontstyle = NULL;

        arg_diff = Argument_checker(TEKST_ARGS);
        if (arg_diff != 0)
          return;

        text.x_lup = take_int(&take_index);

        text.y_lup = take_int(&take_index);

        text.color = take_color(&take_index);
        if (text.color == -1) errors++;

        text.text = take_word(&take_index);
        if (text.text == NULL || strlen(text.text) == 0)
        {
          USART2_SendString("Tekst mag niet leeg zijn\r\n");
          errors++;
        }

        text.fontname = take_word(&take_index);
        if (text.fontname == NULL || (strcmp(text.fontname, "arial") != 0 && strcmp(text.fontname, "consolas") != 0))
        {
          USART2_SendString("Ongeldige fontnaam (arial/consolas)\r\n");
          errors++;
        }

        text.fontsize = take_int(&take_index);
        if (text.fontsize < KLEIN || text.fontsize > GROOT)
        {
          USART2_SendString("Fontgrootte moet 1 (klein) of 2 (groot) zijn\r\n");
          errors++;
        }

        //TODO add a define
        int letter_marge = 8;

        if (text.fontsize == GROOT) {
          letter_marge *= 2;
        }

        errors += check_coord(text.x_lup + letter_marge, VGA_DISPLAY_X, "text.x_lup");
        errors += check_coord(text.y_lup + letter_marge, VGA_DISPLAY_Y, "text.y_lup");

        text.fontstyle = take_word(&take_index);
        if (text.fontstyle == NULL || (strcmp(text.fontstyle, "normaal") != 0 && strcmp(text.fontstyle, "vet") != 0 && strcmp(text.fontstyle, "cursief") != 0))
        {
          USART2_SendString("Fontstijl moet normaal, vet of cursief zijn\r\n");
          errors++;
        }

        if (!errors)
        {
          //errors += check_coord(text.x_lup + strlen(text.text) * letter_w, VGA_DISPLAY_X,"tekst breedte");
          //errors += check_coord(text.y_lup + letter_h, VGA_DISPLAY_Y,"tekst hoogte");
        }

        // Error report
        if (errors > 0)
        {
          USART2_SendString("Totaal aantal errors: ");
          USART2_SendChar(errors + '0');
          USART2_SendString("\r\n");

          // Free memory if it was allocated before returning
          free(text.text);
          free(text.fontname);
          free(text.fontstyle);
          return;
        }

        //LOGIC LAYER FUNCTIE
        textToVGA(text);


        // Geef geheugen vrij
        free(text.text);
        free(text.fontname);
        free(text.fontstyle);
      }
      break;

    case BITMAP: // Vul bitmap struct en roep bitmap functie aan
      {
        USART2_SendString("BITMAP command ontvangen\r\n");

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
          USART2_SendString("De Bitmap functie die is ingevuld bestaat niet\r\n");
          errors++;
        }

        // Error report
        if(errors > 0)
        {
          USART2_SendString("Totaal aantal errors: ");
          USART2_SendChar(errors);
          USART2_SendString("\r\n");

          return;
        }

        //LOGIC LAYER FUNCTIE
        bitmapToVGA(bitmap);
      }
      break;

    case CLEARSCHERM: // Vul clearscherm struct en roep clearscherm functie aan
      {
        USART2_SendString("CLEARSCHERM command ontvangen\r\n");

        clearscreen_struct clearscherm;

        arg_diff = Argument_checker(CLEARSCHERM_ARGS);
        if (arg_diff != 0)
          return;

        clearscherm.color = take_color(&take_index);

        if (clearscherm.color == -1) errors++;

        // Error report
        if(errors > 0)
        {
          USART2_SendString("Totaal aantal errors: ");
          USART2_SendChar(errors);
          USART2_SendString("\r\n");

          return;
        }

        //LOGIC LAYER FUNCTIE
        clearScreenToVGA(clearscherm);
      }
      break;
  }
  return;
}

/**
 * @brief Controleert of het aantal argumenten correct is.
 *
 * @param Argument_goal is verwachte aantal argumenten voor het commando.
 * @return Het verschil tussen het gevonden en verwachte aantal argumenten.
 *         Retourneert 0 als het aantal correct is.
 *
 * Telt het aantal argumenten in de buffer en vergelijkt dit met het
 * verwachte aantal. Geeft een foutmelding als het aantal niet klopt.
 */
char Argument_checker(char Argument_goal)
{
  char argAmount = Argument_counter();
  char arg_diff = 0;

  if(argAmount > Argument_goal)
  {
    arg_diff = argAmount - Argument_goal;
    USART2_SendChar(arg_diff);
    USART2_SendString(" argument(en) te veel. \r\n");
  }
  if(argAmount < Argument_goal)
  {
    arg_diff = Argument_goal - argAmount;
    USART2_SendChar(arg_diff);
    USART2_SendString(" argument(en) te weinig. \r\n");
  }

  return arg_diff;
}

/**
 * @brief Telt het aantal argumenten in de buffer.
 *
 * @return Het aantal gevonden argumenten.
 *
 * De functie parsed de buffer en telt het aantal door komma's gescheiden
 * waarden na het commando-woord.
 */
char Argument_counter()
{
  int8_t idx_check = 0;
  char argAmount = 0;

  // Skip het commando woord
  take_int(&idx_check);
  // Parse parameters op basis van buffer
  char *arg[MAX_ARG];

  for (char argCounter = 0; argCounter < MAX_ARG; argCounter++)
    arg[argCounter] = take_word(&idx_check);

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

/**
 * @brief Haalt een integer-waarde uit de buffer.
 *
 * @param take_index Pointer naar de huidige leespositie in de buffer.
 * @return De geconverteerde integer-waarde.
 *
 * Leest het volgende 'woord' uit de buffer, converteert dit naar een integer
 * en verhoogt de leespositie.
 */
int take_int(uint8_t *take_index)
{
  char* argument = take_word(take_index); // Pak het woord uit de buffer
  int int_argument = atoi(argument); // Converteer woord naar int
  free(argument); // Geef geheugen vrij

  return int_argument; // Geef int terug
}

/**
 * @brief Haalt een 'woord' (string) uit de buffer.
 *
 * @param take_index Pointer naar de huidige leespositie in de buffer.
 * @return Een pointer naar de gealloceerde string met het woord.
 *         De aanroeper is verantwoordelijk voor het vrijgeven van dit geheugen.
 *         Retourneert NULL als er geen woorden meer zijn of bij een memory allocation failure.
 *
 * Een 'woord' wordt gedefinieerd als een reeks karakters gescheiden door een komma
 * of het einde van de buffer. Spaties en newlines aan het begin en einde
 * worden verwijderd.
 */
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

/**
 * @brief Haalt een kleurnaam uit de buffer en converteert deze naar een kleurcode.
 *
 * @param take_index Pointer naar de huidige leespositie in de buffer.
 * @return De kleurcode (gedefinieerd in stm32_ub_vga_screen.h) of -1 bij een ongeldige kleur.
 *
 * Leest het volgende woord, vergelijkt het met een lijst van bekende kleurnamen
 * en retourneert de bijbehorende kleurcode.
 */
int take_color(uint8_t *take_index)
{
  char* color_arg = take_word(take_index);
  int color = -1;

  if(color_arg == NULL)
  {
    return -1; // Malloc failed in take_word
  }

  //If-tree voor het bepalen van kleurwaardes
  if (strcmp(color_arg, "zwart") == 0)
    color = VGA_COL_BLACK;
  else if (strcmp(color_arg, "blauw") == 0)
    color = VGA_COL_BLUE;
  else if (strcmp(color_arg, "groen") == 0)
    color = VGA_COL_GREEN;
  else if (strcmp(color_arg, "rood") == 0)
    color = VGA_COL_RED;
  else if (strcmp(color_arg, "wit") == 0)
    color = VGA_COL_WHITE;
  else if (strcmp(color_arg, "cyaan") == 0)
    color = VGA_COL_CYAN;
  else if (strcmp(color_arg, "magenta") == 0)
    color = VGA_COL_MAGENTA;
  else if (strcmp(color_arg, "geel") == 0)
    color = VGA_COL_YELLOW;
  else if (strcmp(color_arg, "lichtblauw") == 0)
    color = VGA_COL_LIGHT_BLUE;
  else if (strcmp(color_arg, "lichtgroen") == 0)
    color = VGA_COL_LIGHT_GREEN;
  else if (strcmp(color_arg, "lichtcyaan") == 0)
    color = VGA_COL_LIGHT_CYAN;
  else if (strcmp(color_arg, "lichtrood") == 0)
    color = VGA_COL_LIGHT_RED;
  else if (strcmp(color_arg, "lichtmagenta") == 0)
    color = VGA_COL_LIGHT_MAGENTA;
  else if (strcmp(color_arg, "bruin") == 0)
    color = VGA_COL_BROWN;
  else if (strcmp(color_arg, "grijs") == 0)
    color = VGA_COL_GREY;
  else
  {
    USART2_SendString("De kleur die ingevuld is bestaat niet\r\n");
  }

  free(color_arg);
  return color;
}

/**
 * @brief Controleert of een coördinaat binnen het geldige bereik valt.
 *
 * @param val De te controleren waarde.
 * @param max_val De maximale toegestane waarde.
 * @param argument_name De naam van het argument (voor de foutmelding).
 * @return 1 als de coördinaat ongeldig is, anders 0.
 */
static uint8_t check_coord(int val, int max_val, const char* argument_name) {
  if (val < 0 || val > max_val) {
    USART2_SendString(argument_name);
    USART2_SendString(" coordinaat out of range\r\n");
    return 1;
  }
  return 0;
}


//============================================================================= Start
/**
 * @brief Initialiseert de USART2-poort.
 *
 * Configureert de GPIO-pinnen (PA2 als TX, PA3 als RX), stelt de baudrate in op 115200,
 * en activeert de USART-module met interrupts voor ontvangst.
 * Gemaakt op basis van AI-gegenereerde code
 */
void USART2_Init(void)
{
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

/**
 * @brief Interrupt handler voor USART2.
 *
 * Wordt aangeroepen wanneer een karakter is ontvangen. Leest het karakter
 * uit het data register en plaatst het in de ringbuffer `uart_buf`.
 */
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

/**
 * @brief Verwerkt de data in de UART-ringbuffer.
 *
 * Leest karakters uit de ringbuffer en voegt ze toe aan een dynamische
 * commando-buffer. Wanneer een newline-karakter wordt gedetecteerd,
 * wordt het volledige commando verwerkt door `Buffer_Check()`.
 */
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

      if ((c == '\n') && (UART_Flag == 1))  // einde commando
      {
        UART_Flag = 0;

        buffer[idx] = '\0'; // sluit string
        Buffer_Check();     // parse en teken meteen
        free(buffer);       // opruimen
        buffer = NULL;
        idx = 0;

        UART_Flag = 1;
        //USART2_SendString("UART ready!!!\r\n");
      }
    }
  }
}

//============================================================================= Start
/**
 * @brief Verzendt een enkel karakter via USART2.
 *
 * @param c Het te verzenden karakter.
 *
 * Gemaakt op basis van AI-gegenereerde code
 * Wacht tot de transmit buffer leeg is en schrijft dan het karakter.
 */
void USART2_SendChar(char c)
{
  while (!(USART2->SR & USART_SR_TXE));
  USART2->DR = c;
}

/**
 * @brief Verzendt een string via USART2.
 *
 * @param str Pointer naar de te verzenden, null-terminated string.
 */
void USART2_SendString(char *str)
{
  while (*str) USART2_SendChar(*str++);
}
//============================================================================= Einde
