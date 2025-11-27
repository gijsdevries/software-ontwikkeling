#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

void USART2_BUFFER();
char USART2_ReceiveChar();
void USART2_SendString();
void USART2_SendChar(char c);
void USART2_Init();

#endif
