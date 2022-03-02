#include "queue.h"
#include "driver/elevio.h"

#include <stdio.h>

static MotorDirection current_direction = DIRN_STOP;

void set_current_direction(MotorDirection direction){
    current_direction=direction;
}

MotorDirection get_current_direction(){
    return current_direction;
}

int queue_any_orders(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1 ){
                printf("%s", "any orders true\n\r");
                return 1;
            }    
        }
    }
    return 0;
}



int queue_any_orders_above(int current_floor){
    for(int f = current_floor; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b]==1){
                return 1;
            } 
        }
    }
    return 0;
}

int queue_any_orders_below(int current_floor){
    for(int f = 0; f < current_floor; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b]==1){
                return 1;
            } 
        }  
    }
    return 0;
}