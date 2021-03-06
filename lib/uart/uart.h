#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../stream/stream.h"

ISR(USART0_UDRE_vect);

/**
 * \brief An interface for communicating through UART
 */
class UART : public Stream {

private:

    /**
     * A constructor that initializes the UART to a certain size
     */
    UART();

    /**
     * A bool to indicate whether or not a transmission is going on
     */
    bool ongoing_transmission = false;

    /**
     * Starts the transmission. Just a void-void function to modulize
     */
    inline void StartTransmission();

    /**
     * The interrupt handler vector. To be run on each DRE interrupt
     */
    friend void USART0_UDRE_vect();

public:

    /**
     * A Singleton implementation of this class
     */
    static UART& GetInstance(){
        static UART instance;
        return instance;
    }

    /**
     * Write the inserted string to output (i.e. write to computer)
     * @param string The "data string" that shall be written to the output
     * @param size the size of the data string
     */
    void Write(uint8_t *string, uint16_t size);

    /**
     * Initializer because of the singleton implementation.
     * @param baud_rate The baud rate of the uart
     */
    void Init(uint16_t baud_rate);

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    UART(const UART&) = delete;

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    void operator=(const UART&) = delete;

};
