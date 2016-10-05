#include "spi.h"
void SPI::init(bool clock_polarity_falling = 0, bool clock_phase_trailing = 0) {
    // Set registers
    SPCR |= (1<<SPE); // Enable SPI
    SPCR |= (1<<MSTR); // Set master mode


    if(clock_polarity_falling){
        SPCR |= (1<<CPOL);
    }else{
        SPCR &= ~(1<<CPOL);
    }
    if(clock_phase_trailing){
        SPCR |= (1<<CPHA);
    }else{
        SPCR &= ~(1<<CPHA);
    }

    // Set MOSI and SCK to output and all others to input
    DDRB |= (1<<DDB5); // PB5 to input
    DDRB |= (1<<DDB7); // PB7 to input

    DDRB &= ~(1<<DDB6); // MISO to output
    DDRB &= ~(1<<DDB4); // SS to output

    // Enable interrupt. Make sure global interrupts are enabled
    SPCR |= (1<<SPIE);

    // Set SCK = f_osc/128
    SPCR |= (1<<SPR1) | (1<<SPR0);
    SPCR &= ~(1<<SPI2X);

    // Set MSB first
    SPCR &= ~(1<<DORD);
}

void write_byte(uint8_t byte){
    SPDR = byte;
    while(!(SPSR & (1<<SPIF))){}
}

SPI::SPI(): Stream(1,1){

}