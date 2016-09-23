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

<<<<<<< HEAD
    printf("\n\n");

//    OLED &my_oled = OLED::GetInstance();
=======
    OLED &my_oled = OLED::GetInstance();
>>>>>>> baba90094c26b26935b22d7443fdb359a3ee8a8d

    my_oled.GoToLine(0);
    my_oled.ClearLine();

}
#pragma clang diagnostic pop
