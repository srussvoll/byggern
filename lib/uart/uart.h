#ifndef UART_H_
#define UART_H_

#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface for handling streams with default methods.
 */
class UART: Stream{

public:

    /**
    * A constructor that initializes the UART and sets the baud_rate 
    * @param baud_rate The baud rate of the uart. Bitshifts in
    */
    UART(uint8_t baud_rate);

    /**
    * Write the inserted string to output (i.e. write to computer)
    * @param string The "data string" that shall be written to the output
    * @param size the size of the data string
    */
    void Write(uint8_t *string, uint16_t size);


private:

    /**
     * A 64 byte output stream. Everything that's sent from the microcontroller is first stored here.
     */
    uint8_t output_stream_uart[64];

    /**
     * A 64 byte input stream. Everything that's recieved from the client to the microcontroller is stored here.
     */
    uint8_t input_stream_uart[64];

    

};


#endif