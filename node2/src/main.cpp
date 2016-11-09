#include <avr/io.h>
#include <util/delay.h>

#include <lib/dac/dac.h>
#include <lib/ir_detector/ir_detector.h>
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/motor/motor.h"
#include "lib/adc_internal/adc_internal.h"

void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));
void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){

    UART& uart = UART::GetInstance();

    DAC& dac = DAC::GetInstance();
    dac.Initialize(0, 0xFF, 0x50);
    printf("--START-- \n\r");
    Motor& motor = Motor::GetInstance();
    motor.Initialize();
    motor.Start();
    while(true){
        motor.GoLeft();
        motor.Drive(0.7);
        _delay_ms(2000);
        motor.GoRight();
        motor.Drive(0.7);
        _delay_ms(2000);
    }

    /*IR_Detector& ir = IR_Detector::GetInstance();
    ir.Initialize(0x30);
    while(true){
        if(ir.Sample()){
            printf("HIT\r\n");
        }
        _delay_ms(50);
    } */
    
    /*
    while(true){
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0x00);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0xA0);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0x40);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0xFF);
    } */
}

#pragma clang diagnostic pop