#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

class IR_DETECTOR {

private:
    /**
    * A constructor that initializes the UART to a certain size
    */
    IR_DETECTOR();



public:
    /**
    * A Singleton implementation of this class
    */
    static IR_DETECTOR& GetInstance(){
        static IR_DETECTOR instance;
        return instance;
    }
};

