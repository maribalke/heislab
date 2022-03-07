#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"
#include "timer.h"


void order_light();

/**
 * @brief Updates floor indicator light at panel
 * 
 */
void floor_light(int floor); //etasjelys
void stop_lamp(void); //stopplys
void delete_light(int f);
void delete_all_lights();