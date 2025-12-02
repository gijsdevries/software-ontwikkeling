void USART2_BUFFER();
char USART2_ReceiveChar();
void USART2_SendString();
void USART2_SendChar(char c);
void USART2_Init();
void Buffer_to_struct(char cmd_var);
char* take_word(uint8_t *take_index);
int take_int(uint8_t *take_index);
