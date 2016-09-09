
#include "uart.h"
#include <stdio.h>

static int put(char character, FILE* file) {
	send_data(character);
	return 0;
}

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	
	fdevopen(&put, 0);
}

uint8_t send_data(char data){
	
	
	UDR0 = data;
	while ((UCSR0A & (1 << UDRE0)) == 0) {
		// do
	}
	
	return 0;
}