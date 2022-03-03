#pragma once
#include "driver/elevio.h"

int queue[4][3];

void set_current_direction(MotorDirection direction);
MotorDirection get_current_direction();

int queue_any_orders();

int queue_any_orders_below(int current_floor);
int queue_any_orders_above(int current_floor);

void queue_prio(int current_floor);