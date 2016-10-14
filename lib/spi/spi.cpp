#include "spi.h"
#include <stdio.h>
#include <util/delay.h>
namespace SPI_N{
    void SPI_STC_vect(){
        // SPI transfer complete
        SPI& spi = SPI::GetInstance();

        // Check master mode
        if (!(SPCR & (1<<MSTR))){
            printf("MSTR DIS? \n");
        }

        // Put data into the buffer
        if(spi.throw_away_data_count == 0){
            uint8_t byte = SPDR;
            spi.WriteByteToInputStream(byte);

        }else{
            spi.throw_away_data_count -= 1;
        }

        if(SPSR & (1<<WCOL)){
            // SPDR written to during a data transfer.
            // TODO: We might have to handle this. Should not be needed if used correctly
            printf("!!SPI ERR:!! Please see SPI_STC_vect() implementation. Now sleeping for 1000ms so I'm sure you saw this \n");
            _delay_ms(1000);
        }

        if(spi.ongoing_transmission){
            // Sending data. More to send?
            uint8_t byte;
            if(spi.ReadByteFromOutputStream(byte)){
                SPDR = byte;
            }else{
                spi.ongoing_transmission = false;

                // Turn of SS pin. Active low
                *spi.current_pin.port |= (1<<spi.current_pin.pin);
                // Disable interrupts
                SPCR &= ~(1<<SPIE);

            }
        }

    }

    SPI::SPI(): Stream(128,128){
        sei();
    }

    void SPI::Initialize(PIN *pins, uint8_t number_of_pins, bool clock_polarity_falling = 0, bool clock_phase_trailing = 0) {
        printf("INIT!\n");
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
        DDRB |= (1<<DDB5); // PB5 / MOSI to output
        DDRB |= (1<<DDB7); // PB7 / SCK to to output
        DDRB &= ~(1<<DDB6); // MISO to input

        DDRB |= (1<<DDB4); // Set



        // Set SCK = f_osc/128
        SPCR |= (1<<SPR1) | (1<<SPR0);
        SPSR &= ~(1<<SPI2X);

        // Set MSB first
        SPCR &= ~(1<<DORD);

        // Set registers
        SPCR |= (1<<MSTR); // Set master mode
        SPCR |= (1<<SPE); // Enable SPI

        // Remember that you need to run SetDevice in order to select a device.

        for(int i = 0; i < number_of_pins; ++i){
            // Set direction of pin
            *pins[i].ddr |= (1<<pins[i].pin);
            // Set the pin default to high
            *pins[i].port |= (1<<pins[i].pin);
        }
    }


    void SPI::InitializeTransmission() {
        if(!this->ongoing_transmission){
            // Set SS line. Active low
            *this->current_pin.port &= ~(1<<current_pin.pin);

            this->ongoing_transmission = true;

            uint8_t byte;
            Stream::ReadByteFromOutputStream(byte);
            SPDR = byte;
            // Enable interrupts
            SPCR |= (1<<SPIE);
        }
    }

    void SPI::SetDevice(PIN &pin){
        this->current_pin = pin;
        // Flush output and input buffer
        this->FlushOutputBuffer();
        this->FlushInputBuffer();
    }



    void SPI::WriteByte(uint8_t byte, bool wait) {
        while(this->ongoing_transmission){}
        Stream::WriteByte(byte);
        if(!wait){
            this->InitializeTransmission();
        }
    }

    void SPI::WriteByteAndThrowAwayData(uint8_t byte, bool wait) {
        while(this->ongoing_transmission){}
        Stream::WriteByte(byte);
        this->throw_away_data_count += 1;
        if(!wait){
            this->InitializeTransmission();
        }
    }


}
