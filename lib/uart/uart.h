#ifndef UART_H_
#define UART_H_

#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include "lib/stream/stream.h"

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface for handling streams with default methods.
 */

ISR(USART0_UDRE_vect);

class UART: Stream{

public:

    /**
    * A Singleton implementation of this class
    *
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
    void Initialize(uint8_t baud_rate);

    friend void USART0_UDRE_vect();

private:

    /**
    * A constructor that initializes the UART to a certain size
    */
    UART();

    /**
     * A 64 byte output stream. Everything that's sent from the microcontroller is first stored here.
     */
    uint8_t output_stream[64];

    /**
     * A 64 byte input stream. Everything that's recieved from the client to the microcontroller is stored here.
     */
    uint8_t input_stream[64];

};

#endif
