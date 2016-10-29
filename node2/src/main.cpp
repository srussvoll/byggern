#include "lib/servo/servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <lib/dac/dac.h>
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"

void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));
void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){

    printf("--START-- \n\r");
    DAC& dac = DAC::GetInstance();
    dac.Initialize(0, 0xFF, 0x50);
    dac.Reset();
    dac.SetDAC(4);
    dac.WriteAnalogSignalRaw(0xFF);

    while(true){
        _delay_ms(3000);
        dac.WriteAnalogSignalPercentage(0.5);
        _delay_ms(3000);
        dac.WriteAnalogSignalPercentage(1.0);
    }

    while(true){
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0x00);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0xA0);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0x40);
        _delay_ms(2000);
        dac.WriteAnalogSignalRaw(0xFF);
    }
}

#pragma clang diagnostic pop