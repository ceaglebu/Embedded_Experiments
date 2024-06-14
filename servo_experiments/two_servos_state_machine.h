// ------------------------------------ //
// IMPROVED 2 SERVO PROGRAM HEADER      //
// USE INTERRUPTS TO FREE PROCESSOR     //
// USE STATE MACHINE DESIGN PRINCIPLES  //
// ADD STOP / START BUTTON              //
// ------------------------------------ //

//  |=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
//  | STATES                              |
//  | - 0 : Servos off, short arm queued  |
//  | - 1 : Servos off, long arm queued   |
//  | - 2 : Short arm servo active        |
//  | - 3 : Long arm servo active         |
//  |-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-|

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdlib.h>



// Servos
typedef struct _Servo {
    volatile uint8_t * a_pulse_width_counts;
    int angle_deg;
    int dir;
} Servo;

void Servo_write(Servo* servo, int angle);

// State
typedef enum {OFF, ACTIVE, SWITCHING, last_state} State;
typedef enum {START_STOP, SWITCH, last_event} Event;

typedef struct _StateMachine {
    State state;
    bool transition;
    Event event;
} StateMachine;

void StateMachine_change_state(StateMachine*, State);

typedef struct _Conditions {
    int active_servo_idx;
    Servo* servo_list[2]; // Hard coded here to allow for easy instantiation of conditions variable. Should be refactored to be Servo** servo_list. Servos should be an enum as well with last_servo
    int num_servos;
} Conditions;

typedef void (*StateHandler) (StateMachine*, Conditions*);
typedef State const StateTable[last_state][last_event];

// State Handlers
void init_handler(StateMachine* FSM, Conditions* conditions);
void off_handler(StateMachine* FSM, Conditions* conditions);
void active_handler(StateMachine* FSM, Conditions* conditions);
void switching_handler(StateMachine* FSM, Conditions* conditions);