#include <stdio.h>
#include <avr/interrupt.h>

#include "lib/uart/uart.h"

void USART0_UDRE_vect(){
	UART& uart = UART::GetInstance();
	uint8_t byte;
	if(uart.ReadByteFromOutputStream(byte)) {
		UDR0 = byte
	}
}

void UART::Initialize(uint16_t baud_rate) {

	UBRR0H = (uint8_t)(baud_rate >> 8);
	UBRR0L = (uint8_t)(baud_rate >> 8);

	// Sets the transmitter and receiver register
	// If only receive set only TXEN0
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

	// Sets format to eight data bits and two stop bits
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(1<<USBS0);

}

UART::UART(): Stream(64,64) {}

void UART::Write(uint8_t *string, uint16_t size) {
	Stream::Write(string, size);

	// Check if the data register is ready to receive data
	// If it is ready, write data to UDR0
	if((UCSR0A & (1 << UDRE0)) == 1 ) {
		UDR0 = Stream::ReadByteFromOutputStream();
	}
}
