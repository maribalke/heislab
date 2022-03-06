#include "queue.h"
#include "driver/elevio.h"

#include <stdio.h>

static MotorDirection current_direction = DIRN_STOP;

int queue[NUMBER_OF_FLOORS][NUMBER_OF_BUTTONS];


void add_order(){
    for(int f = 0; f < NUMBER_OF_FLOORS; ++f) {
        for(int b = 0; b < NUMBER_OF_BUTTONS; ++b) {
            int btnPressed = elevio_callButton(f,b);//f is floor and b is buttontype [up, down, hb]
            if(btnPressed == 1) {
                queue[f][b] = btnPressed;
                elevio_buttonLamp(f, b, 1);
            }
        }
    }
}



int queue_any_orders(){
    for(int f = 0; f < NUMBER_OF_FLOORS; ++f){
        for(int b = 0; b < NUMBER_OF_BUTTONS; ++b){
            if(queue[f][b] != 0){
                return 1;
            }
        }
    }
    return 0;
}


int queue_find_entry() {
    for(int f = 0; f < NUMBER_OF_FLOORS; ++f){
        for(int b = 0; b < NUMBER_OF_BUTTONS; ++b){
            if(queue[f][b] == 1){
                return f;
            }
        }
    }
    //return 0;
}




void delete_order(int f){
    for(int b = 0; b < N_BUTTONS; b++){
        queue[f][b] = 0;
    }  
}


void delete_all_orders(){
    for(int f = 0; f < N_FLOORS; f++){
        for (int b = 0; b < N_BUTTONS; b++){
            queue[f][b] = 0;
        }
    }
}