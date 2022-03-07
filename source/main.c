#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

#include "fsm.h"

//testing oifhfubhgir 

int main(){
    elevio_init();
    
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    //int floor = elevio_floorSensor();
    set_initial_condition(); 
    
    int current_floor;

    while(1){

        int floor = elevio_floorSensor();
        //int floor_request = get_floor_request();
        set_current_floor(&current_floor);

        floor_light(floor);
        

        fsm_run();
        
        

        

        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return 0;
}   