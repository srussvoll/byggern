// Created by Sondre Vincent Russvoll on 29.08.16.
// Copyright (c) 2016  All rights reserved.

#include "UART.h"
#include <stdio.h>

static int put(char character, FILE* file);
static int get(FILE* file);

UART& UART::getInstance(int UARTNumber) {
    // Return lazy initialized instance of the correct UART.
    if (UARTNumber == 0) {
        static struct UARTRegisters registers = {
            .UDR   = &UDR0,
            .UCSRA = &UCSR0A,
            .UCSRB = &UCSR0B,
            .UCSRC = &UCSR0C,
            .UBRRL = &UBRR0L,
            .UBRRH = &UBRR0H
        };

        static UART instance0(registers);
        return instance0;
    } else {
        static struct UARTRegisters registers = {
            .UDR   = &UDR1,
            .UCSRA = &UCSR1A,
            .UCSRB = &UCSR1B,
            .UCSRC = &UCSR1C,
            .UBRRL = &UBRR1L,
            .UBRRH = &UBRR1H
        };

        static UART instance1(registers);
        return instance1;
    }
}


void UART::enable(unsigned long baud) {
    // Set baud
    baud = FOSC / 16 / baud - 1;
    *this->registers.UBRRH = (unsigned char) (baud >> 8);
    *this->registers.UBRRL = (unsigned char) (baud);

    // Set frame format: 8 data, 2 stop bits
    *this->registers.UCSRC |= (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    // Enable TX and RX and RX interrupt
    *this->registers.UCSRB |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Enable interrupts globally
    sei();
}

void UART::disable() {
    // Disable TX, RX, TX interrupt and RX interrupt
    *this->registers.UCSRB &= ~((1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0));
}

bool UART::send(char* string) {
    size_t length = strlen(string);
    size_t TXLength = strlen(this->TXBuffer);
    // Check that the buffer is large enough. Remember that strings have \0 in addition to the contents.
    if (TXLength + length + 1 <= UART_BUFFER_SIZE) {
        // If the TXBuffer is empty, start a new transmission of the buffer.
        bool initiateTX = TXLength == 0;

        // Add the string to the TXBuffer
        strcat(this->TXBuffer, string);

        if (initiateTX) {
            // Turn on the interrupt for empty TX buffer register and send the first data.
            *this->registers.UCSRB |= (1 << UDRIE0);
            this->sendFromTXBuffer();
        }

        return true;
    } else return false;
}

void UART::sendFromTXBuffer() {
    char toSend = this->TXBuffer[0];
    size_t length = strlen(this->TXBuffer);

    // Pop the first character:
    memmove(this->TXBuffer, this->TXBuffer + 1, length);

    // If the TXBuffer is empty, disable the interrupt.
    if (length - 1 == 0) {
        *this->registers.UCSRB &= ~(1 << UDRIE0);
    }

    // Send the byte:
    *this->registers.UDR = (uint8_t)toSend;
}

bool UART::receive(char* string, size_t length) {
    size_t RXLength = strlen(this->RXBuffer);
    strlcpy(string, this->RXBuffer, length);
    // If the receiver string got all of the RXBuffer, empty buffer:
    if (length > RXLength) {
        this->RXBuffer[0] = '\0';
    } else {
        memmove(this->RXBuffer, this->RXBuffer + length - 1, (RXLength + 1) - (length - 1));
    }

    return RXLength > length;
}

void UART::receiveFromRXBuffer() {
    if (this->RXHandler != nullptr) {
        (*this->RXHandler)((char) *this->registers.UDR);
    }
    // Check that the buffer is large enough. Remember that strings have \0 in addition to the contents.
    else if (strlen(this->RXBuffer) + 1 + 1 <= UART_BUFFER_SIZE) {
        // Add the character to the RXBuffer
        char character = (char) *this->registers.UDR;
        strncat(this->RXBuffer, &character, 1);
    } else {
        this->RXHasOverflowed = true;
    }
}

bool UART::chechRXHasOverflowed() {
    if (this->RXHasOverflowed) {
        this->RXHasOverflowed = false;
        return true;
    } else return false;
}

void UART::setRXHandler(void (*handler)(char character)) {
    this->RXHandler = handler;
}

bool UART::sending() {
    return this->TXBuffer[0] != '\0';
}

void UART::enablePrintf() {
    fdevopen(&put, &get);
}

bool UART::dataToBeReceived() {
    return this->RXBuffer[0] != '\0';
}

void USART0_UDRE_vect(void) {
    UART::getInstance(0).sendFromTXBuffer();
}

void USART1_UDRE_vect(void) {
    UART::getInstance(1).sendFromTXBuffer();
}

void USART0_RXC_vect(void) {
    UART::getInstance(0).receiveFromRXBuffer();
}

void USART1_RXC_vect(void) {
    UART::getInstance(1).receiveFromRXBuffer();
}


static int put(char character, FILE* file) {
    char string[2];
    string[0] = character;
    string[1] = '\0';
    return !UART::getInstance(0).send(string);
}

static int get(FILE* file) {
    char character[2];
    UART::getInstance(0).receive(character, 2);
    if (character[0] == '\0') return _FDEV_EOF;
    else return (int) character[0];
}