// Created by Sondre Vincent Russvoll on 29.08.16.
// Copyright (c) 2016  All rights reserved.

/*
 * This is a simple test program to blink an LED on Port A Pin 0 at 1 Hz.
 * It also tests UART0 at a baud rate of 9600.
 * */

#include "ex1.h"

// tell the linter main() should never return...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(void) {
    ex1();
}
#pragma clang diagnostic pop