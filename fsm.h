#pragma once
#include "order.h"
#include "driver/elevio.h"
#include "order.h"
#include "light.h"
#include "timer.h"
#include "queue.h"

typedef enum {INIT, IDLE, MOVING, DOOR_OPEN, EMERGENCY_STOP} state;

extern state current_state;

int fsm_valid_stop();

void set_next_stop(int floor_request);

void fsm_run(int floor, int current_floor);