#include <avr/io.h>
#include <util/delay.h>

void init_button(uint8_t button_pin) {
    
    DDRB &= ~(1 << button_pin);
    // Enable pull-up resistor for button_pin
    PORTB |= (1 << button_pin);
}

int is_button_pressed(uint8_t button_pin){
    if (!(PINB & (1 << button_pin))) {
        _delay_ms(10); // Debounce delay
        if (!(PINB & (1 << button_pin))) {
            return 1;
        }
    }
    return 0;
}
