#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t ms_counter = 0;   // Counter for milliseconds
volatile uint8_t pattern_counter = 0; // Counter to keep track of the pattern repetition
volatile uint8_t toggle_pattern = 0;  // Variable to toggle between patterns

void init_timer0(void) {
    // Set Timer0 to CTC mode with a prescaler of 1024
    TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);

    // Set the compare value for 1 ms at 16 MHz clock with 1024 prescaler
    OCR0 = 15;

    // Enable Output Compare Match Interrupt for Timer0
    TIMSK = (1 << OCIE0);
}

ISR(TIMER0_COMP_vect) {
    ms_counter++;  // Increment the millisecond counter

    if (ms_counter >= 250) {  // 250 ms reached
        ms_counter = 0;  // Reset millisecond counter
        
        if (pattern_counter < 20) { // 20 because each pattern is 0.25s, so 10 cycles need 20 interrupts
            if (toggle_pattern) {
                PORTB = 0b01010101;  // Pattern 01010101
            } else {
                PORTB = 0b10101010;  // Pattern 10101010
            }
            toggle_pattern = !toggle_pattern; // Toggle pattern
            pattern_counter++;  // Increment the pattern counter
        } else {
            // Stop the timer when pattern is done
            TCCR0 = 0; // Disable Timer0 by clearing the prescaler bits
        }
    }
}

int main(void) {
    // Initialize I/O
    DDRB = 0xFF;  // Set PORTB as output
    DDRA = 0x00;  // Set PORTA as input

    sei(); // Enable global interrupts

    while (1) {
        // Check if PA0 is low
        if (!(PINA & (1 << PA0))) {
            // Reset pattern counter and flags
            pattern_counter = 0;
            toggle_pattern = 0;
            ms_counter = 0;

            // Initialize Timer0
            init_timer0();
        } else {
            // If PA0 is high, turn off all LEDs
            PORTB = 0x00;
        }
    }

    return 0;
}