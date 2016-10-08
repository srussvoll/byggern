#include "spi.h"
#include <stdio.h>
#include <util/delay.h>
namespace SPI_N{
    void SPI_STC_vect(){
        printf("INT\n");
        // SPI transfer complete
        SPI& spi = SPI::GetInstance();

        // Put data into the buffer
        if(!spi.throw_away_data){
            spi.ReadAndInsertIntoInputBuffer();
            spi.throw_away_data = false;
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
                printf("NO MORE\n");
                spi.ongoing_transmission = false;

                // Turn of SS pin. Active low
                *spi.current_pin.port |= (1<<spi.current_pin.pin);
            }
        }

    }

    SPI::SPI(): Stream(128,128){
        sei();
    }

    void SPI::Initialize(PIN **pins, uint8_t number_of_pins, bool clock_polarity_falling = 0, bool clock_phase_trailing = 0) {
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
        SPSR &= ~(1<<SPI2X);

        // Set MSB first
        SPCR &= ~(1<<DORD);

        // Enable interrupts
        SPCR |= (1<<SPIE);

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
            printf("initi trans\n");
            // Set SS line. Active low
            *this->current_pin.port &= ~(1<<current_pin.pin);

            this->ongoing_transmission = true;

            uint8_t byte;
            Stream::ReadByteFromOutputStream(byte);

            SPDR = byte;
            printf("BYTE IS = %2x",byte);
        }
    }

    void SPI::ReadAndInsertIntoInputBuffer(){
        uint8_t byte = SPDR;
        this->WriteByteToInputStream(byte);
    }

    void SPI::SetDevice(PIN &pin){
        this->current_pin = pin;
        // Flush output and input buffer
        this->FlushOutputBuffer();
        this->FlushInputBuffer();
    }

    void SPI::Write(uint8_t *string, uint16_t size) {
        Stream::Write(string, size);

        // Initialize Transmission
        this->InitializeTransmission();
    }

    void SPI::ResetSSPin() {
        if(*this->current_pin.port & (1<<current_pin.pin)){
            // Pin is high, turn off them on again
            *this->current_pin.port &= ~(1<<current_pin.pin);
            _delay_us(100); // Wait 100 us for good measure
            *this->current_pin.port |= (1<<current_pin.pin);
        }else{
            // Pin is low, turn on then off again
            *this->current_pin.port |= (1<<current_pin.pin);
            _delay_us(100); // Wait 100 us for good measure
            *this->current_pin.port &= ~(1<<current_pin.pin);
        }
    }

    void SPI::WriteByte(uint8_t byte, bool wait) {
        Stream::WriteByte(byte);
        if(!wait){
            this->InitializeTransmission();
        }
    }

    void SPI::WriteByteAndThrowAwayData(uint8_t byte, bool wait) {
        Stream::WriteByte(byte);
        this->throw_away_data = true;
        if(!wait){
            this->InitializeTransmission();
        }
    }

    bool SPI::ReadByte(uint8_t &data) {
        Stream::ReadByte(data);
    }
}
