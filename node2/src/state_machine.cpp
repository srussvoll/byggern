#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/scp/scp.h"
#include "../lib/socket/socket.h"
#include "../lib/scp/commands.h"
#include "../lib/motor/motor.h"
#include "../lib/dac/dac.h"
#include <util/delay.h>
#include "../lib/ir_detector/ir_detector.h"
#include "../lib/timer/timer.h"
#include "lib/joystick/joystick.h"
#include "lib/solenoid/solenoid.h"
#include "lib/encoder/encoder.h"
#include <math.h>
#include <lib/servo/servo.h>
#include <lib/oled_scp/oled_scp.h>
#include <lib/utilities/fonts.h>
#include <lib/menu/menu.h>
#include "lib/spi/spi.h"
#include "lib/highscore/highscore.h"

#define STATE_ONGOING        1
#define STATE_IDLE           2
#define STATE_HIGHSCORE      3
#define STATE_RENDER_OLED    4

namespace {
    StateMachine *fsm;
    SCP          *channel;
    SOCKET* sockets[] = {
            &SOCKET::GetInstance(0),
            &SOCKET::GetInstance(1)
    };

    Encoder encoder;
    Servo *servo;

    uint8_t x_direction = 0;
    uint8_t y_direction = 0;
    uint8_t slider_pos  = 127;
    uint8_t slider_ang  = 127;
    bool touchbutton = false;
    bool touchbutton_last = false;

    float K_p = 1.2;
    float T_i = 100;
    float K_d = 0.07;
    float T   = 0.005;
    double e_integral = 0;
    double y_previous = 0;


    char highscore_name[10];
    uint8_t highscore_name_length = 0;
    uint16_t highscore_score = 0;
    SPI_N::PIN oldsspin;

    bool game_started = false;
}

void SetMotorPID();

void StartGame() {
    Timer &timer1 = Timer::GetInstance(0);
    timer1.Stop();
    timer1.Initialize(100, nullptr);
    timer1.Start();

    Solenoid &solenoid = Solenoid::GetInstance();
    solenoid.Pulse();

    game_started = true;
}

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeLoop() {
    //TODO: Add initialization to everything for node 2.
    // Initialize the motor
    Motor &motor = Motor::GetInstance();
    motor.Initialize();

    // Initialize the IR Sensor
    IR_Detector::GetInstance().Initialize(0x05);

    // Initialize the joystick
    // TODO: Correct Quantization levels
    Joystick &joy = Joystick::GetInstance();
    Quantization q;
    q.x_max = 0;
    q.x_min = 0;
    q.y_max = 0;
    q.y_min = 0;
    joy.Initialize(q, 0);

    // Initialize the joystick
    Joystick& joystick = Joystick::GetInstance();
    Quantization levels;
    levels.x_min = 0;
    levels.x_max = 255;
    levels.y_min = 0;
    levels.y_max = 255;
    joystick.Initialize(levels, 0.6);

    //Initialize the Solenoid
    Solenoid & solenoid = Solenoid::GetInstance();
    solenoid.Initialize();

    // Initialize controller timer
    Timer &timer2 = Timer::GetInstance(1);
    timer2.Initialize((uint16_t) (T * 1000), SetMotorPID);

    // Initialize servo
    servo = new Servo(900, 2100);


    fsm->Transition(STATE_IDLE, 0);
}

/*-----------------------   ONGOING  -------------------------------*/

void OngoingEnter() {
    printf("Ongoing enter \n");
    Motor &motor = Motor::GetInstance();
    motor.Start();
    x_direction = 0;
    y_direction = 0;
    touchbutton = false;
    encoder.Reset();
    Timer &timer2 = Timer::GetInstance(1);
    timer2.Start();

    // Start timer for game start
    Timer &timer1 = Timer::GetInstance(0);
    timer1.Initialize(10000, StartGame);
    timer1.Start();
}

void OngoingLoop() {
    // Get joystick values
    uint8_t command;
    uint8_t length;
    uint8_t data[4];

    Joystick& joystick = Joystick::GetInstance();
    Motor& motor = Motor::GetInstance();

    if(channel->Receive(command, data, length)) {
        if (command == CMD_JOYSTICK_VALUES) {
            x_direction = data[0];
            y_direction = data[1];
            slider_pos  = data[2];
            slider_ang  = data[3];
            touchbutton = (bool) data[4];
            joystick.Update(x_direction, y_direction);
        }
    }

    if(game_started && touchbutton){
        if(!touchbutton_last){
            Solenoid & solenoid = Solenoid::GetInstance();
            solenoid.Pulse();
            touchbutton_last = true;
        }
    }else{
        touchbutton_last = false;
    }

    servo->SetAngle(slider_ang - 127);

    // Check fail state
    IR_Detector &ir = IR_Detector::GetInstance();
    if(game_started && ir.Sample()) {
        fsm->Transition(STATE_HIGHSCORE, 0);
        return;
    }
}

void SetMotorPID() {
    if (fsm->current_state != STATE_ONGOING) return;

    Motor& motor = Motor::GetInstance();

    float r = ((float)slider_pos - 127) / 100;
    double y = (double)encoder.ReadByte() / 4500;
    double e = r - y;
    e_integral += e;

    float u = K_p * e + K_p * T / T_i * e_integral + K_p * K_d / T * (y_previous - y);
    y_previous = y;

    if (u > 0) motor.GoRight();
    else       motor.GoLeft();
    u = fabs(u);
    motor.Drive(u);
}

