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

    TestStream s = TestStream();
    uint8_t string[5];
    s.Read(string, 5);

    send_data(string[1]);
    //printf((char*)string);
	//printf("\r\n");
	
	while(1)
	{
		
	}
}