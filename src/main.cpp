#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lib/uart/uart.h"
#include "lib/TestStream/TestStream.h"

int main(void) {
	USART_Init(MYUBRR);

    testStream s();

	printf("Heisann! :)\r\n");
	
	while(1)
	{
		
	}
}