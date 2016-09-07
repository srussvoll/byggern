#define  F_CPU 4915000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lib/uart/uart.h"

int main(void)
{
	USART_Init(MYUBRR);
	
	printf("Heisann! :)\r\n");
	
	while(1)
	{
		
	}
}