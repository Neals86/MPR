#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU 16000000UL

volatile uint8_t blink_count = 0;
volatile uint8_t button_s1_pressed = 0;

// Function to initialize Timer 0
void timer0_init() {

    TCCR0 = (1 << WGM01); // CTC mode
    // Set the prescaler to 1024
    TCCR0 |= (1 << CS02) | (1 << CS00);
    OCR0 = 77; // This value gives approximately 0.5 second delay
    TIMSK |= (1 << OCIE0);
}


ISR(TIMER0_COMP_vect) {

    if (blink_count < 10) {
        PORTB ^= 0xFF;
        blink_count++;
    } else {
        TIMSK &= ~(1 << OCIE0);
        PORTB = 0xFF;
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

        if (!(PINA & (1 << PA0))) {

            for (volatile uint32_t i = 0; i < 10000; i++);
            if (!(PINA & (1 << PA0))) {

                blink_count = 0;
                TIMSK |= (1 << OCIE0);
                while (blink_count < 10 && !(PINA & (1 << PA1)));
                button_s1_pressed = 0;
            }
        }

        // Check for button S1 press to stop blinking
        if (!(PINA & (1 << PA1))) {
            // Debounce the button press, not needed in theory.
            for (volatile uint32_t i = 0; i < 10000; i++); // Simple software delay
            if (!(PINA & (1 << PA1))) {
                // Button S1 pressed, stop blinking
                TIMSK &= ~(1 << OCIE0); // Disable Timer interrupt
                PORTB = 0xFF; // Turn off all LEDs (active low)
            }
        }
    }

    return 0;
}
