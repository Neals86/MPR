#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Define display constants
#define DIS4 0xc2
#define DIS3 0xc1
#define DIS2 0x83
#define DIS1 0x43
#define ZHASNUTO 0xff

unsigned char znak[16] = {0x81, 0xf3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xf1, 0x01, 0x21, 0x11, 0x07, 0x8d, 0x43, 0x0d, 0x1d};

void display_init(void) {
    DDRC |= (1 << PC7) | (1 << PC6); // Set PC7 and PC6 as output
    DDRD |= 0xFF; // Set all PORTD pins as output
}

void display_show_number(uint16_t number) {
    unsigned char jednotky = number % 10;
    number /= 10;
    unsigned char desitky = number % 10;
    number /= 10;
    unsigned char stovky = number % 10;
    number /= 10;
    unsigned char tisice = number % 10;

    PORTD = znak[jednotky];
    PORTC = DIS4;
    _delay_ms(20);
    PORTC = ZHASNUTO;

    PORTD = znak[desitky];
    PORTC = DIS3;
    _delay_ms(20);
    PORTC = ZHASNUTO;

    PORTD = znak[stovky];
    PORTC = DIS2;
    _delay_ms(20);
    PORTC = ZHASNUTO;

    PORTD = znak[tisice];
    PORTC = DIS1;
    _delay_ms(20);
    PORTC = ZHASNUTO;
}

// Variables for the rotary encoder
volatile int16_t counter = 0;
volatile uint8_t encoder_a_last = 0;

int main(void) {
    // Initialize display
    display_init();

    // Set PORTB as input for encoder and button, with pull-up resistors
    DDRB = 0b00000000;
    PORTB = 0b00000111;

    while (1) {
        // Read encoder
        uint8_t encoder_a = PINB & (1 << PB0);
        uint8_t encoder_b = PINB & (1 << PB1);

        if (encoder_a != encoder_a_last) {
            if (encoder_a != encoder_b) {
                counter++;
            } else {
                counter--;
            }
            if (counter > 1000) counter = 1000;
            if (counter < 0) counter = 0;
        }
        encoder_a_last = encoder_a;

        // Check if the button is pressed
        if (!(PINB & (1 << PB2))) {
            counter = 0;
        }

        // Show the number on the display
        display_show_number(counter);
    }
}
