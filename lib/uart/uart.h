#ifndef UART_H_
#define UART_H_

#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>


void USART_Init( unsigned int ubrr );

uint8_t send_data(char data);


#endif