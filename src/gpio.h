#ifndef gpio_h
#define gpio_h

#include <avr/io.h>

#define LOW 0
#define HIGH 1

#define INPUT 0b00
#define OUTPUT 0b01
#define INPUT_PULLUP 0b10

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

#define pin_to_ddr(x) ((x) < 8 ? &DDRD : ((x) < 14 ? &DDRB : &DDRC))
#define pin_to_port(x) ((x) < 8 ? &PORTD : ((x) < 14 ? &PORTB : &PORTC))
#define pin_to_pr(x) ((x) < 8 ? &PIND : ((x) < 14 ? &PINB : &PINC))
#define pin_to_bm(x) _BV((x) < 8 ? (x) : ((x) - 8) % 6)

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

void digital_write(uint8_t pin, uint8_t bool) {
    if (pin > 19) return;

    volatile uint8_t *port = pin_to_port(pin);
    volatile uint8_t bm = pin_to_bm(pin);

    if (bool)
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

inline void analog_init() {
    ADMUX = _BV(REFS0);
    ADCSRA = 0x87;
}

unsigned int analog_read(uint8_t pin) {
    if (pin < 14 || pin > 19) return 0;

    ADMUX = ADMUX & (0xF8 + pin - 14);
    ADCSRA |= _BV(ADSC);

    while (ADCSRA & _BV(ADSC));

    return ADC;
}

void pwm_init() {
    TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS01) | _BV(CS00);  
}

void pwm_write(uint8_t pin, uint8_t val) {
    if (pin == 6)
        OCR0A = val;
    else
        OCR0B = val;

}

#endif
