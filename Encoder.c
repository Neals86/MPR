#define F_CPU 11059200UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>

#define CHECKBIT(x,y) (x & (y)) /* Check bit y in byte x*/
#define SETBIT(x,y) (x |= (y)) /* Set bit y in byte x*/
#define CLEARBIT(x,y) (x &= (~y)) /* Clear bit y in byte x*/

//lookup table ascii char to 7 segment common anode type
static const uint8_t PROGMEM sevsegascii_table[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,	0x80, 0x90, 0xbf // 0 1 2 3 4 5 6 7 8 9 -
};

void displayNumber(int number);
void printNumber(int number);
volatile int num;
volatile int ones;
volatile int tens;
volatile int hundreds;
volatile int thousands;

int main(void)
{
	DDRB = 0xff;
	PORTB = 0x00;
	DDRD = 0x00;   //input port
	PORTD = 0xFF;  //enable pull up resistor
	DDRC = 0xff;
	PORTC = 0x10;
	
	TCCR0 |= (1<< CS02);	//256 PRESCALER
	TCCR0 |= (1<< WGM01);	//CTC MODE
	OCR0 = 65;	//4ms //setting how fast to refresh the display

	MCUCR = 0x01;			//int0 interrupt on level change
	
	GICR  = 0x40;			//enable the int0 interrupt
	TIMSK |= (1<<OCIE0);	//enable the timer0 compare interrupt
	sei();					//enable global interrupts

	num = 10;	//initializing the "counter"

	while(1)
	{
			printNumber(num);
	}
	return 0;
}
void printNumber(int number)
{
	int temp = 0;
	//splitting the number into separate numbers
	//to display on the 4 digit 7-segment display
	thousands = floor(number / 1000);
	temp = number % 1000;
	hundreds = floor(temp/100);
	temp = number % 100;
	tens = floor(temp/10);
	ones = temp % 10;
}
//displaying the number on the 7 segment display
void displayNumber(int number)
{
	if (number<0) {
		number*=-1;
	}	
	PORTB = pgm_read_byte(&sevsegascii_table[number]);
}

ISR(TIMER0_COMP_vect)
{

	//selecting which digit to display
	//timing controlled by Timer0
	switch(PORTC)
	{
		case 0x80:
			displayNumber(hundreds);
			PORTC = 0x40;
			break;
		case 0x40:
			displayNumber(tens);
			PORTC = 0x20;
			break;
		case 0x20:
			displayNumber(ones);
			PORTC = 0x10;
			break;
		case 0x10:
			displayNumber(thousands);
			PORTC = 0x80;
			break;
		default:
			break;

	}
}

ISR(INT0_vect)
{

/* this works for MCUCR=0x01 i.e. interrupt on level change */

	// A raising edge
	if ((PIND & (1<<PD2))) {
		if ((PIND & (1<<PD0))) num++;	// B rising edge
		else num--;						// B falling edge
	}
	// A raising edge		
	else {
		if ((PIND & (1<<PD0))) num--;	// B rising edge
		else num++;						// B falling edge
	}
	
}