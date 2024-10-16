#define F_CPU 11059200UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>

#define CHECKBIT(x, y) (x & (y)) /* Check bit y in byte x */
#define SETBIT(x, y) (x |= (y)) /* Set bit y in byte x */
#define CLEARBIT(x, y) (x &= (~y)) /* Clear bit y in byte x */

static const uint8_t PROGMEM sevsegascii_table[] = {
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0xbf  // 0 1 2 3 4 5 6 7 8 9 -
};

void displayNumber(int number);
void printNumber(int number);

volatile int num;
volatile int ones;
volatile int tens;
volatile int hundreds;
volatile int thousands;

void setup() {
    DDRB = 0xff;    // Set PORTB as output for the 7-segment display
    PORTB = 0x00;   // Initialize PORTB
    DDRD = 0x00;    // Set PORTD as input for the rotary encoder
    PORTD = 0xFF;   // Enable pull-up resistors on PORTD
    DDRC = 0xff;    // Set PORTC as output for digit control
    PORTC = 0x10;   // Initialize PORTC

    TCCR0 |= (1 << CS02);   // Set Timer0 prescaler to 256
    TCCR0 |= (1 << WGM01);  // Set Timer0 to CTC mode
    OCR0 = 65;              // Set the compare value for 4ms refresh rate

    MCUCR = 0x01;           // Set INT0 interrupt on level change
    GICR = 0x40;            // Enable the INT0 interrupt
    TIMSK |= (1 << OCIE0);  // Enable Timer0 compare interrupt
    sei();                  // Enable global interrupts

    num = 10;               // Initialize the counter
}

int main(void) {
    setup();
    while (1) {
        printNumber(num);
    }
    return 0;
}

void printNumber(int number) {
    int temp = 0;
    // Splitting the number into separate digits to display on the 4-digit 7-segment display
    thousands = floor(number / 1000);
    temp = number % 1000;
    hundreds = floor(temp / 100);
    temp = number % 100;
    tens = floor(temp / 10);
    ones = temp % 10;
}

void displayNumber(int number) {
    if (number < 0) {
        number *= -1;
    }
    PORTB = pgm_read_byte(&sevsegascii_table[number]);
}

ISR(TIMER0_COMP_vect) {
    // Selecting which digit to display, timing controlled by Timer0
    switch (PORTC) {
        case 0x80:
            displayNumber(hundreds);
            PORTC = 0x40;
            break;
        case 0x40:
            displayNumber(tens);
            PORTC = 0x20;
            break;
        case 0x20:
            displayNumber(ones);
            PORTC = 0x10;
            break;
        case 0x10:
            displayNumber(thousands);
            PORTC = 0x80;
            break;
        default:
            break;
    }
}

ISR(INT0_vect) {
    // This works for MCUCR = 0x01, i.e., interrupt on level change
    // A raising edge
    if ((PIND & (1 << PD2))) {
        if ((PIND & (1 << PD0))) num++;  // B rising edge
        else num--;  // B falling edge
    }
    // A falling edge
    else {
        if ((PIND & (1 << PD0))) num--;  // B rising edge
        else num++;  // B falling edge
    }
}
