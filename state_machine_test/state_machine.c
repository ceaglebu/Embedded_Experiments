#include "state_machine.h"

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
    printf("Off Handler Runs\n");
    scanf("%d", &(FSM->event));
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
    printf("Active Handler Runs\n");
    scanf("%d", &(FSM->event));
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
    printf("Switching Handler Runs\n");
    return;
}

int main (void) {

    StateTable state_lookup = {
        [OFF]     = {[START_STOP] = ACTIVE,     [SWITCH] = last_state},
        [ACTIVE]  = {[START_STOP] = OFF,        [SWITCH] = SWITCHING },
        [SWITCHING]  = {[START_STOP] = last_state, [SWITCH] = last_state}
    };

    Event event = last_event;
    uint8_t OCR0B = 0;
    uint8_t OCR0A = 0;
    Servo short_arm = (Servo) {.a_pulse_width_counts = &OCR0B, .angle_deg = 0, .dir = 1};
    Servo long_arm = (Servo) {.a_pulse_width_counts = &OCR0A, .angle_deg = 0, .dir = 1};

    StateHandler state_handlers[last_state] = {&off_handler, &active_handler, &switching_handler};
    StateMachine FSM = (StateMachine) {.state = OFF, .transition = true, .event=last_event};
    Conditions conditions = (Conditions) {.active_servo_idx = 0, .servo_list = {&short_arm, &long_arm}, .num_servos = 2};

    while (true) {
        printf("Event: %d\n", FSM.event);
        if (FSM.event != last_event) {
            printf("Event detected\n");
            State* new_state = malloc(1 * sizeof(State));
            *new_state = state_lookup[FSM.state][FSM.event];
            printf("New state: %d\n", *new_state);
            if (*new_state != last_state) {
                StateMachine_change_state(&FSM, *new_state);
            }
            free(new_state);
            FSM.event = last_event;
        }
        (*(state_handlers[FSM.state]))(&FSM, &conditions);
    }
}