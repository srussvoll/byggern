#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER4_COMPA_vect);
ISR(TIMER3_COMPA_vect);

/**
 * \brief A simple 16 bit timer controller for AVR
 *
 * Simple 16 bit timer controller for AVR that counts how many times the counter has triggered,
 * and can call a callback function each time the timer triggers.
 *
 * Provides two timers, timer 3 and timer 4. When using GetInstance, the parameter pass determines which
 * timer it returns.
 *
 * 0 -> timer 4
 * 1 -> timer 5
 *
 * Uses CTC mode. Assumes a 16MHz clock.
 */
class Timer{

private:
    /**
     * Initializer. Not used by end user
     * @param timer_number Which timer number you want to get. Please see GetInstance()
     */
    Timer(uint8_t timer_number): timer_number(timer_number){};

    /**
     * Number of times the timer has triggered
     */
    volatile uint16_t timer;

    /**
     * Which timer is this instance for. See GetInstance()
     */
    uint8_t timer_number;

    /**
     * Callback function to be called when the timer triggers.
     */
    void (*fn)(void);


public:
    /**
    * A Singleton implementation of this class
    * @param timer_number Which timer you want to use. timer_number = 0 -> timer4, timer_number = 1 -> timer5
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
     * @param ms How many milliseconds before the timer triggers
     * @param fn The callback function to be called when the timer triggers. Pass nullptr if you do not want a callback function
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
     * Returns how many times the timer has triggered
     * @param time Variable to contain the data
     */
    void GetNumberOfTimesTriggered(uint16_t &time);

    /**
     * Interrupt handler for timer 4
     */
    friend void TIMER4_COMPA_vect();

    /**
     * Interrupt handler for timer 3
     */
    friend void TIMER3_COMPA_vect();
};