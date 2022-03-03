#include "fsm.h"

state current_state = INIT;




int fsm_valid_stop(){

    int f = get_floor_request();
    MotorDirection current_direction = get_current_direction();

    if((queue[f][current_direction] == 1 || queue[f][2] == 1) && elevio_floorSensor() != -1){
        return 1;
    } 
        return 0;
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



void fsm_run(int current_floor){

    int floor_request = get_floor_request();
    
    switch(current_state){

        case(INIT):
            set_initial_condition();
            current_state = DOOR_OPEN;
            break;

        case(IDLE):
            add_order();
            order_light(); 
            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
            }

            if(queue_any_orders() != 0 && timer_expired()){
                current_state = MOVING;
            }

            if(timer_expired()){
                elevio_doorOpenLamp(0);
            }

            break;



        case(MOVING):

            add_order();

            order_light(); 
            

            if(get_current_direction() == DIRN_STOP){
                elevator_direction(floor_request, current_floor); 
            }

            if(elevio_floorSensor() == floor_request){
                current_state = DOOR_OPEN;
            }



            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
                set_current_direction(DIRN_STOP);
            }
            break;


        case (DOOR_OPEN):
            add_order();
            

            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
            }

            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(0);
            //printf("Door open\n");
            int f = get_floor_request();
            int b = get_btn_request();
            delete_order(f, b);
            delete_light(f, b);
            

            current_state = IDLE;
            timer_start(); //bruker lang tid
            break;


        case(EMERGENCY_STOP):
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);
            delete_all_orders();
            

            if(current_floor == floor_request){
                if(elevio_stopButton() == 0){
                    elevio_stopLamp(0);
                    current_state = DOOR_OPEN;
                }
            } else{
                while(elevio_stopButton() == 1){
                    current_state = IDLE;
                    elevio_stopLamp(0);

                }
            }


            break;
    }
}