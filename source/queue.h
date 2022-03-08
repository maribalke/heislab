/**
 * @file queue.h
 * @author Hermine Alfsen, Mari Balke Fjellang
 * @brief Functions to control the queue
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "driver/elevio.h"

/**
 * @brief Zero matrix with number of floors times number of buttons.
 * 
 */
extern int queue[N_FLOORS][N_BUTTONS];

/**
 * @brief Initilize the elevator to a position that is not inbetween floors
 */
void queue_set_initial_condition();

/**
 * @brief The function sets the correct place in the matrix to 1 if the belonging button is pressed.
 * The belonging panel light will be turned on if button is pressed.
 */
void queue_add_order();

/**
 * @brief Iterates trough the matrix to check whether there is any orders or not. 
 * @return 1 if any orders, if else 0.
 */
int queue_any_orders();

/**
 * @brief Iterates though the matrix and finds the floor where a button is pressed.
 * 
 * @return floor
 */
int queue_get_floor();

/**
 * @brief Deletes all orders in the same floor f. The belonging lights is turned off
 * 
 * @param f 
 */
void queue_delete_order(int f);

/**
 * @brief Deletes all orders in the matrix and turn off all lights.
 * 
 */
void queue_delete_all_orders();
