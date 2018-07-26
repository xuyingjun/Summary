#ifndef __USART_H
#define __USART_H

void UART_Configuration(void);
void SendStr4851(char *str);
void SendStr4852(char *str);
void printfStr(char *str);
void LedInit(void);
char Usart_GetChar(USART_TypeDef* USARTx);
void UsendChar(char dat);
void Usart2SendData(int num,char* sendbuffer);


#endif

