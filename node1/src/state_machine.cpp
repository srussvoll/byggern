#include "../lib/state_machine/state_machine.h"

namespace StateMachine {
    uint8_t current_state = 0;

    void (*transitions[][2])(void) = {
            /*                   enter, leave                   */
            /* state1        */ {}
    };
}