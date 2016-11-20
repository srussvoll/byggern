#pragma once

#include <avr/io.h>

/**
 * \brief Reads the encoder values from the Sanntidslab motor box.
 */
class Encoder {

public:

    /**
     * Constructor setting up the encoder.
     * This resets the encoder counter.
     */
    Encoder();

    /**
     * Reads the value of the encoder.
     * @return The value.
     */
    int16_t ReadByte();

    /**
     * Resets the encoder counter to 0.
     */
    void Reset();
};