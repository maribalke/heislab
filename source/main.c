#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

#include "fsm.h"


int main(){
    
    elevio_init();

    while(1){

        int floor = elevio_floorSensor();
        if(floor != -1){
            elevio_floorIndicator(floor);
        }
        
        fsm_run();
        
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return 0;
}   
