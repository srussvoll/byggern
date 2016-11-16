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
#include "lib/spi/spi.h"

#define STATE_ONGOING        1
#define STATE_IDLE           2
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


    uint8_t highscore_name[10];
    uint8_t highscore_name_length;
    SPI_N::PIN oldsspin;
}

void SetMotorPID();

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeLoop() {
    //TODO: Add initialization to everything for node 2.
    // Initialize the motor
    Motor &motor = Motor::GetInstance();
    motor.Initialize();

    // Initialize the timer
    Timer &timer = Timer::GetInstance(0);
    timer.Initialize(1000, nullptr);

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

    //Initialize the solenoid
    Solenoid& solenoid = Solenoid::GetInstance();
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
    Timer &timer = Timer::GetInstance(0);
    Timer &timer2 = Timer::GetInstance(1);
    timer.Start();
    timer2.Start();
    x_direction = 0;
    y_direction = 0;
    touchbutton = false;
    encoder.Reset();
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

   // printf("X: %d, Y:%d \n", x_direction, y_direction);

    /*Direction direction = joystick.GetDirection();
    if((direction == West) || (direction == NorthWest) | direction == SouthWest){
        printf("Right \n");
        motor.Drive(joystick.GetPercentageX()*0.5);
        motor.GoRight();
    } else if((direction == East) || (direction == SouthEast) || (direction == NorthEast)){
        printf("Left\n");
        motor.Drive(joystick.GetPercentageX()*0.5);
        motor.GoLeft();
    } else {
        motor.Drive(0);
    }*/

    if(touchbutton){
        if(!touchbutton_last){
            printf("Solenoid \n");
            Solenoid& solenoid = Solenoid::GetInstance();
            solenoid.Pulse();
            touchbutton_last = true;
        }
    }else{
        touchbutton_last = false;
    }

    servo->SetAngle(-(slider_ang - 127));

    //printf("X: %3d, Y: %3d, pos: %3d \n", x_direction, y_direction, slider_pos);

    //motor.Drive(0);
    //_delay_ms(100);

    //printf("%d\n", (int) u * 100);
    //printf("%d\n", (int) e);

    // Check fail state
    IR_Detector &ir = IR_Detector::GetInstance();
    if(ir.Sample()) {
        printf("LOST GAME \n");
        fsm->Transition(STATE_IDLE, 0);
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

    //u = 0.5;
    if (u > 0) motor.GoRight();
    else       motor.GoLeft();
    u = fabs(u);
    //if (u > 0.8) u = 0.8;
    motor.Drive(u);
    //printf("r: %d, y: %d, e: %d, e_int: %d, u: %d\n", (int) (r * 100), (int) (y * 100), (int) (e * 100),
           //(int) e_integral, (int) (u * 100));
}

void OngoingLeave(){
    printf("Ongoing left\n");
    Motor &motor = Motor::GetInstance();
    motor.Stop();

    Timer &timer = Timer::GetInstance(0);
    Timer &timer2 = Timer::GetInstance(1);
    timer.Stop();
    timer2.Stop();
    uint16_t time;
    timer.GetFullSecondsPassed(time);
    printf("Time passed = %d \n\r", time);
    uint8_t data[] = {(uint8_t)(time & 0x00FF), (uint8_t)((time & 0xFF00)) >> 8};
    channel->Send(0, CMD_GAME_STOP, data, 2);
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
        //printf("CMD = %2x", command);
        if (command == CMD_GAME_START) {
            fsm->Transition(STATE_ONGOING, 0);
            return;
        }
    }
}

/*-----------------------   HIGHSCORE  -------------------------------*/
void HighscoreEnter(){
    // Select new SS
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();
    SPI_N::PIN nordic_pin = SPI_N::PIN(&DDRG, &PORTG, 1);
    spi.SetDevice(nordic_pin);
    spi.FlushInputBuffer();
    spi.FlushOutputBuffer();
    highscore_name_length = 0;

    oldsspin = spi.current_pin;
}

void HighscoreLoop(){
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();
    // Wait while the pin is low
    while(!(PING & (1 << PG0)));
    // New message, start transmission
    spi.WriteByte(0x00, 0);
    uint8_t read_byte;
    spi.ReadByte(read_byte);
    if(read_byte == 0x33){
        fsm->Transition(STATE_IDLE, 0);
        return;
    }
    highscore_name[highscore_name_length] = read_byte;
    highscore_name_length += 1;
    // Wait until the pin is high
    while(PING & (1 << PG0));
}

void HighscoreLeave(){
    // The the SS pin back to the old value
    SPI_N::SPI::GetInstance().SetDevice(oldsspin);
    // Transmit the data back to node 1
    for(int i = 0; i < highscore_name_length; i++){
        printf("REC BYTE %2x \n", highscore_name[i]);
    }
}

/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, InitializeLoop, nullptr},
/* 1. ONGOING                    */ {OngoingEnter, OngoingLoop, OngoingLeave},
/* 2. IDLE                       */ {IdleEnter, IdleLoop,       nullptr},
/* 3. GET HIGHSCORE              */ {HighscoreEnter, HighscoreLoop, HighscoreLeave}
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}