#include "lib/servo/servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <lib/menu/menu.h>
#include <lib/utilities/fonts.h>
#include <lib/oled_scp/oled_scp.h>
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/scp/commands.h"
#include <lib/dac/dac.h>
#include <lib/ir_detector/ir_detector.h>
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/adc_internal/adc_internal.h"
#include "state_machine.h"
#include "lib/timer/timer.h"
#include "init.h"
void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    InitializeStateMachine();
}
#pragma clang diagnostic pop