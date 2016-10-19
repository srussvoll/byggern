#include "lib/servo/servo.h"
#include <avr/io.h>
#include <util/delay.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    Servo servo(900, 2100);

    int16_t angle = 0;
    while(true) {
        _delay_ms(1000);
        angle += 20;
        servo.SetAngle(angle);
    }
}
#pragma clang diagnostic pop