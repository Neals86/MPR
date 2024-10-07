#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL  // Updated clock speed to 16 MHz
#include <util/delay.h>

// Pin definitions
#define ENC_A (1<<PA0)     // Encoder A input
#define ENC_B (1<<PA1)     // Encoder B input
#define ENC_BTN (1<<PA2)   // Encoder button input (reset)
#define SEG_PORT PORTC     // Port connected to segments of the display
#define DIG_PORT PORTD     // Port connected to digits

volatile int counter = 0;  // Counter value (0-9999)
volatile int last_state = 0;  // Last state of encoder A and B

// 7-segment digit codes (common anode, segments a to g)
uint8_t seg_digits[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Function to display the counter value on the 7-segment display
void display_number(int number) {
    int digits[4];
    
    // Split the number into digits
    digits[0] = number / 1000;
    digits[1] = (number / 100) % 10;
    digits[2] = (number / 10) % 10;
    digits[3] = number % 10;
    
    // Display each digit in sequence (multiplexing)
    for (int i = 0; i < 4; i++) {
        DIG_PORT = ~(1 << i); // Select the digit
        SEG_PORT = ~seg_digits[digits[i]]; // Send the segment data
        _delay_ms(1); // Short delay for each digit (smaller value since clock is faster)
    }
}

// External Interrupt 0 for the encoder A signal (PA0)
ISR(INT0_vect) {
    int new_state = PINA & (ENC_A | ENC_B); // Read A and B

    // Clockwise rotation (A changes before B)
    if (((last_state & ENC_A) == 0) && ((new_state & ENC_A) != 0)) {
        if (new_state & ENC_B) {
            if (counter < 9999) counter++; // Increment counter
        } else {
            if (counter > 0) counter--; // Decrement counter
        }
    }

    // Counterclockwise rotation (B changes before A)
    if (((last_state & ENC_B) == 0) && ((new_state & ENC_B) != 0)) {
        if (new_state & ENC_A) {
            if (counter > 0) counter--; // Decrement counter
        } else {
            if (counter < 9999) counter++; // Increment counter
        }
    }

    last_state = new_state; // Update last state
}

// External Interrupt 1 for the reset button (PA2)
ISR(INT1_vect) {
    counter = 0; // Reset the counter
}

void setup() {
    // Set Port A as input for encoder (PA0, PA1) and button (PA2)
    DDRA &= ~(ENC_A | ENC_B | ENC_BTN);
    
    // Set Ports C and D as output for 7-segment display
    DDRC = 0xFF;
    DDRD = 0xFF;
    
    // Enable external interrupts for encoder and button
    GICR |= (1<<INT0) | (1<<INT1);    // Enable INT0 and INT1
    MCUCR |= (1<<ISC01) | (1<<ISC11); // Falling edge for INT0 and INT1
    
    // Initialize segment and digit ports
    SEG_PORT = 0xFF;
    DIG_PORT = 0xFF;
    
    // Enable global interrupts
    sei();
}

int main(void) {
    setup();
    
    while (1) {
        display_number(counter); // Continuously update the display with the counter value
    }
}