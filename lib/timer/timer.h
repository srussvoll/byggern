#ifdef __AVR_ATmega2560__
#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
/**
 * Simple 16 bit timer controller for AVR. Uses CTC mode. Assumes a 16MHz clock, and only counts whole seconds passed.
 * If you are not using a 16MHz clock, you need to change the OCR1A register to match your clock. Please refer to the
 * ATMEGA2560 datasheet, chapter 17
 */
ISR(TIMER4_COMPA_vect);
ISR(TIMER3_COMPA_vect);
class Timer{
public:
    /**
    * A Singleton implementation of this class
    *
    */
    static Timer& GetInstance(uint8_t timer_number){
        if(timer_number == 0){
            static Timer instance(0);
            return instance;
        }else{
            static Timer instance(1);
            return instance;
        }
    }

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    Timer(const Timer&) = delete;

    /**
     * Initializes the timer. You must run this function in order for the timer to work
     */
    void Initialize(uint16_t ms, void(*fn)(void));

    /**
     * Wipes all previous data and starts the timer.
     */
    void Start();

    /**
     * Stops the timer but retains the data
     */
    void Stop();

    /**
     * Sets time to the number of full seconds passed since Start() was called.
     * @param time
     */
    void GetFullSecondsPassed(uint16_t &time);

    /**
     * Interrupt handler for when one second has passed
     */
    friend void TIMER4_COMPA_vect();

    friend void TIMER3_COMPA_vect();


private:
    /**
     * Initializer. Not used due to singleton
     */
    Timer(uint8_t timer_number): timer_number(timer_number){};

    /**
     * Number of full seconds passed
     */
    volatile uint16_t timer;

    uint8_t timer_number;

    void (*fn)(void);

};
#endif