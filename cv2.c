/*
 * cv2.c
 *
 * Èítaè MOD1000 ovládání pøes RE, T = reset
 * Author : Vladimir.Muzik
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>


#define DIS4 0xc2
#define DIS3 0xc1
#define DIS2 0x83
#define DIS1 0x43
#define OFF 0xff

unsigned char znak[16] = {0x81, 0xf3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xf1, 0x01, 0x21, 0x11, 0x07, 0x8d, 0x43, 0x0d, 0x1d};

void display_init(void) {
	DDRC = 0xff; 
	DDRD = 0xFF; 
}

void display_show_number(uint16_t number) {
	unsigned char jednotky = number % 10;
	number /= 10;
	unsigned char desitky = number % 10;
	number /= 10;
	unsigned char stovky = number % 10;
	number /= 10;
	unsigned char tisice = number % 10;

	PORTD = znak[jednotky];
	PORTC = DIS4;
	_delay_ms(2);
	PORTC = OFF;

	PORTD = znak[desitky];
	PORTC = DIS3;
	_delay_ms(2);
	PORTC = OFF;

	PORTD = znak[stovky];
	PORTC = DIS2;
	_delay_ms(2);
	PORTC = OFF;

	PORTD = znak[tisice];
	PORTC = DIS1;
	_delay_ms(2);
	PORTC = OFF;
}


volatile int16_t counter = 0;
volatile uint8_t encoder_a_last = 0;

int main(void) {
	
	display_init();

	
	DDRB = 0b00000000;
	PORTB = 0b00000111;

	while (1) {
		
		uint8_t encoder_a = PINB & (1 << PB0);
		uint8_t encoder_b = PINB & (1 << PB1);
display_show_number(counter);
		if (encoder_a != encoder_a_last) {
			
			_delay_ms(20);
			if (encoder_a != (PINB & (1 << PB0)))

			if (encoder_a != encoder_b) {
				
				counter++;
				} else {
				counter--;
				_delay_ms(7);
			}
			if (counter > 1000) counter = 1000;
			if (counter < 0) counter = 0;
		}
		encoder_a_last = encoder_a;
          
		
		if (!(PINB & (1 << PB2))) {
			counter = 0;
		}

		
		
	}
}