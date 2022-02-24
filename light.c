#include "light.h"

void floor_light(int floor){
    if(floor != -1){
        elevio_floorIndicator(floor);
    }

}

void order_light(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            elevio_buttonLamp(f, b, btnPressed);
        }
    }
}

void stop_lamp(){
    while(elevio_stopButton()){
        elevio_stopLamp(1);
    }
    elevio_stopLamp(0);       
}