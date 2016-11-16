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
#include "lib/oled_memory/oled_memory.h"
#include "lib/menu/menu.h"
#include "lib/utilities/new.h"

#define STATE_MENU                  1
#define STATE_GAME                  2
#define STATE_HIGHSCORE             3

namespace {
    StateMachine *fsm;
    SCP *channel;
    SOCKET *sockets[] = {
            &SOCKET::GetInstance(0),
            &SOCKET::GetInstance(1)
    };
    Menu::Controller *controller;
}
/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeFSM(){
    //printf("STATE INITIALIZE ENTERED\n");

    // Initialize Oled
    OLED_memory &memory_oled = OLED_memory::GetInstance();
    memory_oled.Init(128, 64);
    OLED &my_oled = (OLED&) memory_oled;

    my_oled.SetFont(Fonts::f8x8, 8, 8);
    controller = new Menu::Controller(my_oled, 4);
    char item0[] = "Play Games";
    char item1[] = "Settings";
    char item2[] = "Highscore";
    char item3[] = "About";
    char item4[] = "Controllers";
    char item5[] = "Back";

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);

    // Sub-menu
    Menu::Item *i4 = new Menu::Item(item4, sizeof(item4) - 1);
    Menu::Item *i5 = new Menu::Item(item5, sizeof(item5) - 1);

    Menu::Item* main_items[] = {i0, i1, i2, i3};
    Menu::Item* sub_main_items[] = {i4, i5};

    controller->AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller->ControlGoToItem(1);
    controller->AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    fsm->Transition(STATE_GAME, false);
}

/*-----------------------     MENU    -------------------------------*/

void MenuEnter() {
    printf("STATE MENU ENTERED \n");
    controller->Render();
}

void MenuLoop() {
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

    if(y_value < 40){
        controller->SelectNext();
        controller->Render();
        _delay_ms(400);
    }else if (y_value > 220){
        controller->SelectPrevious();
        controller->Render();
        _delay_ms(400);
    }else if(x_value > 170){
        controller->ExecuteItem();
        _delay_ms(400);
    }else if(x_value < 40){
        controller->GoToParent();
        controller->Render();
    }
}

void MenuLeave() {
    printf("STATE MENU LEFT \n");
}

/*----------------------   PLAY GAME  -------------------------------*/

void PlayGameEnter() {
    printf("STATE PLAY GAME ENTERED\n");
    // Send start of game command
    channel->Send(0, CMD_GAME_START, nullptr, 0);
    return;
}

void PlayGameLoop() {
    // Send joystick message
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC &adc_y = ADC::GetInstance(ADC_ADDRESS2);

    // X-Value
    while(!adc_x.request_sample());
    uint8_t x_value;
    while(!adc_x.ReadByte(x_value)){
        ;
    }

    // Y-Value
    while(!adc_y.request_sample());
    uint8_t y_value;
    while(!adc_y.ReadByte(y_value)){
        ;
    }

    bool touchbutton_value;
    touchbutton_value = PINB & (1 << PB0);

    uint8_t x[] = {x_value, y_value, touchbutton_value};
    printf("X: %d, Y:%d, T:%d \n", x_value, y_value, touchbutton_value);
    channel->Send(1, CMD_JOYSTICK_VALUES, x, 3);

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
/* 0. Initialize                 */ {InitializeFSM, nullptr, nullptr},
/* 1. Menu                       */ {MenuEnter,    MenuLoop,       MenuLeave},
/* 2. Play Game                  */ {PlayGameEnter, PlayGameLoop,   PlayGameLeave},
/* 3. Highscore Score            */ {nullptr,            nullptr,         nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Transition(0, true);
    fsm->Start();
}