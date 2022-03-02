#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"
#include "order.h"
#include "light.h"
#include "timer.h"
#include "queue.h"

// void undefined_floor(){
//     int floor = elevio_floorSensor();
// }

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
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(elevio_callButton(f, b) == 1){
                queue[f][b] = 1;
            }
        }
    }
}

int get_floor_request(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                return f;
            }
        }
    }
    return 0;
}

int get_btn_request(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                return b;
            }
        }
    }
    return 0;
}



void cab_direction(int floor, int current_floor){
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP); 
    } 
}


void elevator_direction(int floor, int current_floor){
    
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP);
    } 
    else{
        cab_direction(floor, current_floor);
    }
    
}


void delete_order(int f, int b){
    queue[f][b] = 0;    
}


void delete_all_orders(){
    for(int f = 0; f < N_FLOORS; f++){
        for (int b = 0; b < N_BUTTONS; b++){
            queue[f][b] = 0;
        }
    }
}








