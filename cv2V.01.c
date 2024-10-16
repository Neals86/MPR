#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL  
#define ENC_A (1 << PA0)     
#define ENC_B (1 << PA1)     
#define ENC_BTN (1 << PA2)   
#define SEG_PORT PORTC     
#define DIG_PORT PORTD     

volatile int counter = 0;  
volatile int last_A = 0, last_B = 0;  

unsigned char digit_segments[16] = {
    0x81, 0xf3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xf1,
    0x01, 0x21, 0x11, 0x07, 0x8d, 0x43, 0x0d, 0x1d
};

void display_number(int number) {
    int digits[4];
    digits[0] = number / 1000;
    digits[1] = (number / 100) % 10;
    digits[2] = (number / 10) % 10;
    digits[3] = number % 10;

    for (int i = 0; i < 4; i++) {
        DIG_PORT = ~(1 << i); 
        SEG_PORT = digit_segments[digits[i]]; 
        _delay_ms(5); // Increased delay to 5ms 
        SEG_PORT = 0xFF; // Turn off all segments before switching
    }
}

void poll_encoder() {
    int new_A = PINA & ENC_A;
    int new_B = PINA & ENC_B;

    if ((last_A != 0) && (new_A == 0)) {
        if (new_B != 0) {
            if (counter < 9999) counter++;
        } else {
            if (counter > 0) counter--;
        }
    }

    if ((last_B != 0) && (new_B == 0)) {
        if (new_A != 0) {
            if (counter > 0) counter--;
        } else {
            if (counter < 9999) counter++;
        }
    }

    last_A = new_A;
    last_B = new_B;
}

void poll_reset_button() {
    if (!(PINA & ENC_BTN)) { 
        counter = 0; 
        _delay_ms(200); 
    }
}

void setup() {
    DDRA &= ~(ENC_A | ENC_B | ENC_BTN); 
    PORTA |= (ENC_A | ENC_B | ENC_BTN);
    DDRC = 0xFF;
    DDRD = 0xFF; 
    SEG_PORT = 0xFF;
    DIG_PORT = 0xFF;
}

int main(void) {
    setup(); 

    while (1) {
        poll_encoder(); 
        poll_reset_button(); 
        display_number(counter); 
    }
}
