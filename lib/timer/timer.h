#ifdef __AVR_ATmega2560__
#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
/**
 * Simple 16 bit timer controller for AVR. Uses CTC mode. Assumes a 16MHz clock, and only counts whole seconds passed.
 * If you are not using a 16MHz clock, you need to change the OCR1A register to match your clock. Please refer to the
 * ATMEGA2560 datasheet, chapter 17
 */
ISR(TIMER1_COMPA_vect);
class Timer{
public:
    /**
    * A Singleton implementation of this class
    *
    */
    static Timer& GetInstance(){
        static Timer instance;
        return instance;
    }

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    Timer(const Timer&) = delete;

    void Initialize();

    void Start();

    void Stop();

    void GetFullSecondsPassed(uint16_t &time);

    friend void TIMER1_COMPA_vect();


private:
    Timer(){};

    volatile uint16_t timer;

};
#endif