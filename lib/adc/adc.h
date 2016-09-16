#pragma once

#include "../stream/stream.h"

class ADC : public Stream {
private:
    ADC();

public:


    /**
     * A Singleton implementation of this class
     *
     */
    static ADC& GetInstance() {
        static ADC instance;
        return instance;
    }

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    ADC(const ADC&) = delete;

    /**
    * Beacause of singleton - makes sure its not copied etc.
    */
    void operator=(const ADC&) = delete;
};
