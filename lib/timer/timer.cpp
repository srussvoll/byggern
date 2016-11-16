#ifdef __AVR_ATmega2560__
#include "timer.h"
#include "../lib/utilities/printf.h"

void TIMER1_COMPA_vect() {
    // One second has passed
    Timer &t = Timer::GetInstance();
    t.timer += 1;
}

void Timer::Initialize(uint8_t ms) {
    if(this->timer_number == 0){
        // Set timer 1 in CTC Mode
        TCCR1B |= (1<<WGM12);
    }

    /* How far are we counting?
     * If t = 1 s, f_clock = 16MHz and prescaler=1024
     * C = t/t_c, where t_c = 1/f and f = 1/(f_clock/prescaler)
     * => f = 1/(16*10^6*1/1024) = 6.4*10^(-5)
     * => C = 1 / (6.4*10^(-5)) = 15625
     */
    C = ms * 10^3 *
    OCR1A = 15625;

    // Enable global interrupts
    sei();

    // Enable CTC Compare A interrupt
    TIMSK1 |= (1<<OCIE1A);

}

void Timer::Start(){
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    this->timer = 0;
    // Prescaler at 1024
    TCCR1B |= ((1 << CS10) | (1 << CS12));
}

void Timer::Stop() {
    TCCR1B &= ~((1<<CS10) | (1 << CS11) | (1 << CS12));
}

void Timer::GetFullSecondsPassed(uint16_t &time) {
    time = this->timer;
}
#endif