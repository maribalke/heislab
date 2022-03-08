
#include <time.h>
#include "driver/elevio.h"

#define TIME_LIMIT 3

static int door_timer = -1;


void timer_start() {
	door_timer = time(NULL);
	elevio_doorOpenLamp(1);
}


int timer_expired() {
	if (door_timer == -1) return 0;
	else return (time(NULL) - door_timer > TIME_LIMIT);
}