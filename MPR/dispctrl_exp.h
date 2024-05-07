

#define DIS4 0xc2
#define DIS3 0xc1
#define DIS2 0x83
#define DIS1 0x43
#define ZHASNUTO 0xff

unsigned char znak[7] = {0x0d, 0x83, 0x81, 0x25, 0x8f, 0x11, 0x13};

void display_init(void) {
    DDRC |= (1 << PC7) | (1 << PC6) | (1 << PC0) | (1 << PC1); 
    DDRD |= 0xFF; 
	PORTC = 0xff;
}
static void  _delay_ms (double _ms);




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
    _delay_ms(10);
    PORTC = ZHASNUTO;

    PORTD = znak[desitky];
    PORTC = DIS3;
    _delay_ms(10);
    PORTC = ZHASNUTO;

    PORTD = znak[stovky];
    PORTC = DIS2;
    _delay_ms(10);
    PORTC = ZHASNUTO;

    PORTD = znak[tisice];
    PORTC = DIS1;
    _delay_ms(10);
    PORTC = ZHASNUTO;
}