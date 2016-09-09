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
     * Initializes the whol
     */
    UART(uint16_t baud_rate);

private:

    /**
     * Stores the baud rate for the uart inferface
     */
    uint16_t baud_rate_;




};


void USART_Init( unsigned int ubrr );

uint8_t send_data(char data);


#endif