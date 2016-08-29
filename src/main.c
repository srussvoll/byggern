// Created by Sondre Vincent Russvoll on 29.08.16.
// Copyright (c) 2016  All rights reserved.

/*
 * This is a simple test program to blink an LED on Port A Pin 0 at 1 Hz.
 * */

#include <avr/io.h>
#include <util/delay.h>

// Pin and frequency:
#define PORT PORTA
#define DDR  DDRA
#define PIN  PA0
#define FREQ 1

// tell the linter main() should never return...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {
    // Set pin to output and turn it on.
    DDR  |= (1 << PIN);
    PORT |= (1 << PIN);

    // Toggle LED
    while(1) {
        _delay_ms(500 / FREQ);
        PORT ^= (1 << PIN);
        _delay_ms(500 / FREQ);
        PORT ^= (1 << PIN);
    }
}
#pragma clang diagnostic pop