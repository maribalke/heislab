#include "queue.h"
#include "driver/elevio.h"

#include <stdio.h>

int queue[N_FLOORS][N_BUTTONS];

void queue_set_initial_condition(){
    queue_delete_all_orders();

    int floor = elevio_floorSensor();
    while(floor == -1){
        floor = elevio_floorSensor();
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
}


void queue_add_order(){
    for(int f = 0; f < N_FLOORS; ++f) {
        for(int b = 0; b < N_BUTTONS; ++b) {
            int btnPressed = elevio_callButton(f,b);
            if(btnPressed == 1) {
                queue[f][b] = btnPressed;
                elevio_buttonLamp(f, b, 1);
            }
        }
    }
}



int queue_any_orders(){
    for(int f = 0; f < N_FLOORS; ++f){
        for(int b = 0; b < N_BUTTONS; ++b){
            if(queue[f][b] != 0){
                return 1;
            }
        }
    }
    return 0;
}


int queue_get_floor() {
    for(int f = 0; f < N_FLOORS; ++f){
        for(int b = 0; b < N_BUTTONS; ++b){
            if(queue[f][b] == 1){
                return f;
            }
        }
    }
    return 0;
}




void queue_delete_order(int f){
    for(int b = 0; b < N_BUTTONS; b++){
        queue[f][b] = 0;
        elevio_buttonLamp(f, b, 0);
    }  
}


void queue_delete_all_orders(){
    for(int f = 0; f < N_FLOORS; f++){
        for (int b = 0; b < N_BUTTONS; b++){
            queue[f][b] = 0;
            elevio_buttonLamp(f, b, 0);
        }
    }
}