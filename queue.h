#pragma once
#include "driver/elevio.h"

#define NUMBER_OF_FLOORS 4
#define NUMBER_OF_BUTTONS 3



extern int queue[NUMBER_OF_FLOORS][NUMBER_OF_BUTTONS];

void add_order();
void set_current_floor(int* current_floor); 


int queue_any_orders();

int queue_find_entry();

void delete_order(int f);

void delete_all_orders();
