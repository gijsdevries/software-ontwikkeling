//Prototypes voor UART
char USART2_ReceiveChar();
void USART2_SendString();
void USART2_SendChar(char c);
void USART2_Init();

//Prototypes voor buffer
void USART2_BUFFER();
void Buffer_Check();

//Define waardes van de fucnties
#define LIJN 0
#define RECHTHOEK 1
#define TEKST 2
#define BITMAP 3
#define CLEARSCHERM 4

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
