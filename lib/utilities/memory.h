#pragma once
#include <avr/io.h>
namespace Utilities{
    /**
    * Sets the required bits for the external memory to work as intended.
    */
    void initialize_memory(){
        // Enable external memory
        MCUCR |= (1 << SRE);
        // Define our sectors
        EMCUCR |= (2 << SRL0);
        // Enable wait state for sector 1. Wait one cycle during read/write strobe
        EMCUCR |= (1 << SRW01);
    }
}
