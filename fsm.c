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



void set_next_stop()
{   
    int floor_request = get_floor_request();
    MotorDirection current_direction = get_current_direction();
    // hvis direction oppover, lop fra next_stop  og oppver, leter etter nedover-pil og kryss
    if (current_direction == DIRN_UP)
    {
        for (int f = floor_request+1; f < N_FLOORS; ++f)
        {

            if (queue[f][1] == 1)
            { // funnet en down
                floor_request = f;
            }
            if (queue[f][2] == 1)
            { // funnet en kryss
                floor_request = f;
            }
        }

        // hvis direction nedover, loop fra next_stop og nedover, leter etter oppover-pil og kryss

        if (current_direction == DIRN_DOWN)
        {
            for (int f = 0; f < floor_request; ++f)
            {
                if (queue[f][0] == 1)
                { // funnet en up
                    floor_request = f;
                }

                if (queue[f][2] == 1)
                { // funnet en kryss
                    floor_request = f;
                }
            }
        }
    }
}



void fsm_run(int current_floor){

    int floor_request = get_floor_request();
    switch(current_state){

        case(INIT):
            set_initial_condition();
            current_state = IDLE;
            break;

        case(IDLE):
            add_order();
            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
            }

            if(queue_any_orders() != 0){
                current_state = MOVING;
            }
            break;

        case(MOVING):

            add_order();
            set_next_stop();
            order_light(); 
            
            //MotorDirection dir = get_current_direction();

            if(get_current_direction() == DIRN_STOP){
                printf("løkke1\n");
                set_next_stop();
                elevator_direction(floor_request, current_floor);
                
            }

            if(elevio_floorSensor() == floor_request){
                printf("løkke2\n");
                
                current_state = DOOR_OPEN;
                //elevio_motorDirection(DIRN_STOP); //stopper hele og vi må da legge til ny retning
            }

            
            // if(fsm_valid_stop()){
            //     current_state = DOOR_OPEN;
            // } 

            //elevator_direction(floor_request, current_floor);

            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
                set_current_direction(DIRN_STOP);
            }
            break;


        case (DOOR_OPEN):
            add_order();
            set_next_stop();

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
            //timer_start(); bruker lang tid
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