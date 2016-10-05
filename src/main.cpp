#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lib/utilities/fonts.h"
#include "init.h"
#include "lib/uart/uart.h"
#include "lib/oled/oled.h"

uint16_t write_errors       = 0;
uint16_t retrieval_errors   = 0;
// volatile char *ext_ram = (char *) malloc(0x1D00 * sizeof(char)); // Start address for the SRAMuint8_t

void SRAM_test(uint16_t seed) {
    //volatile char *ext_ram = (char *) malloc(0x0500 * sizeof(char)); // Start address for the SRAMuint8_t
    volatile char *ext_ram = (char *) 0x8000; // Start address for the SRAM
    uint16_t ext_ram_size       = 0x1D00;

    printf("Starting SRAM test...\n");

    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)

    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            //printf("Write phase error: ext_ram[%4x] = %02X (should be %02X)\n", &ext_ram[i], retreived_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            //printf("Retrieval phase error: ext_ram[%4x] = %02X (should be %02X)\n", &ext_ram[i], retreived_value, some_value);
            retrieval_errors++;
        }
    }
    //printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {
    printf("Test av en litt lengre string som ikke skal overflowe :)\n");

    OLED &my_oled = OLED::GetInstance();
    my_oled.Init(128,64);
    my_oled.SetNumberOfLines(8);
    my_oled.SetFont(Fonts::f8x8, 8, 8);
    /*uint8_t pixels[] = {0xAA, 0xAA, 0xAA};
    uint8_t test_array[3][5] = {{0b01111110,0b00001100,0b00110000,0b01111110,0b00000000},{0b00000000,0b00000010,0b01111110,0b00000010,0b00000000}, {0b01111110,0b00001000,0b00001000,0b01111110,0b00000000}};
    uint8_t *dummy[3] = { test_array[0], test_array[1], test_array[2] };
    uint8_t **ptr = dummy;
    my_oled.GoToLine(3);*/

    //my_oled.WriteBitmap(bitmap_character, 8,8,3,3,true);
    char in[] = "Test av en litt lengre string som ikke skal overflowe :)";
    printf("sizeof: %d \n", sizeof(in) - 1);
    my_oled.WriteLine(in, 40, 1, 0);
    my_oled.Repaint();

}
#pragma clang diagnostic pop
