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

    uint8_t *oled_command = (uint8_t*)0x3100;
    uint8_t *oled_data    = (uint8_t*)0x3000;

    // INIT OLED
    *oled_command = 0xae; // display off
    *oled_command = 0xa1; //segment remap
    *oled_command = 0xda; //common pads hardware: alternative
    *oled_command = 0x12;
    *oled_command = 0xc8; //common output scan direction:com63~com0
    *oled_command = 0xa8; //multiplex ration mode:63
    *oled_command = 0x3f;
    *oled_command = 0xd5; //display divide ratio/osc. freq. mode
    *oled_command = 0x80;
    *oled_command = 0x81; //contrast control
    *oled_command = 0x50;
    *oled_command = 0xd9; //set pre-charge period
    *oled_command = 0x21;
    *oled_command = 0x20; //Set Memory Addressing Mode
    *oled_command = 0x02;
    *oled_command = 0xdb; //VCOM deselect level mode
    *oled_command = 0x30;
    *oled_command = 0xad; //master configuration
    *oled_command = 0x00;
    *oled_command = 0xa4; //out follows RAM content
    *oled_command = 0xa6; //set normal display
    *oled_command = 0xaf; // display on

    while(true) {
        *oled_data = 0xAA;
    }

}
#pragma clang diagnostic pop
