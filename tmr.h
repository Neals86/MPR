#include <avr/io.h>

// Timer prescaler values
#define TIMER_PRESCALER_1       (0b001 << CS00)
#define TIMER_PRESCALER_8       (0b010 << CS00)
#define TIMER_PRESCALER_64      (0b011 << CS00)
#define TIMER_PRESCALER_256     (0b100 << CS00)
#define TIMER_PRESCALER_1024    (0b101 << CS00)

// Timer modes
#define TIMER_MODE_NORMAL           0b0000
#define TIMER_MODE_PWM_PHASE_CORR   0b0100
#define TIMER_MODE_CTC              0b1100
#define TIMER_MODE_FAST_PWM         0b0100

// Function to set up timer
void setup_timer(uint8_t timer, uint8_t mode, uint8_t prescaler) {
    switch(timer) {
        case 0: // Timer/Counter0
            TCCR0 |= mode;
            TCCR0 |= prescaler;
            break;
        case 1: // Timer/Counter1
            TCCR1A |= (mode & 0x03);
            TCCR1B |= ((mode & 0x0C) << 1);
            TCCR1B |= prescaler;
            break;
        case 2: // Timer/Counter2
            TCCR2 |= mode;
            TCCR2 |= prescaler;
            break;
        default:
            // Invalid timer number
            break;
    }
}

// Function to set up timer interrupts
void enable_timer_interrupt(uint8_t timer) {
    switch(timer) {
        case 0: // Timer/Counter0
            TIMSK |= (1 << TOIE0);
            break;
        case 1: // Timer/Counter1
            TIMSK |= (1 << TOIE1);
            break;
        case 2: // Timer/Counter2
            TIMSK |= (1 << TOIE2);
            break;
        default:
            // Invalid timer number
            break;
    }
}