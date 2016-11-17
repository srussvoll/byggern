#ifdef __AVR_ATmega2560__

#include <math.h>
#include "timer.h"
#include "../lib/utilities/printf.h"

void TIMER4_COMPA_vect() {
    // One second has passed
    Timer &t = Timer::GetInstance(0);
    t.timer += 1;
    if(t.fn != nullptr){
        (*(t.fn))();
    }
}

void TIMER3_COMPA_vect() {
    // One second has passed
    Timer &t = Timer::GetInstance(1);
    t.timer += 1;
    if(t.fn != nullptr){
        (*(t.fn))();
    }
}

void Timer::Initialize(uint16_t ms, void(*fn)(void)) {
    if(this->timer_number == 0){
        // Set timer 1 in CTC Mode
        TCCR4B |= (1<<WGM42);
    }else if(this->timer_number == 1){
        TCCR3B |= (1<<WGM32);
    }

    /* How far are we counting?
     * If t = 1 s, f_clock = 16MHz and prescaler=1024
     * C = t/t_c, where t_c = 1/f and f = 1/(f_clock/prescaler)
     * => f = 1/(16*10^6*1/1024) = 6.4*10^(-5)
     * => C = 1 / (6.4*10^(-5)) = 15625
     */
    uint16_t C = (uint16_t) (ms * 0.001 *  (16 * 1000000) / (1024));
    //printf("\nC = %d, MS = %d\n", C, ms);
    if(this->timer_number == 0){
        OCR4A = C;
    }else if(this->timer_number == 1){
        OCR3A = C;
    }


    // Enable global interrupts
    sei();

    // Enable CTC Compare A interrupt
    if(this->timer_number == 0){
        TIMSK4 |= (1<<OCIE4A);
    }else if(this->timer_number == 1){
        TIMSK3 |= (1<<OCIE3A);
    }

    this->fn = fn;

}

void Timer::Start(){
    if(this->timer_number == 0){
        TCNT4H = 0x00;
        TCNT4L = 0x00;
        this->timer = 0;
        // Prescaler at 1024
        TCCR4B |= ((1 << CS40) | (1 << CS42));
    }else if(this->timer_number == 1){
        TCNT3H = 0x00;
        TCNT3L = 0x00;
        this->timer = 0;
        // Prescaler at 1024
        TCCR3B |= ((1 << CS30) | (1 << CS32));
    }
}

void Timer::Stop() {
    if(this->timer_number == 0){
        TCCR4B &= ~((1<<CS40) | (1 << CS41) | (1 << CS42));
    }else if(this->timer_number == 1){
        TCCR3B &= ~((1<<CS30) | (1 << CS31) | (1 << CS32));
    }
}

void Timer::GetFullSecondsPassed(uint16_t &time) {
    time = this->timer;
}

#endif