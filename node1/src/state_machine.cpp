#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/uart/uart.h"
#include <util/delay.h>
#include <lib/utilities/printf.h>
#include "../lib/menu/menu.h"
#include "../lib/utilities/fonts.h"
#include "../lib/joystick/joystick.h"

#define STATE_MENU                  1
#define STATE_GAME                  2
#define STATE_HIGHSCORE             3

StateMachine *fsmP;
void InitializeLoop(){
    fsmP->Transition(STATE_MENU,0);
}
void MenuLoop(){
}
void MenuEnter(){
}
void MenuLeave(){
}

void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, nullptr, nullptr},
/* 1. Menu                       */ {&MenuEnter, &MenuLoop, &MenuLeave},
/* 2. Play Game                  */ {nullptr,nullptr, nullptr},
/* 3. Highscore Scree            */ {nullptr,nullptr, nullptr},
};



void InitializeStateMachine(){
    StateMachine fsm((void (**)(void)) state_functions, 0);
    fsmP = &fsm;
    fsm.Start();
}