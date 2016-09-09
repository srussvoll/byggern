#include "uart.h"
#include <stdio.h>

static int put(char character, FILE* file) {
	send_data(character);
	return 0;
}

UART::UART(uint16_t baud_rate){

	this.baud_rate_ = baud_rate;
	UBRR0H = (uint8_t)(this.baud_rate_ >> 8);
	UBRR0L = (uint8_t)(this.baud_rate_ >>8);

	// Sets the transmitter and receiver register
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

	// Sets format to eight data bits and two stop bits
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(1<<USBS0);

	// To enable printf
	// fdevopen(&put, 0);


}

UART::Write()

static int put(char character, FILE* file) {
	send_data(character);
	return 0;
}

uint8_t send_data(char data){
	
	
	UDR0 = data;
	while ((UCSR0A & (1 << UDRE0)) == 0) {
		// do
	}
	
	return 0;
}


