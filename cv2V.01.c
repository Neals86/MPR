#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define ENC_A (1 << PA0)
#define ENC_B (1 << PA1)
#define ENC_BTN (1 << PA2)
#define SEG_PORT PORTC
#define DIG_PORT PORTD

volatile int counter = 0;
unsigned char digit_segments[16] = {
    0x81, 0xf3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xf1,
    0x01, 0x21, 0x11, 0x07, 0x8d, 0x43, 0x0d, 0x1d
};

void display_number(int number) {
    int digits[4];
    digits[0] = number / 1000;
    digits[1] = (number / 100) % 10;
    digits[2] = (number / 10) % 10;
    digits[3] = number % 10;

    for (int i = 0; i < 4; i++) {
        DIG_PORT = ~(1 << i);
        SEG_PORT = digit_segments[digits[i]];
        _delay_ms(5); // Increased delay to 5ms
        SEG_PORT = 0xFF; // Turn off all segments before switching
    }
}

ISR(PCINT0_vect) {
    int new_A = PINA & ENC_A;
    int new_B = PINA & ENC_B;
    static int last_A = 0;

    if (new_A && !last_A) {
        if (new_B) {
            if (counter < 9999) counter++;
        } else {
            if (counter > 0) counter--;
        }
    }

    last_A = new_A;
}

ISR(INT0_vect) {
    if (!(PINA & ENC_BTN)) {
        counter = 0;
        _delay_ms(200);
    }
}

void setup() {
    DDRA &= ~(ENC_A | ENC_B | ENC_BTN);
    PORTA |= (ENC_A | ENC_B | ENC_BTN);
    DDRC = 0xFF;
    DDRD = 0xFF;
    SEG_PORT = 0xFF;
    DIG_PORT = 0xFF;

    // Enable pin change interrupt on PA0 and PA1
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (ENC_A | ENC_B);

    // Enable external interrupt on PA2
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    sei(); // Enable global interrupts
}

int main(void) {
    setup();

    while (1) {
        display_number(counter);
    }
}
