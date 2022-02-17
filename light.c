#include "light.h"

void floor_light(int floor){
    if(floor != -1){
        elevio_floorIndicator(floor);
    }

}

void stop_lamp(){
    while(elevio_stopButton()){
        elevio_stopLamp(1);
    }
    elevio_stopLamp(0);
            
}