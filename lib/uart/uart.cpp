#include "uart.h"
#include <stdio.h>
#include <avr/interrupt.h>

static int put(char character, FILE* file) {
	send_data(character);
	return 0;
}

<<<<<<< HEAD
void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
=======
void USART0_UDRE_vect(){
	UART uart = UART::GetInstance();
	uint8_t *byte = null;
	ReadByteFromOutputStream(byte)
	if(byte != null) {
		UDR0 = *byte;
	}
}

UART::Initialize(uint16_t baud_rate) {
>>>>>>> 13769612f71067b3f81387e650ae5060270b6c33

	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

<<<<<<< HEAD
	fdevopen(&put, 0);
=======
	// To enable printf
	fdevopen(&put, 0);	

}

UART::UART(): Stream(64,64) {
>>>>>>> 13769612f71067b3f81387e650ae5060270b6c33
}

uint8_t send_data(char data){


	UDR0 = data;
	while ((UCSR0A & (1 << UDRE0)) == 0) {
		// do
	}

<<<<<<< HEAD
	return 0;
=======
>>>>>>> 13769612f71067b3f81387e650ae5060270b6c33
}
