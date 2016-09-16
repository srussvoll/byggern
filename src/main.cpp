#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lib/utilities/memory.h"
#include "lib/utilities/printf.h"
#include "lib/uart/uart.h"
#include "lib/adc/adc.h"


void init_hardware() __attribute__((naked)) __attribute__((section(".init8")));

void init_hardware(){
    Utilities::initialize_memory();
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::enable_printf(uart);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {

    printf("\n\n");
    //printf("Test av en litt lengre string som mest sannsynlig overflower..\n");
    ADC &myAdc = ADC::GetInstance(0x1405, 2);

	while(1) {
        myAdc.request_sample();
        while(myAdc.GetAvailableReadBytes()==0){
            //printf("wait\n");
        }

        uint8_t string;
        myAdc.ReadByte(string);
        printf("%d\n", string);

        _delay_ms(50);

	}
}
#pragma clang diagnostic pop
