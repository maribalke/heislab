/**
 * @file light.h
 * @author Hermine Alfsen, Mari Balke Fjellang
 * @brief Functions to control the lights
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"
#include "timer.h"

/**
 * @brief Sets button lights
 * 
 */
void order_light();

/**
 * @brief Updates floor lights
 * 
 */
void floor_light(int floor); 
