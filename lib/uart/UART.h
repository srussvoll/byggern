// Created by Sondre Vincent Russvoll on 29.08.16.
// Copyright (c) 2016  All rights reserved.

/*
 * This library defines a UART class.
 * The getInstance is a singleton for either UART0 or UART1.
 *
 * A singleton could have been avoided by using two global instances of UART, one for each UART.
 * However global instances are instantiated at "random" times (sequential in each C++ file, but each C++ file in "random" order).
 * Additionally global variables require at least four extra bytes for addressing compared to the stack (added to the program size).
 * Potentially some SRAM may also be saved by using a singleton compared to a global instance.
 * Reference: Atmel AVR4027: Tips and Tricks to Optimize Your C Code for 8-bit AVR Microcontrollers, Tip #2 (http://www.atmel.com/images/doc8453.pdf)
 * */

#ifndef BYGGERN_UART_H
#define BYGGERN_UART_H

#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#define UART_BUFFER_SIZE 100

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
    static uint8_t printf_UART;

    char TXBuffer[UART_BUFFER_SIZE];
    char RXBuffer[UART_BUFFER_SIZE];

    void sendFromTXBuffer();
    void receiveFromRXBuffer();
    bool RXHasOverflowed = false;
    void (*RXHandler)(char character) = nullptr;

public:
    static UART& getInstance(uint8_t UARTNumber);
    static void enablePrintf(uint8_t UARTNumber);

    void enable(unsigned long baud);
    void disable();
    bool send(char* string);
    bool receive(char* string, size_t length);
    bool chechRXHasOverflowed();
    void setRXHandler(void (* handler)(char character));

    // Use these to implement synchronous sending and waiting for new data:
    bool sending();
    bool dataToBeReceived();

    friend void USART0_UDRE_vect(void);
    friend void USART1_UDRE_vect(void);
    friend void USART0_RXC_vect(void);
    friend void USART1_RXC_vect(void);

    friend int put(char character, FILE* file);
    friend int get(FILE* file);


/*
 * Singleton requires disabling duplicating the object.
 * */
public:
    UART(const UART&) = delete;
    void operator=(const UART&) = delete;
};

#endif //BYGGERN_UART_H