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

void cab_order(int floor, int current_floor){
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        set_current_direction(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP);
        set_current_direction(DIRN_UP);
    } 
}


void floor_order(int floor, int current_floor){
    
    if(current_floor > floor){
        elevio_motorDirection(DIRN_DOWN);
        set_current_direction(DIRN_DOWN);
        }
    else if(current_floor < floor){
        elevio_motorDirection(DIRN_UP);
        set_current_direction(DIRN_UP);
    } 
    else{
        cab_order(floor, current_floor);
    }
    
}


void delete_order(int f, int b){
    queue[f][b] = 0;    
}



void queue_orders(int floor, int current_floor){
    switch(get_current_direction())
    {
    case DIRN_DOWN:
        if(queue_any_orders_below(current_floor) == 1){
            floor_order(floor, current_floor);
        }
        break;

    case DIRN_UP:
        if(queue_any_orders_above(current_floor) == 1){
            floor_order(floor, current_floor);
        }
        break;

    case DIRN_STOP:
    printf("%s", "case stop\n\r");
        if(queue_any_orders() == 0){
            elevio_motorDirection(DIRN_STOP);
        } else{
            printf("%s","elsecase\n\r");
            floor_order(floor, current_floor);
        }
        break;

    default:
        //elevio_motorDirection(DIRN_STOP);
        break;
    }
}


void go(int current_floor){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                if(elevio_floorSensor() != f){
                    // if(queue_any_orders_above(current_floor) == 1 && queue_any_orders_below(current_floor) == 1){
                    //     queue_orders(f, current_floor);
                    // }
                    // else{
                    //     floor_order(f, current_floor);
                    // }
                    queue_orders(f, current_floor);
                    
                }
                else{
                    elevio_motorDirection(DIRN_STOP);
                    delete_light(f, b);
                    //delete_light();
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


