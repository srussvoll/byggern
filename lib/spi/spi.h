#pragma once

#include "../stream/stream.h"
#include <avr/io.h>
namespace SPI_N{
    /**
     * A struct which holds the information about a pin. Used in order to pass pin information
     */
    struct PIN{
        volatile uint8_t *port;
        volatile uint8_t *ddr;
        uint8_t pin;
        PIN(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin): port(port), ddr(ddr), pin(pin){}
        PIN(): port(nullptr), ddr(nullptr), pin(0){}
        PIN operator=(PIN *original_pin){
            PIN newPin(original_pin->port, original_pin->ddr, original_pin->pin);
            return newPin;
        }
    };
    class SPI: Stream{
    public:
        /**
        * A Singleton implementation of this class
        *
        */
        static SPI& GetInstance(){
            static SPI instance;
            return instance;
        }

        /**
         * Because of singleton - makes sure its not copied etc.
         */
        SPI(const SPI&) = delete;

        void SetDevice(PIN &pin);

        /**
        * Because of singleton - makes sure its not copied etc.
        */
        void operator=(const SPI&) = delete;

        void init(PIN **pins, uint8_t number_of_pins, bool clock_polarity_falling, bool clock_phase_trailing);
    private:
        SPI();
        void InitializeTransmission();
        void ReadAndInsertIntoInputBuffer();
        bool ongoing_transmission;
        PIN current_pin;
        friend void SPI_STC_vect();
    };
}
