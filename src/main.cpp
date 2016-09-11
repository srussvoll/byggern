#define F_CPU 4915200
#define FOSC 4915200
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lib/uart/uart.h"
#include "lib/TestStream/TestStream.h"

#include <stdlib.h>
void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
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
            //printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        } else {
          //printf("Worked, %4d\n", i);
        }
      //  _delay_ms(2000);
    }
    // Retrieval phase: Check that no values were changed during or after the writephase
            srand(seed); // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            //printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        } else {
          //printf("Worked, %4d\n", i);
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

ISR(USART0_UDRE0_vect);

int main(void) {
    //USART_Init(MYUBRR);

    char c = 'u';
    //send_data(c);

    MCUCR |= (1 << SRE);
    //EMCUCR |= (1 << SRW10) | (1 << SRW11);
    SRAM_test();




    /*TestStream s = TestStream();
    uint8_t string[5];
    s.Read(string, 5);

    //send_data(string[1]);
    //printf((char*)string);
	//printf("\r\n");
	*/

	while(1)
	{

	}
}
