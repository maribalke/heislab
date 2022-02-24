#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "order.h"
#include "light.h"



int main(){
    elevio_init();
    
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    //int floor = elevio_floorSensor();
    set_initial_condition(); 
    int current_floor;

    while(1){

        int floor = elevio_floorSensor();
        set_current_floor(&current_floor);

        floor_light(floor);
        order_light();


        add_order();
        floor_order(floor, current_floor);
        go(current_floor);
        

        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            stop_lamp();
            break;
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return 0;
}  
        
        

    

    

   