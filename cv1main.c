#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  

#define F_CPU 16000000 
#define BLINKS 5        


static uint8_t pattern = 0xAA;    
static uint8_t blink_flag = 0;    
static uint8_t blink_count = 0;  
uint8_t overflow_count = 0;

void timer0_init() {
	TCCR0 = (1 << WGM01);           
	TCCR0 |= (1 << CS02) | (1 << CS00);  
	OCR0 = 7812;                     
	TIMSK |= (1 << OCIE0);          
}

ISR(TIMER0_COMP_vect) {
	  
	overflow_count++;
if (overflow_count >= 61) {
blink_flag = 1;
overflow_count = 0;
}
}

int main() {
	
	DDRB = 0xFF;    
	PORTB = 0xFF;   

	DDRA &= ~((1 << PA1) | (1 << PA0));  
	PORTA |= (1 << PA1) | (1 << PA0);    
	
	timer0_init();

	
	sei();


	while (1) {
		
		if (!(PINA & (1 << PA0))) {  
			while (!(PINA & (1 << PA0))) {
				_delay_ms(50);  
			}

			
			for (blink_count = 0; blink_count < BLINKS * 2; blink_count++) { 
				while (!blink_flag) {
					_delay_ms(50);  
				}

				blink_flag = 0;  
				
				if (!(PINA & (1 << PA1))) {
					PORTB = 0xFF;
					}
				
				if (pattern == 0xAA) {
					pattern = 0x55;  
					} else {
					pattern = 0xAA;  
				}

				PORTB = pattern;  

			
				if (!(PINA & (1 << PA1))) {
					PORTB = 0xFF;  
					break;         
				}
			}

			PORTB = 0xFF;  
		}

		
	if (!(PINA & (1 << PA1))) {  
			PORTB = 0xFF;            

			while (!(PINA & (1 << PA1))) {
				_delay_ms(50);  
			}
		}
	}


	
}	
