#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lib/utilities/fonts.h"
#include "init.h"
#include "lib/uart/uart.h"
#include "lib/oled/oled.h"
#include "lib/menu/menu.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/mcp2515/mcp2515_regisers.h"
#include "lib/spi/spi.h"

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

void SPITest();

int main(void) {
    SPITest();
}

void SPITest(){
    _delay_ms(100);
    SPI_N::SPI &my_spi = SPI_N::SPI::GetInstance();

    SPI_N::PIN ss = SPI_N::PIN(&PORTD, &DDRD, 5);
    SPI_N::PIN ss_a[] = {ss};

    my_spi.Initialize(ss_a, 1, 0,0);
    my_spi.SetDevice(ss);

    MCP2515 &my_mcp = MCP2515::GetInstance();
    my_mcp.Initialize(&my_spi);

    printf("start\n");

    my_mcp.SetNormal();
    while(1){
        uint8_t byte;
        my_mcp.ReadStatus(byte);
        _delay_ms(100);
        printf("BYTE = %2x \n", byte);
        break;
    }

};

#pragma clang diagnostic pop
