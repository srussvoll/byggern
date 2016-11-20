#include "state_machine.h"

#include <util/delay.h>
#include <stdio.h>

#include "lib/state_machine/state_machine.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/menu/menu.h"
#include "lib/utilities/fonts.h"
#include "lib/scp/scp.h"
#include "lib/scp/commands.h"
#include "lib/highscore/highscore.h"
#include "lib/adc/adc.h"
#include "lib/oled_memory/oled_memory.h"
#include "lib/menu/menu.h"
#include "lib/utilities/new.h"

#define STATE_MENU                   1
#define STATE_GAME                   2
#define STATE_HIGHSCORE              3
#define STATE_WAITING_FOR_HIGHSCORE  4
#define STATE_RENDER_OLED_FROM_NODE2 5

#define NUMBER_OF_HIGHSCORES        6

namespace {
    StateMachine *fsm;
    SCP *channel;
    Socket *sockets[] = {
            &Socket::GetInstance(0),
            &Socket::GetInstance(1)
    };
    Menu::Controller *controller;
    Highscore::Highscore *highscore;

    uint8_t OLEDNumber = 0;
}

void TransitionToGame(){
    fsm->Transition(STATE_GAME, false);
}

void TransitionToHighscore() {
    fsm->Transition(STATE_HIGHSCORE, false);
}

void ClearHighscores() {
    highscore->ClearHighscores();
    highscore->StoreScores();
}

void RequestNode2Render() {
    channel->Send(0, CMD_RENDER_OLED, nullptr, 0);
    fsm->Transition(STATE_RENDER_OLED_FROM_NODE2, false);
}

void SendUART() {
    char number_string[] = "No.    \n";
    itoa(OLEDNumber, &number_string[3], 10);

    UART::GetInstance().Write((uint8_t *) number_string, sizeof(number_string) - 1);
    OLEDNumber += 1;
}

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeFSM(){
    // Initialize Oled
    OLED_memory &memory_oled = OLED_memory::GetInstance();
    memory_oled.Init(128, 64);
    OLED &my_oled = (OLED&) memory_oled;

    my_oled.SetFont(Fonts::f8x8, 8, 8);

    // Initialize menu
    controller = new Menu::Controller(my_oled, 4);
    char item0[] = "Play";
    char item1[] = "Highscore";
    char item2[] = "OLEDtest";
    char item3[] = "UARTtest";
    char item1_0[] = "Show";
    char item1_1[] = "Clear";

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);

    // Sub-menu
    Menu::Item *i1_0 = new Menu::Item(item1_0, sizeof(item1_0) - 1);
    Menu::Item *i1_1 = new Menu::Item(item1_1, sizeof(item1_1) - 1);

    Menu::Item* main_items[] = {i0, i1, i2, i3};
    Menu::Item* sub_main_items[] = {i1_0, i1_1};

    controller->AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller->ControlGoToItem(1);
    controller->AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    controller->ControlGoToItem(0);

    i0->AddAction(TransitionToGame);
    i2->AddAction(RequestNode2Render);
    i3->AddAction(SendUART);
    i1_0->AddAction(TransitionToHighscore);
    i1_1->AddAction(ClearHighscores);

    // Initialize highscores
    highscore = new Highscore::Highscore;

    fsm->Transition(STATE_MENU, false);
}

/*-----------------------     MENU    -------------------------------*/

void MenuEnter() {
    controller->Render();
    _delay_ms(400);
}

void MenuLoop() {
    ADC &adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC &adc_y = ADC::GetInstance(ADC_ADDRESS2);
    while(!adc_x.request_sample());
    uint8_t x_value;
    while(!adc_x.ReadByte(x_value));
    while(!adc_y.request_sample());
    uint8_t y_value;
    while(!adc_y.ReadByte(y_value));

    if(y_value < 40){
        controller->SelectNext();
    } else if (y_value > 220){
        controller->SelectPrevious();
    } else if(x_value > 170){
        controller->ExecuteItem();
    } else if(x_value < 40){
        controller->GoToParent();
    } else {
        return;
    }
    _delay_ms(400);
}

