#pragma once

#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>
namespace SPI_N{
    /**
     * \brief A struct which holds the information about a pin. Used in order to pass pin information
     */
    struct PIN{
        /**
         * A pointer to the port register for the pin
         */
        volatile uint8_t *port;
        /**
         * A pointer to the data direction register for the pin
         */
        volatile uint8_t *ddr;
        /**
         * The pin number
         */
        uint8_t pin;
        PIN(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin): port(port), ddr(ddr), pin(pin){}
        PIN(): port(nullptr), ddr(nullptr), pin(0){}
        PIN operator=(PIN *original_pin){
            PIN newPin(original_pin->port, original_pin->ddr, original_pin->pin);
            return newPin;
        }
    };

    ISR(SPI_STC_vect);
    /**
     * \brief An SPI driver which implements the AVR SPI interface
     *
     * An SPI driver which implements the AVR SPI interface. Uses the STREAM class as both input and output buffer.
     * Supports all four modes of SPI (please see the Initialize function). Uses interrupt for when a transfer is complete
     */
    class SPI: public Stream {
    public:
        /**
        * A Singleton implementation of this class
        *
        */
        static SPI& GetInstance() {
            static SPI instance;
            return instance;
        }

        /**
         * Because of singleton - makes sure its not copied etc.
         */
        SPI(const SPI&) = delete;

        /**
        * Because of singleton - makes sure its not copied etc.
        */
        void operator=(const SPI&) = delete;

        /**
        * Sets the device the SPI driver is going to use. This must be called in order for it to function.
        * The device passed into set device must also have been initialized in the Initialize function when
        * initializing the SPI driver
        * @param pin The PIN the SPI is going to use
        */
        void SetDevice(PIN pin);

        /**
         * Initializes the SPI.
         * @param pins An array with PIN structs, where each struct is a pin that is used by the SPI
         * @param number_of_pins Number of PIN structs in the pins array
         * @param clock_polarity_falling If enabled, SCK is high when idle. Defaults to disabled (SCK low when idle)
         * @param clock_phase_trailing If enabled, samples on the trailing edge of SCK: Defaults to disabled (sample on the leading edge)
         */
        void Initialize(PIN *pins, uint8_t number_of_pins, bool clock_polarity_falling, bool clock_phase_trailing);

        /**
         * Writes a byte to the output stream. Puts the returned data into the input stream
         * @param byte The byte to be sent
         * @param wait If set, the SPI driver will not start the transmission of the output buffer
         */
        void WriteByte(uint8_t byte, bool wait);

        /**
         * Writes a byte to the output stream. Throws away the returned data
         * @param byte The byte to be sent
         * @param wait If set, the SPI driver will not start the transmission of the output buffer
         */
        void WriteByteAndThrowAwayData(uint8_t byte, bool wait);

        /**
         * A struct of the type PIN which indicates which pin is currently selected.
         */
        PIN current_pin;

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
         * A bool indicating wether or not an outgoing transmission is ongoing
         */
        volatile bool ongoing_transmission = false;

        /**
         * A flag indicating how many transfer we are going to not care about the data received
         */
        uint8_t throw_away_data_count = 0;

        /**
         * The interrupt vector for when a transmission is complete.
         */
        friend void SPI_STC_vect();
    };
}
