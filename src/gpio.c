#include "gpio.h"

void set_pin_mode(uint8_t pin, uint8_t mode) {
    if (pin > 19) return;

    volatile uint8_t *ddr = pin_to_ddr(pin);
    volatile uint8_t bm = pin_to_bm(pin);
    
    if (mode == INPUT) {
        *ddr &= ~bm;
    } else if (mode == OUTPUT) {
        *ddr |= bm;
    } else {
        volatile uint8_t *port = pin_to_port(pin);
        *ddr &= ~bm;
        *port |= bm;
    }
}

void digital_write(uint8_t pin, uint8_t out) {
    if (pin > 19) return;

    volatile uint8_t *port = pin_to_port(pin);
    volatile uint8_t bm = pin_to_bm(pin);

    if (out)
        *port |= bm;
    else
        *port &= ~bm;
}

void digital_toggle(uint8_t pin) {
    if (pin > 19) return;

    volatile uint8_t *port = pin_to_port(pin);
    volatile uint8_t bm = pin_to_bm(pin);

    *port ^= bm;
}

unsigned int digital_read(uint8_t pin) {
    if (pin > 19) return LOW;

    volatile uint8_t *pr = pin_to_pr(pin);
    volatile uint8_t bm = pin_to_bm(pin);

    return (*pr & bm) > 0;
}

unsigned int analog_read(uint8_t pin) {
    if (pin < 14 || pin > 19) return 0;

    ADMUX = ADMUX & (0xF8 + pin - 14);
    ADCSRA |= _BV(ADSC);

    while (ADCSRA & _BV(ADSC));

    return ADC;
}

// Plan on having only pins 5 & 6 support PWM for now
void pwm_write(uint8_t pin, uint8_t val) {
    if (pin == 6)
        OCR0A = val;
    else if (pin == 5)
        OCR0B = val;
}