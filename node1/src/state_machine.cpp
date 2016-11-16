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
#include "lib/highscore/highscore.h"
#include "lib/adc/adc.h"
#include "lib/oled_memory/oled_memory.h"
#include "lib/menu/menu.h"
#include "lib/utilities/new.h"

#define STATE_MENU                  1
#define STATE_GAME                  2
#define STATE_HIGHSCORE             3

#define NUMBER_OF_HIGHSCORES        6

namespace {
    StateMachine *fsm;
    SCP *channel;
    SOCKET *sockets[] = {
            &SOCKET::GetInstance(0),
            &SOCKET::GetInstance(1)
    };
    Menu::Controller *controller;
    Highscore::Highscore *highscore;
}

void TransitionToGame(){
    fsm->Transition(STATE_GAME, false);
}

void TransitionToHighscore() {
    fsm->Transition(STATE_HIGHSCORE, false);
}

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeFSM(){
    //printf("STATE INITIALIZE ENTERED\n");

    // Initialize Oled
    OLED_memory &memory_oled = OLED_memory::GetInstance();
    memory_oled.Init(128, 64);
    OLED &my_oled = (OLED&) memory_oled;

    my_oled.SetFont(Fonts::f8x8, 8, 8);

    // Initialize menu
    controller = new Menu::Controller(my_oled, 4);
    char item0[] = "Play Game";
    char item1[] = "Settings";
    char item2[] = "Highscore";
    char item3[] = "About";
    char item1_0[] = "Controllers";

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);

    // Sub-menu
    Menu::Item *i1_0 = new Menu::Item(item1_0, sizeof(item1_0) - 1);

    Menu::Item* main_items[] = {i0, i1, i2, i3};
    Menu::Item* sub_main_items[] = {i1_0};

    controller->AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller->ControlGoToItem(1);
    controller->AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    controller->ControlGoToItem(0);

    i0->AddAction(TransitionToGame);
    i2->AddAction(TransitionToHighscore);

    // Initialize highscores
    highscore = new Highscore::Highscore;

    fsm->Transition(STATE_MENU, false);
}

/*-----------------------     MENU    -------------------------------*/

void MenuEnter() {
    //printf("STATE MENU ENTERED \n");
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
    //printf("STATE MENU LEFT \n");
}

/*----------------------   PLAY GAME  -------------------------------*/

void PlayGameEnter() {
    //printf("STATE PLAY GAME ENTERED\n");
    // Send start of game command
    channel->Send(0, CMD_GAME_START, nullptr, 0);
    return;
}

void PlayGameLoop() {
    // Send joystick message
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC &adc_y = ADC::GetInstance(ADC_ADDRESS2);
    ADC &adc_p = ADC::GetInstance(ADC_ADDRESS3);
    ADC &adc_a = ADC::GetInstance(ADC_ADDRESS4);

    // X-Value
    while(!adc_x.request_sample());
    uint8_t x_value;
    while(!adc_x.ReadByte(x_value));

    // Y-Value
    while(!adc_y.request_sample());
    uint8_t y_value;
    while(!adc_y.ReadByte(y_value));

    // Touch slider position
    while(!adc_p.request_sample());
    uint8_t p_value;
    while(!adc_p.ReadByte(p_value));

    // Touch slider angle
    while(!adc_a.request_sample());
    uint8_t a_value;
    while(!adc_a.ReadByte(a_value));

    // Touch button
    bool touchbutton_value = (bool) (PINB & (1 << PB0));

    uint8_t x[] = {x_value, y_value, p_value, a_value, (uint8_t) touchbutton_value};
    //printf("X: %d, Y:%d, T:%d \n", x_value, y_value, touchbutton_value);
    channel->Send(1, CMD_JOYSTICK_VALUES, x, sizeof(x));

    // Check for end of game command
    uint8_t command;
    uint8_t length;
    uint8_t data[2];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_GAME_STOP) {
            //printf("Game ended with %d seconds\n", data[0]);
            //TODO: Implement check highscore|
            fsm->Transition(STATE_MENU, 0);
            return;
        }
    }

    _delay_ms(50);

}

void PlayGameLeave() {
    //printf("STATE PLAY GAME LEFT\n");
    return;
}

void HighscoreEnter() {
    Highscore::Score **scores;
    uint8_t score_length = highscore->GetHighscores(0, scores);
    if (score_length > NUMBER_OF_HIGHSCORES) score_length = NUMBER_OF_HIGHSCORES;
    OLED &oled = OLED_memory::GetInstance();
    oled.SetNumberOfLines(NUMBER_OF_HIGHSCORES + 1);
    oled.Clear();

    char title[] = "Highscores";
    oled.WriteLine(title, sizeof(title) - 1, 0, 0);

    for (uint8_t i = 0; i < score_length; ++i) {
        oled.WriteLine(scores[i]->name, scores[i]->name_length - 1, i + 1, 0);
        //printf("\nScore: ");
        //UART::GetInstance().Write((uint8_t *) scores[i]->name, scores[i]->name_length - 1);
        char score[6];
        itoa(scores[i]->score, score, 10);
        uint8_t length = 1;
        while(true) {
            if (score[length] == '\0') break;
            length += 1;
        }
        oled.WriteLine(score, length, i + 1, oled.GetMaxLineCharacters() - 1 - 4);
    }

    oled.Repaint();
}

void HighscoreLoop() {
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    while(!adc_x.request_sample());
    uint8_t x_value;
    while(!adc_x.ReadByte(x_value));
    if(x_value < 40) fsm->Transition(STATE_MENU, false);
}


/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {InitializeFSM, nullptr, nullptr},
/* 1. Menu                       */ {MenuEnter,    MenuLoop,       MenuLeave},
/* 2. Play Game                  */ {PlayGameEnter, PlayGameLoop,   PlayGameLeave},
/* 3. Highscore Score            */ {HighscoreEnter,            HighscoreLoop,         nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    /*Highscore::Highscore hs;
    char name1[] = "Sondre";
    char name2[] = "V";
    char name3[] = "Johan";
    char name4[] = "Baugen";

    Highscore::Score score1(4357, name1, sizeof(name1));
    Highscore::Score score2(10352, name2, sizeof(name2));
    Highscore::Score score3(11233, name3, sizeof(name3));
    Highscore::Score score4(1337, name4, sizeof(name4));

    // Store new scores if you want. Currently the highscore is loaded from the EEPROM.
    hs.ClearHighscores();
    hs.SaveScore(score1);
    hs.SaveScore(score2);
    hs.SaveScore(score3);
    hs.SaveScore(score4);
    hs.StoreScores();

    return;*/

    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Transition(0, true);
    fsm->Start();
}