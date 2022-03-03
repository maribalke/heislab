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

int get_floor_request();
int get_btn_request();

void fsm_run(int current_floor);

