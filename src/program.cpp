#include <util/delay.h>
#include "uart.hpp"
#include "gpio.h"

int main(void) {
    analog_init();
    pwm_init();
    
    UARTSerial::init();
    
    char str[16];

    while (1) {
        if (UARTSerial::hasString()) {
            UARTSerial::readString(str, 16);
            UARTSerial::print(str);
            UARTSerial::send('\n');
        }
    }
    
    return 0;
}