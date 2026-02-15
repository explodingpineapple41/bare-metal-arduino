#include <util/delay.h>
#include "gpio.h"

int main() {
    analog_init();
    pwm_init();
    
    set_pin_mode(5, OUTPUT);
    set_pin_mode(12, INPUT_PULLUP);
    set_pin_mode(13, OUTPUT);
    
    
    while (1) {
        int input = analog_read(A0);
       
        pwm_write(3, input / 4);
    }
    
    return 0;
}