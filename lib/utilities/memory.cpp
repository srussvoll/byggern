#ifdef __AVR_ATmega162__

#include "memory.h"

#include <avr/io.h>

namespace Utilities {
    void InitializeMemory() {
        // Enable external memory
        MCUCR |= (1 << SRE);
        // Define our sectors
        EMCUCR |= (2 << SRL0);
        // Enable wait state for sector 1. Wait one cycle during read/write strobe
        EMCUCR |= (1 << SRW01);

        // Disable A15
        SFIOR |= (1 << XMM0);
    }
}

#endif