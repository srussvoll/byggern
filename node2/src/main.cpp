#include <util/delay.h>

#include "../lib/servo/servo.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    //Servo servo(900, 2100);
    DDRA |= 1 << DDA7;
    while(true){
        PORTA |= 1 << PORTA7;
        _delay_ms(1000);
        PORTA |= 0 << PORTA7;
        _delay_ms(1000);
    }
}
#pragma clang diagnostic pop