#pragma once

#include <stdint.h>

class StateMachine {
private:
    uint8_t current_state;
    void (*(*transitions))();

public:
    StateMachine(void (*(*transitions))(void), uint8_t initial_state = 0) : transitions((void (**)(void)) transitions), current_state(initial_state) {};
    bool Transition(uint8_t state, bool reenter);
    uint8_t GetCurrentState();
};