#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/uart/uart.h"
#include <util/delay.h>
#include <lib/utilities/printf.h>
#include "../lib/menu/menu.h"
#include "../lib/utilities/fonts.h"
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

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeLoop(){
    printf("STATE INITIALIZE LOOP ENTERED\n");
    // Initialize joystick
    fsm->Transition(STATE_GAME,0);
    return;
}

/*-----------------------     MENU    -------------------------------*/

void MenuEnter() {
    printf("STATE MENU ENTERED\n");
}

void MenuLoop() {
    // TODO: Implement menu
}

void MenuLeave() {
    printf("STATE MENU LEFT \n");
}

/*----------------------   PLAY GAME  -------------------------------*/

void PlaygeGameEnter() {
    printf("STATE PLAY GAME ENTERED\n");
    // Send start of game command
    channel->Send(0, CMD_GAME_START, nullptr, 0);
    return;
}

void PlayGameLoop() {
    // Send joystick message
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC &adc_y = ADC::GetInstance(ADC_ADDRESS2);
    while(!adc_x.request_sample());
    uint8_t x_value;
    while(!adc_x.ReadByte(x_value)){
        ;
    }
    while(!adc_y.request_sample());
    uint8_t y_value;
    while(!adc_y.ReadByte(y_value)){
        ;
    }

    uint8_t x[] = {x_value, y_value};
    //printf("X: %d, Y:%d \n", x_value, y_value);
    channel->Send(1, CMD_JOYSTICK_VALUES, x, 2);
    _delay_ms(100);
    // Check for end of game command
    uint8_t command;
    uint8_t length;
    uint8_t data[2];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_GAME_STOP) {
            printf("Game ended with %d seconds\n", data[0]);
            //TODO: Implement check highscore|
            fsm->Transition(STATE_MENU, 0);
            return;
        }
    }

}

void PlayGameLeave() {
    printf("STATE PLAY GAME LEFT\n");
    return;
}


/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr,            &InitializeLoop, nullptr},
/* 1. Menu                       */ {&MenuEnter,    &MenuLoop,       &MenuLeave},
/* 2. Play Game                  */ {PlaygeGameEnter, &PlayGameLoop,   &PlayGameLeave},
/* 3. Highscore Score            */ {nullptr,            nullptr,         nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}