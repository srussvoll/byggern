#pragma once

#include <stdint.h>

namespace StateMachine {
    extern uint8_t current_state;

    typedef void (*transitions[][2])(void);

    bool transition(uint8_t state, transitions transitions, reenter = 0) {
        if (state != current_state || state == current_state && reenter) {
            uint8_t old_state = current_state;
            current_state = state;

            (*transitions[old_state][1])();
            (*transitions[current_state][0])();
            return true;
        } else return false;
    }
}