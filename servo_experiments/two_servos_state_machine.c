// ------------------------------------ //
// IMPROVED 2 SERVO PROGRAM             //
// USE INTERRUPTS TO FREE PROCESSOR     //
// USE STATE MACHINE DESIGN PRINCIPLES  //
// ADD STOP / START BUTTON              //
// ------------------------------------ //



//  |=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
//  | STATES                              |
//  | - 0 : Servos off                    |
//  | - 1 : Servo  on, long arm queued    |
//  | - 2 : Active Servo Switching        |
//  |-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-|
#include "two_servos_state_machine.h"

#define SERVO_SPEED (1)

void Servo_write(Servo* servo, int angle) {
    int pulse_width_counts = ((float) angle / 180) * 31 + 7;
    *(servo->a_pulse_width_counts) = pulse_width_counts;
    servo->angle_deg = angle;
}

void StateMachine_change_state(StateMachine* FSM, State new_state) {
    FSM->state = new_state;
    FSM->transition = true;
    return;
}

void off_handler(StateMachine* FSM, Conditions* conditions) {
    if (FSM->transition == true) {
        FSM->transition = false;
    }
    // enter power save ?
    _delay_ms(10);
    return;
}

void active_handler(StateMachine* FSM, Conditions* conditions) {
    Servo* active_servo = conditions->servo_list[conditions->active_servo_idx];
    if (FSM->transition == true) {
        Servo_write(active_servo, active_servo->angle_deg);
        FSM->transition = false;
    }
    
    active_servo->angle_deg += active_servo->dir * SERVO_SPEED;
    if (active_servo->angle_deg < 0) {
        active_servo->angle_deg = 0;
        active_servo->dir *= -1;
    } else if (active_servo->angle_deg > 180) {
        active_servo->angle_deg = 180;
        active_servo->dir *= -1;
    }
    
    Servo_write(active_servo, active_servo->angle_deg);
    _delay_ms(5);
    return;
}

void switching_handler(StateMachine* FSM, Conditions* conditions) {
    if (FSM->transition == true) {
        FSM->transition = false;
    }
    if (++(conditions->active_servo_idx) >= conditions->num_servos) {
        conditions->active_servo_idx = 0;
    }
    StateMachine_change_state(FSM, ACTIVE);
    return;
}

// volatile bool start_stop_button_down = false;
// volatile bool start_stop_button_pressed = false;
// volatile bool switch_button_down = false;
// volatile bool switch_button_pressed = false;

// //TODO: Use a static variable to track when the button was initially pressed. 
// // Plan: - Activate interrupt on up and down
// //       - keep track if button is up or down (switch every time there is valid interrupt)
// //       - interrupt is not valid if another interrupt happened too recently

// ISR(INT0_vect) {
//     // If button is down: turn off button
//     // If button is not down: turn button on, start timer
//     switch_button_down = !switch_button_down
//     if (switch_button_down) {
        
//     }
//     event = SWITCH;
//     PORTB ^= _BV(PORTB5);
// }

// ISR(INT1_vect) {
//     event = START_STOP;
// }

int main (void) {

    // Setup GPIO pins for buttons and servos
    // D2 = Switch button, D3 = Stop button. D2 = INT0, D3 = INT1
    // D5 = PWM output B, D6 = PWM output A
    DDRD |= _BV(DDD5) | _BV(DDD6);
    DDRD &= ~(_BV(DDD2) | _BV(DDD3));
    DDRB |= _BV(PORTB5);

    // Set up timer0 for PWM

    // Select 1024 prescaled clock
    TCCR0B |= _BV(CS02) | _BV(CS00);
    // Select FastPWM mode with TOP = 0xFF
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    // Select non-inverted output
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1);

    // Initialize servos
    Servo short_arm = (Servo) {.a_pulse_width_counts = &OCR0B, .angle_deg = 0, .dir = 1};
    Servo long_arm = (Servo) {.a_pulse_width_counts = &OCR0A, .angle_deg = 0, .dir = 1};

    Servo_write(&short_arm, short_arm.angle_deg);
    Servo_write(&long_arm, long_arm.angle_deg);

    // Set up Interrupts
    // sei();
    // EIMSK |= _BV(INT1) | _BV(INT0); // Interrupt mask for external interrupts
    // EICRA |= _BV(ISC10) | _BV(ISC00); // Rising and Falling edge detection (use to determine length of button press)

    // Initialize state machine
    StateTable state_lookup = {
        [OFF]     = {[START_STOP] = ACTIVE,     [SWITCH] = last_state},
        [ACTIVE]  = {[START_STOP] = OFF,        [SWITCH] = SWITCHING },
        [SWITCHING]  = {[START_STOP] = last_state, [SWITCH] = last_state}
    };

    StateHandler state_handlers[last_state] = {&off_handler, &active_handler, &switching_handler};
    StateMachine FSM = (StateMachine) {.state = OFF, .transition = true, .event = last_event};
    Conditions conditions = (Conditions) {.active_servo_idx = 0, .servo_list = {&short_arm, &long_arm}, .num_servos = 2};

    bool start_stop_down = false;
    bool switch_down = false;

    while (true) {
        if (PIND & _BV(PIND3) && !start_stop_down) {
            _delay_ms(5);
            if (PIND & _BV(PIND3)) {
                FSM.event = START_STOP;
                start_stop_down = true;
            }
        } else if (!(PIND & _BV(PIND3)) && start_stop_down) {
            start_stop_down = false;
        } else if (PIND & _BV(PIND2) && !switch_down) {
            _delay_ms(5);
            if (PIND & _BV(PIND2)) {
                FSM.event = SWITCH;
                switch_down = true;
            }
        } else if (!(PIND & _BV(PIND2)) && switch_down) {
            switch_down = false;
        }
        if (FSM.event != last_event) {
            State* new_state = malloc(1 * sizeof(State));
            *new_state = state_lookup[FSM.state][FSM.event];
            if (*new_state != last_state) {
                StateMachine_change_state(&FSM, *new_state);
            }
            free(new_state);
            FSM.event = last_event;
            _delay_ms(5);
        }
        (*(state_handlers[FSM.state]))(&FSM, &conditions);
    }
}

// BUGS: starting in on mode, pressing start button switches active motor.