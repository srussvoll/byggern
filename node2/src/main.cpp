#include <avr/io.h>
#include <util/delay.h>

#include <lib/dac/dac.h>
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

    printf("--START-- \n\r");
    DAC& dac = DAC::GetInstance();
    dac.Initialize(0, 0xFF, 0x50);
    dac.Reset();


    dac.SetDAC(4);
    //dac.WriteAnalogSignalRaw(0xFF);
    DDRH |= (1 << DDH1);
    while(true){
        PORTH |= (1 << PORTH1);
        _delay_ms(2000);
        dac.WriteAnalogSignalPercentage(0.3);
        PORTH &= ~(1 << PORTH1);
        _delay_ms(2000);
        dac.WriteAnalogSignalPercentage(0.3);
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