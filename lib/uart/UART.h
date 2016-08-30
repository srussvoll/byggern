// Created by Sondre Vincent Russvoll on 29.08.16.
// Copyright (c) 2016  All rights reserved.

/*
 * This library defines a UART class.
 * The getInstance is a singleton for either UART0 or UART1.
 * */

#ifndef BYGGERN_UART_H
#define BYGGERN_UART_H

#include <avr/interrupt.h>
#include <string.h>

#define UART_BUFFER_SIZE 200

// Define the USART Data Register Empty and RX Complete ISRs
ISR(USART0_UDRE_vect);
ISR(USART1_UDRE_vect);
ISR(USART0_RXC_vect);
ISR(USART1_RXC_vect);

struct UARTRegisters {
    volatile uint8_t* UDR;
    volatile uint8_t* UCSRA;
    volatile uint8_t* UCSRB;
    volatile uint8_t* UCSRC;
    volatile uint8_t* UBRRL;
    volatile uint8_t* UBRRH;
};

class UART {
private:
    UART(UARTRegisters registers) : registers(registers) {
        this->TXBuffer[0] = '\0';
        this->RXBuffer[0] = '\0';
    };
    UARTRegisters registers;

    char TXBuffer[UART_BUFFER_SIZE];
    char RXBuffer[UART_BUFFER_SIZE];

    void sendFromTXBuffer();
    void receiveFromRXBuffer();
    bool RXHasOverflowed = false;
    void (*RXHandler)(char character) = nullptr;

public:
    static UART& getInstance(int UARTNumber);

    void enable(unsigned long baud);
    void disable();
    bool send(char* string);
    bool receive(char* string, size_t length);
    bool chechRXHasOverflowed();
    void setRXHandler(void (* handler)(char character));

    friend void USART0_UDRE_vect(void);
    friend void USART1_UDRE_vect(void);
    friend void USART0_RXC_vect(void);
    friend void USART1_RXC_vect(void);


/*
 * Singleton requires disabling duplicating the object.
 * */
public:
    UART(const UART&) = delete;
    //void operator=(const UART&) = delete;
};

#endif //BYGGERN_UART_H