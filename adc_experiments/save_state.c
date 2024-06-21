// Similar to servo integration.c, but handles button inputs to save and recall position.

// Servos have write mode and read mode. 
// Write mode bases servo position on potentiometers.
// Read mode Puts servo position in saved state.

// Button in D2 = mode switch button
// Button in D3 = save button

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "adc_hal.h"

typedef struct _Servo {
    volatile uint8_t * a_pwm_width;
    ADC_selection adc_pot;
    int angle;
    int saved_angle;
} Servo;

// Enums for state and events. last_xxx serves as count of xxx. Also useful for placeholder value guaranteed to be larger than largest other enum value.
typedef enum {WRITE, READ, SAVING, last_state} state;
typedef enum {SWITCH, SAVE, last_event} event;

volatile state current_state = WRITE;
volatile event current_event = last_event;

volatile bool state_transition = false;

int adc_to_angle(adc_result adc) {
    return (int) (180 * (float) adc / 1023);
}

void Servo_write(Servo* servo, int angle) {
    if (angle == servo->angle) {
        return;
    }

    int pwm_width = ((float) angle / 180) * 31 + 7;
    *(servo->a_pwm_width) = pwm_width;
    servo->angle = angle;
}

void change_state(state new_state) {
    current_state = new_state;
    state_transition = true;
    current_event = last_event;
}

int main(void) {

    // Set up buttons for input, servos for output
    DDRD &= ~(_BV(DDD2) | _BV(DDD3));
    DDRD |= _BV(DDD5) | _BV(DDD6);

    // Set up Fast PWM on timer 0 for servos

    // Timer clock select
    TCCR0B |= _BV(CS02) | _BV(CS00);

    // Select PWM mode and non-invert outputs
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1);

    // Set up ADCs
    adc_init();
    adc_enable_pin(ADC_1);
    adc_enable_pin(ADC_2);
    sei();

    Servo servo_1 = (Servo) {.a_pwm_width = &OCR0B, .adc_pot = ADC_1, .angle = 0, .saved_angle = 0};
    Servo servo_2 = (Servo) {.a_pwm_width = &OCR0A, .adc_pot = ADC_2, .angle = 0, .saved_angle = 0};

    Servo* a_servos[2] = {&servo_1, &servo_2};

    bool save_down = false;
    bool switch_down = false;

    while (true) {
        if (PIND & _BV(PIND3) && !save_down) {
            _delay_ms(5);
            if (PIND & _BV(PIND3)) {
                current_event = SAVE;
                save_down = true;
            }
        } else if (!(PIND & _BV(PIND3)) && save_down) {
            save_down = false;
        } else if (PIND & _BV(PIND2) && !switch_down) {
            _delay_ms(5);
            if (PIND & _BV(PIND2)) {
                current_event = SWITCH;
                switch_down = true;
            }
        } else if (!(PIND & _BV(PIND2)) && switch_down) {
            switch_down = false;
        }

        
        switch (current_state)
        {
        case WRITE:
            
            switch (current_event)
            {
            case SWITCH:
                change_state(READ);
                break;
            case SAVE:
                change_state(SAVING);
                break;
            default:
                for (int idx_servo = 0; idx_servo < 2; idx_servo += 1) {
                    Servo* a_servo = a_servos[idx_servo];
                    adc_select_pin(a_servo -> adc_pot);
                    Servo_write(a_servo, adc_to_angle(adc_read()));
                }
                break;
            }
            break;
        case SAVING:
            for (int idx_servo = 0; idx_servo < 2; idx_servo += 1) {
                Servo* a_servo = a_servos[idx_servo];
                a_servo->saved_angle = a_servo->angle;
            }
            change_state(WRITE);
            break;
        case READ:
            switch (current_event)
            {
                case (SWITCH):
                    change_state(WRITE);
                    break;
                default:
                    if (state_transition) {
                        for (int idx_servo = 0; idx_servo < 2; idx_servo += 1) {
                            Servo* a_servo = a_servos[idx_servo];
                            Servo_write(a_servo, a_servo->saved_angle);
                        }
                        state_transition = false;
                    }
            }
            break;
        default:
            break;
        }
        _delay_ms(5);
    }
}