#include "lib/servo/servo.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    Servo servo(900, 2100);
    while(true);
}
#pragma clang diagnostic pop