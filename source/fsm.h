/**
 * @file fsm.h
 * 
 * @author Hermine Alfsen, Mari Balke Fjellang
 * @brief Finite state machine and other functions 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"
#include "queue.h"
#include "light.h"
#include "timer.h"

typedef enum {INIT, IDLE, MOVING, DOOR_OPEN, EMERGENCY_STOP, OBSTRUCTION} state;

typedef enum {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_NONE} direction; 

typedef enum {unknown = -1, first = 0, second = 1, third = 2, fourth = 3} position;



/**
 * @brief Updates the new floor position as the elevator is located at a floor. The previous value is stored.
 * 
 */
void fsm_update_current_floor();


/**
 * @brief Checks whether the next_stop-position is above or below the current position and sets the 
 * correct direction
 * 
 * @param next_stop 
 */
void fsm_go_to(int next_stop);


/**
 * @brief Finds the next direction based on the buttons pressed if the elevator has no previous direction.
 * 
 */
void fsm_find_directon();


/**
 * @brief Checks whether the elevator should stop at the floor where a button is pressed. 
 * @param next_stop is set to a floor if the if-statements are true
 */
void fsm_set_next_stop();


/**
 * @brief Checks whether the elevator is allowed to stop
 * @return 1 if success, 0 if the stop is not valid 
 * 
 */
int fsm_valid_stop();


/**
 * @brief Runs the elevator
 */
void fsm_run();

