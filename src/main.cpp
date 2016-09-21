#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "init.h"
#include "lib/uart/uart.h"

#include <stdlib.h>

void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x4800; // Start address for the SRAM
    uint16_t ext_ram_size       = 0x800;
    uint16_t write_errors       = 0;
    uint16_t retrieval_errors   = 0;

    printf("Starting SRAM test...\n");

    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();

    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {

    printf("Test av en litt lengre string som ikke skal overflowe :)\n");

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
        *oled_data = 0x30AA;
        _delay_us(500);
    }

    //printf("\n\n");

    /*char string[] = "Test av en litt lengre string som ikke skal overflowe :)\n";
    UART::GetInstance().Write((uint8_t *) string, sizeof(string));*/

}
#pragma clang diagnostic pop
