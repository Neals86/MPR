#include <avr/io.h>
#include <avr/interrupt.h>

void timer0_init() {
    // Set CTC mode (WGM01)
    TCCR0 = (1 << WGM01); // TCCR0 = 0x02

    // Set prescaler to 1024 (CS02 and CS00)
    TCCR0 |= (1 << CS02) | (1 << CS00); // TCCR0 = 0x07

    // Set OCR0 to 255 for 1-second overflow
    OCR0 = 255; // Lower byte is 0xFF

    // Enable Timer0 compare match interrupt
    TIMSK |= (1 << OCIE0); // Enable interrupt

    // Enable global interrupts
    sei();
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMP_vect) {
    // Code to execute every 1 second
}

int main(void) {
    timer0_init();

    while (1) {
        // Main loop
    }
}