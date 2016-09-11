#include "uart.h"
#include <stdio.h>

static int put(char character, FILE* file) {
    send_data(character);
    return 0;
}

UART::UART(uint8_t baud_rate): Stream(64,64) {

	UBRR0H = (uint8_t)(baud_rate >> 8);
	UBRR0L = (uint8_t)(baud_rate >> 8);

	// Sets the transmitter and receiver register
	// If only receive set only TXEN0
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

	// Sets format to eight data bits and two stop bits
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(1<<USBS0);

	// To enable printf
	// fdevopen(&put, 0);
}

UART::Write(uint8_t *string, uint16_t size) {
	Stream::Write(string, size);

	// Check if the data register is ready to receive data
	// If it is ready, write data to UDR0
	if((UCSR0A & (1 << UDRE0)) == 1 ) {
		UDR0 = ReadByteFromOutputStream();
	}

}