#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"

void set_initial_condition(int floor){
    printf("floor: %d \n",floor);
    if(floor == -1){
            elevio_motorDirection(DIRN_DOWN);
        }
        else{
            elevio_motorDirection(DIRN_STOP);
        }
}

int queue[4][3];
void set_request(int floor_request, ButtonType button){

}