void RenderOLEDFromNode2Leave() {
    OLED_memory &memory_oled = OLED_memory::GetInstance();
    memory_oled.Init(128, 64);
}

void RenderOLEDFromNode2Loop(){
    uint8_t command;
    uint8_t length;
    uint8_t data[6];

    if(channel->Receive(command, data, length)) {
        if (command == WRITE_TO_ADDRESS) {
            volatile uint8_t *address = (volatile uint8_t*)(((uint16_t)data[0] << 8) + data[1]);
            for (int i = 2; i < length; ++i) {
                *address = data[i];
            }
        } else if (command == CMD_GOTO_MAIN_MENU) {
            fsm->Transition(STATE_MENU, false);
        }
    }
}

/*----------------------   PLAY GAME  -------------------------------*/

void PlayGameEnter() {
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
    channel->Send(1, CMD_JOYSTICK_VALUES, x, sizeof(x));

    // Check for end of game command
    uint8_t command;
    uint8_t length;
    if(channel->Receive(command, nullptr, length)) {
        if (command == CMD_WAIT_FOR_HIGHSCORE) {
            //printf("Game ended with %d seconds\n", data[0]);
            //TODO: Implement check highscore
            fsm->Transition(STATE_WAITING_FOR_HIGHSCORE, 0);
            return;
        }
    }

    _delay_ms(50);

}

/*----------------------   HIGHSCORE  -------------------------------*/


void HighscoreEnter() {
    Highscore::Score **scores;
    uint8_t score_length = highscore->GetHighscores(0, scores);
    if (score_length > NUMBER_OF_HIGHSCORES) score_length = NUMBER_OF_HIGHSCORES;
    OLED &oled = OLED_memory::GetInstance();
    oled.SetNumberOfLines(NUMBER_OF_HIGHSCORES + 1);

    char title[] = "Highscore";
    oled.WriteLine(title, sizeof(title) - 1, 0, 0);

    for (uint8_t i = 0; i < score_length; ++i) {
        oled.WriteLine(scores[i]->name, scores[i]->name_length, i + 1, 0);
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
    if (x_value < 40) fsm->Transition(STATE_MENU, false);
}

/*----------------------   WaitForHighscore  -------------------------------*/
void WaitForHighscoreLoop(){
    uint8_t command;
    uint8_t length;
    uint8_t data[13];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_SAVE_HIGHSCORE) {
            OLED &oled = OLED_memory::GetInstance();
            oled.SetNumberOfLines(3);
            char message1[] = "Saving...";
            oled.WriteLine(message1, sizeof(message1) - 1, 1, 3);
            oled.Repaint();

            Highscore::Score score((data[0] << 8) | data[1], (char *) &data[3], data[2]);

            highscore->SaveScore(score);
            highscore->StoreScores();

            fsm->Transition(STATE_MENU, 0);
            return;
        }
    }
}

void WaitForHighscoreEnter() {
    OLED &oled = OLED_memory::GetInstance();
    oled.SetNumberOfLines(4);
    char message1[] = "Please enter";
    char message2[] = "your name.";
    oled.WriteLine(message1, sizeof(message1) - 1, 1, 2);
    oled.WriteLine(message2, sizeof(message2) - 1, 2, 2);
    oled.Repaint();
}

/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {InitializeFSM, nullptr, nullptr},
/* 1. Menu                       */ {MenuEnter,    MenuLoop,       nullptr},
/* 2. Play Game                  */ {PlayGameEnter, PlayGameLoop,   nullptr},
/* 3. Highscore Score            */ {HighscoreEnter,            HighscoreLoop,         nullptr},
/* 4. Waiting for Highscore Data */ {WaitForHighscoreEnter, WaitForHighscoreLoop, nullptr},
                                    {nullptr, RenderOLEDFromNode2Loop, RenderOLEDFromNode2Leave},
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