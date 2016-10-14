#ifdef __AVR_ATMega162__

#include "init.h"

#include <avr/io.h>
#include "lib/utilities/memory.h"
#include "lib/utilities/printf.h"
#include "lib/uart/uart.h"

void init_memory() {
    Utilities::InitializeMemory();
}

void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
}

#endif
