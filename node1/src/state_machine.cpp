#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/uart/uart.h"
#include <util/delay.h>
#include <lib/utilities/printf.h>
#include "../lib/menu/menu.h"
#include "../lib/utilities/fonts.h"
#include "../lib/joystick/joystick.h"
#include "lib/scp/scp.h"
#include "lib/scp/commands.h"
#include <stdio.h>
#include "lib/adc/adc.h"

#define STATE_MENU                  1
#define STATE_GAME                  2
#define STATE_HIGHSCORE             3

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};

/* States: enter, loops and leaves */
void InitializeLoop(){
    // Initialize joystick
    Joystick &joystick = Joystick::GetInstance();
    Quantization q(255,0,255,0); //TODO: Enter values for quantization
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC &adc_y = ADC::GetInstance(ADC_ADDRESS2);
    joystick.Init(q,0.5,&adc_x,&adc_y);
    fsm->Transition(STATE_GAME,0);
}

void MenuLoop() {}
void MenuEnter() {}
void MenuLeave() {}

void PlayGameLoop() {

    Joystick &joystick = Joystick::GetInstance();

    // Generate command message

    printf("Sent cmd\n");
    uint8_t x[] = {joystick.XValue(), joystick.YValue()};

    channel->Send(1, CMD_JOYSTICK_VALUES, x, 2);

}
void PlayGameEnter() {
    Joystick &joystick = Joystick::GetInstance();

    // Send start of game command
    channel->Send(0, CMD_GAME_START, nullptr, 0);
}
void PlayGameLeave() {
    Joystick &joystick = Joystick::GetInstance();

    // Send end of game command
    channel->Send(0, CMD_GAME_STOP, nullptr, 0);
}

/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, &InitializeLoop, nullptr},
/* 1. Menu                       */ {&MenuEnter, &MenuLoop, &MenuLeave},
/* 2. Play Game                  */ {PlayGameEnter, &PlayGameLoop, &PlayGameLeave},
/* 3. Highscore Score            */ {nullptr,nullptr, nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}