#pragma once

#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>
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

    ISR(SPI_STC_vect);

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
        /**
         * Initializes the ISP.
         * @param pins An array with PIN structs, where each struct is a pin that is used by the SPI
         * @param number_of_pins Number of PIN structs in the pins array
         * @param clock_polarity_falling If enabled, SCK is high when idle. Defaults to disabled (SCK low when idle)
         * @param clock_phase_trailing If enabled, samples on the trailing edge of SCK: Defaults to disabled (sample on the leading edge)
         */
        void init(PIN **pins, uint8_t number_of_pins, bool clock_polarity_falling, bool clock_phase_trailing);
    private:
        /**
         * Initializer for SPI. Not used because of singleton
         */
        SPI();
        /**
         * Send the data in the output buffer onto the SPI.
         */
        void InitializeTransmission();
        /**
         * Reads the data from the SPI and puts it inot the input buffer
         */
        void ReadAndInsertIntoInputBuffer();
        /**
         * A bool indicating wether or not an outgoing transmission is ongoing
         */
        bool ongoing_transmission;
        /**
         * A struct of the type PIN which indicates which pin is currently selected.
         */
        PIN current_pin;
        /**
         * The interrupt vector for when a transmission is complete. According to the data sheet this is both incoming and outgoing
         */
        friend void SPI_STC_vect();
    };
}
