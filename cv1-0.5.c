#include <avr/io.h>
#include <avr/interrupt.h>

// Define the frequency of the oscillator
#define F_CPU 16000000UL

volatile uint8_t blink_count = 0; // To count the number of blinks
volatile uint8_t button_s1_pressed = 0; // To detect if button S1 is pressed

// Function to initialize Timer 0
void timer0_init() {
    // Set Timer 0 in CTC (Clear Timer on Compare Match) mode
    TCCR0 = (1 << WGM01); // CTC mode
    // Set the prescaler to 1024
    TCCR0 |= (1 << CS02) | (1 << CS00);
    // Calculate and set the Compare Match value for 0.5 seconds delay
    // Formula: OCR0 = (F_CPU / (2 * N * f)) - 1; where N is the prescaler (1024), f = 1 / delay (0.5 s)
    OCR0 = 77; // This value gives approximately 0.5 second delay
    // Enable Output Compare Match Interrupt for Timer 0
    TIMSK |= (1 << OCIE0);
}

// ISR for Timer 0 compare match
ISR(TIMER0_COMP_vect) {
    // Toggle LED pattern
    if (blink_count < 10) { // Blink 10 times
        PORTB ^= 0xFF; // Toggle all LEDs on PORTB (active low)
        blink_count++; // Increment blink count
    } else {
        TIMSK &= ~(1 << OCIE0); // Disable Timer interrupt after 10 blinks
        PORTB = 0xFF; // Turn off all LEDs (active low)
    }
}

// Main function
int main() {
    // Initialize I/O
    DDRB = 0xFF; // Set PORTB as output for LEDs
    PORTB = 0xFF; // Initial state of LEDs is off (active low)

    DDRA &= ~((1 << PA0) | (1 << PA1)); // Set PA0 and PA1 as input for buttons S0 and S1
    PORTA |= (1 << PA0) | (1 << PA1); // Enable pull-up resistors for buttons S0 and S1 (active low)

    // Initialize Timer 0
    timer0_init();

    // Enable global interrupts
    sei();

    // Main loop
    while (1) {
        // Check for button S0 press (assuming S0 is connected to PA0)
        if (!(PINA & (1 << PA0))) { // If S0 is pressed (active low)
            // Debounce the button press
            for (volatile uint32_t i = 0; i < 10000; i++); // Simple software delay
            if (!(PINA & (1 << PA0))) { // Confirm button press
                // Button S0 is confirmed pressed; Start blinking
                blink_count = 0; // Reset blink counter
                TIMSK |= (1 << OCIE0); // Enable Timer interrupt
                while (blink_count < 10 && !(PINA & (1 << PA1))); // Wait for 10 blinks or S1 press
                button_s1_pressed = 0; // Reset S1 press flag
            }
        }

        // Check for button S1 press to stop blinking (assuming S1 is connected to PA1)
        if (!(PINA & (1 << PA1))) { // If S1 is pressed (active low)
            // Debounce the button press
            for (volatile uint32_t i = 0; i < 10000; i++); // Simple software delay
            if (!(PINA & (1 << PA1))) { // Confirm button press
                // Button S1 pressed, stop blinking
                TIMSK &= ~(1 << OCIE0); // Disable Timer interrupt
                PORTB = 0xFF; // Turn off all LEDs (active low)
            }
        }
    }

    return 0;
}