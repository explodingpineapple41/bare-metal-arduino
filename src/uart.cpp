#include "uart.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define UBRR ((F_CPU / (16UL * BAUD)) - 1)

volatile unsigned char UARTSerial::buffer[BUF_SIZE];
volatile unsigned char UARTSerial::bufHead = 0;
volatile unsigned char UARTSerial::bufTail = 0;

ISR(USART_RX_vect) {
    UARTSerial::handleRx();
}

void UARTSerial::init() {
    UBRR0H = UBRR >> 8;
    UBRR0L = UBRR;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    sei();
}

// Blocking send function
void UARTSerial::send(char c) {
    while (!(UCSR0A & _BV(UDRE0)));
    UDR0 = c;
}

// Expects string to be terminated by null character '\0'
void UARTSerial::print(const char s[]) {
    while (*s)
        send(*s++);
}

// Only unsigned 2 byte integers for now
void UARTSerial::sendInt(int x) {
    char number[] = {'0', '0', '0', '0', '0', '\0'};
    
    for (int i = 4; i >= 0; --i) {
        number[i] = (x % 10) + '0';
        x /= 10;
    }

    print(number);
}

bool UARTSerial::isAvailable() {
    return bufHead != bufTail;
}

// The buffer may not recieve all the characters before the CPU runs instructions so this function checks if a full line is recieved via UART
bool UARTSerial::hasString() {
    unsigned char i = bufTail;
    while (i != bufHead) {
        if (buffer[i++] == '\n')    
            return true;
    }
    return false;
}

void UARTSerial::flushBuffer() {
    bufHead = 0;
    bufTail = 0;
}

// This expects data to be present in the buffer
unsigned char UARTSerial::readByte() {
    return buffer[bufTail++];
}

// This expects strings stored in the buffer to end with a newline character '\n'
unsigned char UARTSerial::readString(char str[], unsigned char size) {
    unsigned char len = 0;
    while (len < size) {
        char c = readByte();
        if (c == '\n')
            break;
        
        str[len++] = c;
    }
    str[len] = '\0';
    return len;
}

// Only to be used by the ISR
void UARTSerial::handleRx() {
    unsigned char data = UDR0;
    UARTSerial::buffer[UARTSerial::bufHead++] = data;
}
