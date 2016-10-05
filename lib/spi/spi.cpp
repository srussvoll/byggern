#include "spi.h"
#include <avr/interrupt.h>
namespace SPI_N{
    void SPI_STC_vect(){
        // SPI transfer complete
        SPI& spi = SPI::GetInstance();
        uint8_t byte;

        if(spi.ReadByteFromOutputStream(byte)){
            SPDR = byte;
        }else{
            // Turn off interrupts
            SPCR &= ~(1<<SPIE);
            spi.ongoing_transmission = false;
        }
    }

    SPI::SPI(): Stream(1,128){
        sei();
    }

    void SPI::init(PIN **pins, uint8_t number_of_pins, bool clock_polarity_falling = 0, bool clock_phase_trailing = 0) {
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



        // Set SCK = f_osc/128
        SPCR |= (1<<SPR1) | (1<<SPR0);
        SPCR &= ~(1<<SPI2X);

        // Set MSB first
        SPCR &= ~(1<<DORD);

        // Remember that you need to run SetDevice in order to select a device.

        for(int i = 0; i < number_of_pins; ++i){
            // Set direction of pin
            *pins[i]->ddr |= (1<<pins[i]->pin);
            // Set the pin default to high
            *pins[i]->port |= (1<<pins[i]->pin);
        }
    }


    void SPI::InitializeTransmission() {
        if(!this->ongoing_transmission){
            this->ongoing_transmission = true;

            uint8_t byte;
            Stream::ReadByteFromOutputStream(byte);

            SPDR = byte;

            // Enable interrupts
            SPCR |= (1<<SPIE);
        }
    }

    void SPI::ReadAndInsertIntoInputBuffer(){
        // TODO: Implement read in SPI
    }

    void SPI::SetDevice(PIN &pin){
        this->current_pin = pin;
        // Flush output buffer
        this->FlushOutputBuffer();
    }

}
