#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/uart/uart.h"
#include <util/delay.h>
#include <lib/utilities/printf.h>

void MenuLoop(){
    char msg[] = "Menu loop is so fun\n";
    UART::GetInstance().Write((uint8_t *)msg, sizeof(msg));
    _delay_ms(1000);
}
void MenuEnter(){

}
void MenuLeave(){

}
void (*state_functions[][3])(void) = {
/* 0. Menu                       */ {&MenuEnter, &MenuLoop, &MenuLeave},
/* 1. Play Game                  */ {nullptr,nullptr, nullptr},
/* 2. Highscore Scree            */ {nullptr,nullptr, nullptr},
};



void InitializeStateMachine(){
    StateMachine fsm((void (**)(void)) state_functions, 0);
    fsm.Start();
}