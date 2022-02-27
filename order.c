#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"
#include "order.h"
#include "light.h"
#include "timer.h"

void set_initial_condition(){
    //printf("floor: %d \n",floor);
    int floor = elevio_floorSensor();
    while(floor == -1){
        floor = elevio_floorSensor();
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
}

void set_current_floor(int* current_floor){
    if(elevio_floorSensor() != -1){
        *current_floor = elevio_floorSensor();
    }
}


void add_order(){
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            if(elevio_callButton(i, j) == 1){
                queue[i][j] = 1;
            }
        }
    }
}

void cab_order(int floor, int current_floor){
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP);
    } 
}


void floor_order(int floor, int current_floor){
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP);
    } 
    else{
        cab_order(floor, current_floor);
    }
    
}


void delete_order(int f, int b){
    queue[f][b] = 0;    
}


void go(int current_floor){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                if(elevio_floorSensor() != f){
                    floor_order(f, current_floor);
                }
                else{
                    elevio_motorDirection(DIRN_STOP);

                    // timer_start();
                    // //elevio_doorOpenLamp(1); //timer fungerer med while, men da er vi stuck i den
                    // while(timer_expired() == 0){ //begynner ikke å telle
                    //     elevio_doorOpenLamp(1);
                    // }
                    // elevio_doorOpenLamp(0);
                  
                    delete_order(f,b);
                }
            }
        }
    }
}   


