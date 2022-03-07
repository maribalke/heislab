#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"
#include "queue.h"
#include "light.h"
#include "timer.h"

typedef enum {INIT, IDLE, MOVING, DOOR_OPEN, EMERGENCY_STOP, OBSTRUCTION} state;

typedef enum {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_TBT} direction; //Trenger vi TBT

typedef enum {unknown = -1, first = 0, second = 1, third = 2, fourth = 3} position;

typedef struct
{
    position start;
    position stop;
    position next_stop;

} movement;


extern state current_state;

extern direction current_direction;

extern direction previous_direction;


void fsm_search_beyond_next_stop();

int fsm_valid_stop();

void elevator_direction();

void fsm_find_directon();

/**
 * @brief Initilize the elevator to a known state
 * 
 */
void set_initial_condition(); //definerer start tilstand

/**
 * @brief Function that updates the last passed floor
 * 
 * 
 * 
 */
void set_current_floor(int* current_floor);


void fsm_update_light();



/**
 * @brief Elevator goes to floor and stops
 * 
 * @param next_stop 
 * @param floor_indicator 
 */
void fsm_go_to(int next_stop);


//int fsm_is_emergancy();


/**
 * @brief Runs the whole elevator
 * 
 */
void fsm_run();


//Helper functions