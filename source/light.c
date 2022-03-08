#include "light.h"


void floor_light(int floor){
    if(floor != -1){
        elevio_floorIndicator(floor);
    }
}


void order_light(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(elevio_callButton(f, b) == 1){
                elevio_buttonLamp(f, b, 1);
            }
        }
    }
}

