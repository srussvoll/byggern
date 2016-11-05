#include "spi_sync.h"
#include <stdio.h>
#include <util/delay.h>
#include "pins.h"
namespace SPI_N_SYNC{
    void SPI_SYNC::Initialize(PIN *pins, uint8_t number_of_pins, bool clock_polarity_falling = 0, bool clock_phase_trailing = 0) {
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
        MOSI_DDR |= (1<<MOSI_PIN); // PB5 / MOSI to output
        SCK_DDR |= (1<<SCK_PIN); // PB7 / SCK to to output
        MISO_DDR &= ~(1<<MISO_PIN); // MISO to input
        SS_DDR |= (1<<SS_PIN); // Set SS high

        SPSR |= (1<<SPI2X);
        SPCR &= ~(1<<SPR0);
        SPCR &= ~(1<<SPR1);


        // Set MSB first
        SPCR &= ~(1<<DORD);

        // Set registers
        SPCR |= (1<<MSTR); // Set master mode
        SPCR |= (1<<SPE); // Enable SPI

        // Remember that you need to run SetDevice in order to select a device.
        for(uint8_t i = 0; i < number_of_pins; ++i){
            // Set direction of pin
            *pins[i].ddr |= (1<<pins[i].pin);
            // Set the pin default to high
            *pins[i].port |= (1<<pins[i].pin);
        }
    }


    void SPI_SYNC::InitializeTransmission() {
        // Reset the input buffer
        this->input_buffer_index = 0;
        this->input_buffer_number_of_times_read = 0;

        // Set SS line. Active low
        *this->current_pin.port &= ~(1<<current_pin.pin);
        SPDR = this->output_buffer[0];
        // For all the bytes we are going to send
        for(int i = 1; i < this->output_buffer_index; i++) {
            // Wait for the transmission to complete
            while(!(SPSR & (1<<SPIF))) {
                // Put data into the buffer
                if(this->throw_away_data_count == 0) {

                    this->input_buffer[this->input_buffer_index] = SPDR;

                    this->input_buffer_index += 1;

                } else {
                    this->throw_away_data_count -= 1;
                }

                // Send more data
                SPDR = this->output_buffer[i];
            }
        }

        // Set SS line high again
        *this->current_pin.port |= (1<<this->current_pin.pin);

        // Reset the output buffer
        this->output_buffer_index = 0;
    }

    void SPI_SYNC::SetDevice(PIN &pin){
        this->current_pin = pin;

        // Flush output and input buffer
        this->input_buffer_index = 0;
        this->output_buffer_index = 0;
    }

    void SPI_SYNC::WriteByte(uint8_t byte, bool wait) {
        this->output_buffer[this->output_buffer_index] = byte;
        this->output_buffer_index += 1;
        if(!wait){
            this->InitializeTransmission();
        }
    }

    void SPI_SYNC::WriteByteAndThrowAwayData(uint8_t byte, bool wait) {
        this->output_buffer[this->output_buffer_index] = byte;
        this->output_buffer_index += 1;
        this->throw_away_data_count += 1;
        if(!wait){
            this->InitializeTransmission();
        }
    }

    void SPI_SYNC::ReadByte(uint8_t &byte) {
        byte = this->input_buffer[this->input_buffer_number_of_times_read];
        this->input_buffer_number_of_times_read += 1;
    }
}
