// --------------------------------------- //
// INTEGRATE NORMAL C CODE INTO EMBEDDED C //
// SWITCH BETWEEN ACTIVATING TWO SERVOS    //
// --------------------------------------- //

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <stdbool.h>
#include <avr/delay.h>

#define SERVO_SPEED (1)

typedef struct _Servo {
    volatile uint8_t * pulse_width_cycles;
    int angle;
    int dir;
} Servo;

void Servo_write(Servo* servo, int angle) {
    int pulse_width_cycles = ((float) angle / 180) * 31 + 7;
    *(servo->pulse_width_cycles) = pulse_width_cycles;
    servo->angle = angle;
}

int main(void) {
    // pinModes
    // D2   - Input Button
    // D5/6 - PWM Servo outputs (Timer0 fastPWM outputs)
    DDRD |= _BV(DDD5) | _BV(DDD6);
    DDRD &= ~_BV(DDD2);

    // Set up fast PWM on timer 0
    // --------------------------

    // Select clock
    TCCR0B |= _BV(CS02) | _BV(CS00);

    // Select PWM mode and non-invert outputs
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1);
   
    // Set timer compare registers
    Servo short_arm = (Servo) {.pulse_width_cycles = &OCR0B, .angle = 0, .dir = 1};
    Servo long_arm = (Servo) {.pulse_width_cycles = &OCR0A, .angle = 0, .dir = 1};

    Servo* active_servo = &short_arm;
    int active_servo_idx = 0;
    Servo* servos[2] = {&short_arm, &long_arm};
    int num_servos = 2;

    bool button_down = false;

    for (int servo_idx = 0; servo_idx < num_servos; servo_idx++) {
        Servo_write(servos[servo_idx], 90);
    }

    while (true) {
        if (!button_down && PIND & _BV(PIND2)) {
            if (++active_servo_idx > num_servos - 1) {
                active_servo_idx = 0;
            }
            active_servo = servos[active_servo_idx];
            button_down = true;
        } else if (button_down && !(PIND & _BV(PIND2))) {
            button_down = false;
        }
        
        active_servo->angle += active_servo->dir * SERVO_SPEED;
        if (active_servo->angle < 0) {
            active_servo->angle = 0;
            active_servo->dir *= -1;
        } else if (active_servo->angle > 180) {
            active_servo->angle = 180;
            active_servo->dir *= -1;
        }

        Servo_write(active_servo, active_servo->angle);
        _delay_ms(5);
    }
}