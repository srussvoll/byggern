#pragma once

#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø, Sondre Russvoll
 * @version 1.0
 *
 * An SPI driver
 */
namespace SPI_N{
    /**
     * A struct which holds the information about a pin. Used in order to pass pin information
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
     * This class is an SPI driver which uses the
     */
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

        /**
         * Write the inserted string to output (i.e. write to SPI selected by SetDevice)
         * @param string The "data string" that shall be written to the output
         * @param size the size of the data string
        */
        void Write(uint8_t *string, uint16_t size);

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
         * Reads a byte from the input stream
         * @param data Byte to insert the byte into
         */
        bool ReadByte(uint8_t &data);

        /**
         * Resets the SS pin. That is, if the SS pin is high, put it low and then high again.
         * If it is low, put it high and then low again
         */
        void ResetSSPin();
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
         * A flag indicating whether or not to keep the next recieved byte
         */
        bool throw_away_data;

        /**
         * The interrupt vector for when a transmission is complete.
         */
        friend void SPI_STC_vect();
    };
}
