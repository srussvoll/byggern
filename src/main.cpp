#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/uart/uart.h"


int main(void) {
    UART& uart = UART::GetInstance();
    uart.Init(9600);

    char string[] = "Naa synes jeg Johan bruker for mye stroem...\n";
    uart.Write((uint8_t *) string, sizeof(string));

	while(1)
	{

	}
}
