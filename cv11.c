#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t overflow_count = 0;

void timer0_init() {
    // Set CTC mode (WGM01)
    TCCR0 = (1 << WGM01); // Set CTC mode

    // Set prescaler to 1024 (CS02 and CS00)
    TCCR0 |= (1 << CS02) | (1 << CS00); // Prescaler 1024

    // Set OCR0 to 255
    OCR0 = 255; // Count from 0 to 255

    // Enable Timer0 compare match interrupt
    TIMSK |= (1 << OCIE0); // Enable Timer0 Compare Match Interrupt

    // Enable global interrupts
    sei();
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMP_vect) {
    overflow_count++;
    
    // Check if 61 overflows have occurred
    if (overflow_count >= 61) {
        // Reset overflow count
        overflow_count = 0;
        
        // Code to execute every 1 second
    }
}

int main(void) {
    timer0_init();

    while (1) {
        // Main loop can remain empty or do other tasks
    }
}