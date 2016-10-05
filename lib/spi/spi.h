#pragma once

#include "../stream/stream.h"
#include <avr/io.h>

class SPI: Stream{
public:
    /**
    * A Singleton implementation of this class
    *
    */
    static SPI& GetInstance(){
        static SPI instance;
        return instance;
    }



    /**
     * Because of singleton - makes sure its not copied etc.
     */
    SPI(const SPI&) = delete;

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    void operator=(const SPI&) = delete;
private:
    SPI();
    void init(bool clock_polarity_falling, bool clock_phase_trailing);
    void InitializeTransmission();
    bool ongoing_transmission;
    friend void SPI_STC_vect();
};