void OngoingLeave() {
    Motor &motor = Motor::GetInstance();
    motor.Stop();

    Timer &timer1 = Timer::GetInstance(0);
    Timer &timer2 = Timer::GetInstance(1);
    timer1.Stop();
    timer2.Stop();
    timer1.GetFullSecondsPassed(highscore_score);
    channel->Send(0, CMD_WAIT_FOR_HIGHSCORE, nullptr, 0);
    _delay_ms(500);
    sockets[0]->FlushInputBuffer();
    sockets[1]->FlushInputBuffer();
}

/*-----------------------   IDLE  -------------------------------*/

void IdleEnter(){
    printf("IDLE STATE ENTERED \n\r");
}

void IdleLoop() {
    uint8_t command;
    uint8_t length;
    uint8_t data[1];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_GAME_START) {
            fsm->Transition(STATE_ONGOING, 0);
            return;
        } else if (command == CMD_RENDER_OLED) {
            fsm->Transition(STATE_RENDER_OLED, false);
        }
    }
}

void RenderOLEDToNode1() {
    OLED_SCP &scp_oled = OLED_SCP::GetInstance();
    scp_oled.Init(128, 64, *channel, WRITE_TO_ADDRESS);
    OLED &my_oled = (OLED&) scp_oled;

    my_oled.SetFont(Fonts::f8x8, 8, 8);

    Menu::Controller controller(my_oled, 5);

    char item0[] = "Menylinje 0";
    char item1[] = "Menylinje 1";
    char item2[] = "Menylinje 2";
    char item3[] = "Menylinje 3";
    char item4[] = "Menylinje 4";
    char item5[] = "Menylinje 5";
    char item6[] = "Menylinje 6";
    char item7[] = "Menylinje 7";
    char item8[] = "Submeny 1";
    char item9[] = "Submeny 2";

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);
    Menu::Item *i4 = new Menu::Item(item4, sizeof(item4) - 1);
    Menu::Item *i5 = new Menu::Item(item5, sizeof(item5) - 1);
    Menu::Item *i6 = new Menu::Item(item6, sizeof(item6) - 1);
    Menu::Item *i7 = new Menu::Item(item7, sizeof(item7) - 1);

    Menu::Item *i8 = new Menu::Item(item8, sizeof(item8) - 1);
    Menu::Item *i9 = new Menu::Item(item9, sizeof(item9) - 1);


    Menu::Item* main_items[] = {i0, i1, i2, i3, i4, i5, i6, i7};
    Menu::Item* sub_main_items[] = {i8,i9};

    controller.AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller.ControlGoToItem(1);
    controller.AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    controller.Render();

    for (uint8_t i = 0; i < 1; ++i) {
        controller.SelectNext();
    }

    controller.ExecuteItem();

    controller.GoToParent();
    for (uint8_t i = 0; i < 4; ++i) {
        controller.SelectNext();
    }

    channel->Send(0, CMD_GOTO_MAIN_MENU, nullptr, 0);
    fsm->Transition(STATE_IDLE, false);
}

/*-----------------------   HIGHSCORE  -------------------------------*/
void HighscoreEnter(){
    printf("Enter highscore \n");

    // Select new SS
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();
    oldsspin = spi.current_pin;

    SPI_N::PIN nordic_pin = SPI_N::PIN(&PORTG, &DDRG, 1);
    spi.SetDevice(nordic_pin);

    // Reduce SCK speed
    SPCR |= (1 << SPR0) | (1 << SPR1);
    highscore_name_length = 0;
}

void HighscoreLoop(){
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Wait while the pin is low
    while(!(PING & (1 << PING0)));

    // New message, start transmission
    spi.WriteByte(0x00, 0);
    while(spi.GetAvailableReadBytes() == 0);
    uint8_t read_byte;
    spi.ReadByte(read_byte);

    // End character. Exit state
    if(read_byte == 0x33){
        fsm->Transition(STATE_IDLE, 0);
        return;
    }

    highscore_name[highscore_name_length] = (char) read_byte;
    highscore_name_length += 1;

    // Wait while the pin is high
    while(PING & (1 << PING0));
}

void HighscoreLeave(){
    // The the SS pin back to the old value
    SPI_N::SPI::GetInstance().SetDevice(oldsspin);

    // Increase clock speed
    SPSR |= (1<<SPI2X);
    SPCR &= ~(1<<SPR0);
    SPCR &= ~(1<<SPR1);

    // Transmit the data back to node 1
    uint8_t data[3 + MAX_NAME_LENGTH] = {(uint8_t)(highscore_score >> 8), (uint8_t)(highscore_score & 0xFF), highscore_name_length};
    memcpy(&data[3], highscore_name, highscore_name_length);
    channel->Send(0, CMD_SAVE_HIGHSCORE, data, 3 + highscore_name_length);
}

/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, InitializeLoop, nullptr},
/* 1. ONGOING                    */ {OngoingEnter, OngoingLoop, OngoingLeave},
/* 2. IDLE                       */ {IdleEnter, IdleLoop,       nullptr},
/* 3. GET HIGHSCORE              */ {HighscoreEnter, HighscoreLoop, HighscoreLeave},
                                    {RenderOLEDToNode1, nullptr, nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}