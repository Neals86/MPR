#include <avr/io.h>
#include <util/delay.h>

void init_button(uint8_t button_pin) {
    
    DDRB &= ~(1 << button_pin);
    // Enable pull-up resistor for button_pin
    PORTB |= (1 << button_pin);
}

int debounce(uint8_t button_pin){
    if (!(PINB & (1 << button_pin))) {
        _delay_ms(10); // Debounce delay
        if (!(PINB & (1 << button_pin))) {
            return 0;
        }
    }
    return 0;
}
