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

void SRAM_test(uint16_t seed)
{
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
    //printf("Test av en litt lengre string som ikke skal overflowe :)\n");
    printf("Test av en litt lengre string som ikke skal overflowe :)\n");
    //
     OLED &my_oled = OLED::GetInstance();
    //
     my_oled.Init(128,64);
     my_oled.SetNumberOfLines(1);
     uint8_t pixels[] = {0xAA, 0xAA, 0xAA};
     uint8_t test_array[3][5] = {{0b01111110,0b00001100,0b00110000,0b01111110,0b00000000},{0b00000000,0b00000010,0b01111110,0b00000010,0b00000000}, {0b01111110,0b00001000,0b00001000,0b01111110,0b00000000}};
     uint8_t *dummy[3] = { test_array[0], test_array[1], test_array[2] };
     uint8_t **ptr = dummy;

     for(int i = 121; true; i++){
         my_oled.WriteBitmap(ptr,5,24,i % 128,16);
         my_oled.Repaint();
         _delay_ms(2000);
         my_oled.Clear();
     }

    // my_oled.Repaint();
    // my_oled.Clear();
     //my_oled.Repaint();
    //
    // /*
    // for(int j = 0; j<7; j++){
    //     my_oled.GoToLine(j);
    //     my_oled.ClearLine();
    //     _delay_ms(1000);
    // }
    // */
    //
    // //my_oled.WriteByte(3,4,0xAA);
    //
    // uint8_t testFont = pgm_read_byte(&(f5x7[0][1]));
    //
    // //printf("verideliksom %d\n", testFont[5]);
    //
    // uint8_t test_array[1][5] = {{0b01111110,0b01001010,0b01001010,0b00110100,0b00000000}};
    // uint8_t *dummy[1] = { test_array[0] };
    // uint8_t **ptr = dummy;
    //
    // // my_oled.WriteByte(0,0,0b01111110);
    // // my_oled.WriteByte(0,1,0b01001010);
    // // my_oled.WriteByte(0,2,0b01001010);
    // // my_oled.WriteByte(0,3,0b00110100);
    // // my_oled.WriteByte(0,4,0b00000000);
    //
    // my_oled.WriteBitmap(0, 0, (uint8_t**) ptr, 1, 5);
    // my_oled.WriteBitmap(0, 8, (uint8_t**) ptr, 1, 5);


    //my_oled.WriteByte(5,2,0xFF);

    //uint8_t aa[4] = {0b01111000,0b00010100,0b01111000,0b00000000};
    //my_oled.WriteByteArray(0,0,aa,4);

     //for (int i = 0; i < 10; ++i) {
    //     SRAM_test(i);
     //}

//    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
    // //SRAM_test(2);
        // volatile uint8_t *p1 = (uint8_t *)0x8f00;
        // volatile uint8_t *p2 = (uint8_t *)0x8fff;
        // uint8_t dummy;
        // while(1){
        //     //*p1 = 0xAA;
        //     *p2 = 0x10;
        //     dummy = *p2;
        //     printf("%4X - %2X\n",p2, dummy);
        // }
}
#pragma clang diagnostic pop
