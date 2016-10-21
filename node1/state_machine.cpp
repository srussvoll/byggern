#include "state_machine.h"

namespace StateMachine {
    uint8_t current_state = 0;

    void (*transitions[][2])(void) = {
        /*                   enter, leave                   */
        /* state1        */ {}
    };

    bool transition(uint8_t state, bool reenter = 0) {
        if (state != current_state || state == current_state && reenter) {
            uint8_t old_state = current_state;
            current_state = state;

            (*transitions[old_state][1])();
            (*transitions[current_state][0])();
            return true;
        } else return false;
    }
}