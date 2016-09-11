#include "lib/uart/uart.h"
#include <stdio.h>
#include <avr/interrupt.h>

void USART0_UDRE_vect(){
	UART uart = UART::GetInstance();
	uint8_t *byte = null;
	ReadByteFromOutputStream(byte)
	if(byte != null) {
		UDR0 = *byte;
	}
}

UART::Initialize(uint16_t baud_rate) {
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

}

UART::UART(): Stream(64,64) {}
