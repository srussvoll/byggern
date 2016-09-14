#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>

#include "lib/uart/uart.h"

int put(char c, FILE* f) {
    UART& uart = UART::GetInstance();
    uart.Write((uint8_t *) &c, 1);
    while (uart.GetOutputBufferLength() != 0) {}
    return 0;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    fdevopen(&put, NULL);

    char string[] = "Test av en litt lengre string som mest sannsynlig overflower..\n";

	while(1) {
        while(uart.GetOutputBufferLength() != 0) {}
        uart.Write((uint8_t *) string, sizeof(string));

        //printf("Hei.\n");
	}
}
#pragma clang diagnostic pop
