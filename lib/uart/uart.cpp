#include <stdio.h>
#include <avr/interrupt.h>

#include "lib/uart/uart.h"

void USART0_UDRE_vect(){
	UART& uart = UART::GetInstance();
	uint8_t byte;
	
	if (uart.ReadByteFromOutputStream(byte)) {
		UDR0 = byte;
	} else {
		UCSR0B &= ~(1 << UDRIE0);
	}
}

void UART::Init(uint16_t baud_rate) {

	UBRR0H = (uint8_t)(baud_rate >> 8);
	UBRR0L = (uint8_t)(baud_rate);

	// Sets the transmitter and receiver register
	// If only receive set only TXEN0
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);


	// Sets format to eight data bits and two stop bits
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(1<<USBS0);

}

UART::UART(): Stream(64,64) {
	sei();
}

void UART::Write(uint8_t *string, uint16_t size) {
	bool stream_empty = Stream::GetOutputBufferLength();
	Stream::Write(string, size);

	if (stream_empty) {
		// Initialize transmission
		uint8_t byte;
		Stream::ReadByteFromOutputStream(byte);
		
		// Enable interrupts
		UCSR0B |= (1 << UDRIE0);

		// Write data 
		UDR0 = byte;
	}
}
