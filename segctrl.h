#define DIS4 0xc2
#define DIS3 0xc1
#define DIS2 0x83
#define DIS1 0x43
#define ZHASNUTO 0xff

unsigned char znak[36] = {
    // Patterns for displaying characters A-Z and numbers 0-9
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // B
    0x39, // C
    0x5E, // D
    0x79, // E
    0x71, // F
    0x3D, // G
    0x76, // H
    0x30, // I
    0x1E, // J
    0x76, // K (similar to H)
    0x38, // L
    0x37, // M
    0x54, // N
    0x3F, // O (same as 0)
    0x73, // P
    0x67, // Q
    0x50, // R
    0x6D, // S (same as 5)
    0x78, // T
    0x3E, // U
    0x3E, // V (similar to U)
    0x3E, // W (similar to U)
    0x6E, // X
    0x6F, // Y (same as 9)
    0x5B  // Z
};

void display_init(void) {
    DDRC |= (1 << PC7) | (1 << PC6); // Set PC7 and PC6 as output
    DDRD |= 0xFF; // Set all PORTD pins as output
}

void display_show_characters(const char *characters) {
    uint8_t i = 0;
    while (characters[i] != '\0') {
        if (characters[i] >= '0' && characters[i] <= '9') {
            PORTD = znak[characters[i] - '0'];
        } else if (characters[i] >= 'A' && characters[i] <= 'Z') {
            PORTD = znak[characters[i] - 'A' + 10];
        } else {
            // Character not supported, you can add more cases as needed
        }
        
        // Select the appropriate digit
        switch (i) {
            case 0:
                PORTC = DIS4;
                break;
            case 1:
                PORTC = DIS3;
                break;
            case 2:
                PORTC = DIS2;
                break;
            case 3:
                PORTC = DIS1;
                break;
            default:
                // More than 4 characters, handle as needed
                break;
        }

        _delay_ms(20); // Adjust delay as needed
        PORTC = ZHASNUTO; // Turn off the selected digit
        i++;
    }
}

void display_clear(void) {
    PORTC = ZHASNUTO;
}