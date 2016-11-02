#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/scp/scp.h"
#include "../lib/socket/socket.h"
#include "../lib/scp/commands.h"
#include "../lib/motor/motor.h"
#include "../lib/dac/dac.h"
#include <util/delay.h>

#define STATE_ONGOING        1;
#define STATE_IDLE           2;

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};

/* States: enter, loops and leaves */
void InitializeLoop() {
    printf("correct node\n");
    //TODO: Add initialization to everything for node 2.

    DAC& dac = DAC::GetInstance();
    DDRH |= (1 << DDH1);
    while(true){
        PORTH |= (1 << PORTH1);
        _delay_ms(2000);
        dac.WriteAnalogSignalPercentage(0.3);
        PORTH &= ~(1 << PORTH1);
        _delay_ms(2000);
        dac.WriteAnalogSignalPercentage(0.3);
    }


}

void OngoingLoop() {
    uint8_t command;
    uint8_t length;
    uint8_t data[2];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_JOYSTICK_VALUES) {
            // TODO: Do things with x or y values. Need motor and stuff
        }
    }
}

/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, &InitializeLoop, nullptr},
/* 1. ONGOING                       */ {nullptr, &OngoingLoop, nullptr},
/* 2. IDLE                    */ {nullptr, nullptr, nullptr}
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}