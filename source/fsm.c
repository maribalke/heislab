#include "fsm.h"

state current_state = INIT;

direction current_direction = DIRECTION_TBT;

direction previous_direction = DIRECTION_TBT;

position current_position; // kan vær (-1)

position newest_floor_position; // kan bare være (0,..,3)

position previous_floor_position; // kan bare være (0, .., 3)

position next_stop;

int joker = 0;

void set_initial_condition(){
    //printf("floor: %d \n",floor);
    delete_all_orders();
    delete_all_lights();
    elevio_stopLamp(0);

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

void fsm_update_floor_position()
{
    current_position = elevio_floorSensor();
    if (current_position != -1)
    {
        previous_floor_position = newest_floor_position;
        newest_floor_position = current_position;
    }
}



void fsm_go_to(int next_stop)
{
    if (next_stop > newest_floor_position)
    {
        elevio_motorDirection(DIRN_UP);
    }
    if (next_stop < newest_floor_position)
    {
        elevio_motorDirection(DIRN_DOWN);
    }

    if (next_stop == newest_floor_position && current_position == -1 && previous_direction == DIRECTION_UP) {
        elevio_motorDirection(DIRN_DOWN);
        current_direction = DIRECTION_DOWN;
        next_stop = newest_floor_position + 1;
    }
    if (next_stop == newest_floor_position && current_position == -1 && previous_direction == DIRECTION_DOWN) {
        elevio_motorDirection(DIRN_UP);
        current_direction = DIRECTION_UP;
        next_stop = newest_floor_position - 1;
    }

    //LAGT til av oskar for å løse problem 1
    

}

//finner retningen 
void fsm_find_directon()
{
    next_stop = queue_find_entry();
    if (next_stop > newest_floor_position)
    {
        current_direction = DIRECTION_UP;
    }
    if (next_stop < newest_floor_position) 
    {
        current_direction = DIRECTION_DOWN;
    }
    if (next_stop == newest_floor_position)
    {
        current_direction = DIRECTION_TBT;
    }
}

void fsm_search_beyond_next_stop()
{
    // hvis direction oppover, lop fra next_stop  og oppver, leter etter nedover-pil og kryss
    if (current_direction == DIRECTION_UP)
    {
        for (int f = next_stop + 1; f < NUMBER_OF_FLOORS; ++f)
        {

            if (queue[f][1] == 1)
            { // funnet en down
                next_stop = f;
            }
            if (queue[f][2] == 1)
            { // funnet en kryss
                next_stop = f;
            }
        }

        // hvis direction nedover, loop fra next_stop og nedover, leter etter oppover-pil og kryss

        if (current_direction == DIRECTION_DOWN)
        {
            for (int f = 0; f < next_stop; ++f)
            {
                if (queue[f][0] == 1)
                { // funnet en up
                    next_stop = f;
                }

                if (queue[f][2] == 1)
                { // funnet en kryss
                    next_stop = f;
                }
            }
        }
    }
}

int fsm_valid_stop()
{
    if ((queue[newest_floor_position][current_direction] == 1 || queue[newest_floor_position][2] == 1) && current_position != -1)
    {
        return 1;
    }
    return 0;
}

// Main run fsm_search_beyond_next_stop


void fsm_run()
{
    
    if (current_direction == DIRECTION_DOWN) {
        printf("GOING DOWN");
    }
    if (current_direction == DIRECTION_UP) {
        printf("GOING UP");
    }
    if (current_state == IDLE) {
        printf("IDLE");
    }
    if (current_state == MOVING) {
        printf("MOVING");
    }
    if (current_state == DOOR_OPEN) {
        printf("DOOR OPEN");
    }
    if (current_state == EMERGENCY_STOP) {
        printf("EMERGENCY_STOP");
    }
    

    printf("%d\n", next_stop);

    // Updates for every run of loop
    fsm_update_floor_position(); // updated {current_position, newest_floor_position, previous_floor_position}
    
    //printf("Current newest flore: %d\n",newest_floor_position);

    switch (current_state)
    {


    case (INIT):
        set_initial_condition();
        current_state = DOOR_OPEN;
        break;

    case (IDLE):
        add_order();
        order_light(); 
        elevio_stopLamp(0);

        if(elevio_stopButton()){
            current_state = EMERGENCY_STOP;
        }

        if(elevio_obstruction()){
            current_state = OBSTRUCTION;
        } 

        if(queue_any_orders() != 0 && timer_expired() && !elevio_obstruction()){
            current_state = MOVING;
        }

        if(timer_expired()){
            elevio_doorOpenLamp(0);
        }

        break;

    case (MOVING):

        add_order();
        
        // Hvis ikke er bestemt
        if (current_direction == DIRECTION_TBT)
        {
            fsm_find_directon(); // gir også "første next_stop"
        }

        // Hvis vi er der vi skal være(ende stop) (et ende stelevio_stopButtonopp, vurdering å sette direction til Undetirmand)
        
        if (newest_floor_position == next_stop && current_position != -1) //lagt til en && av oskar
        {
            current_state = DOOR_OPEN;
            current_direction = DIRECTION_TBT;
        }

        // hvis bestilling over - motsatt retning eller kryss, oppdater next_stop
        fsm_search_beyond_next_stop();

        // Hvis bestilling inni samme retning.-> newest_floor_position har et kryss i kømatrisen eller retning retning samme retning- Kast opp

        if (fsm_valid_stop())
        {
            current_state = DOOR_OPEN;
        }

        fsm_go_to(next_stop);

        if(elevio_stopButton()){
            current_state = EMERGENCY_STOP;
            previous_direction = current_direction;
            current_direction = DIRECTION_TBT;
        }

        break;

    case (DOOR_OPEN):
        add_order();
        order_light();
            
        if(elevio_stopButton()){
            current_state = EMERGENCY_STOP;
        } 

        if(elevio_obstruction()){
            current_state = OBSTRUCTION;
        } 
   
        elevio_motorDirection(DIRN_STOP);
        elevio_doorOpenLamp(0);
        
        int f = elevio_floorSensor();
        delete_order(f);
        delete_light(f);

        current_state = IDLE;
        timer_start(); 
        
        break;
        
    
    case(EMERGENCY_STOP):
        elevio_motorDirection(DIRN_STOP);
        delete_all_orders();
        

        if(elevio_stopButton() ){
                elevio_doorOpenLamp(1);
        }
        

        if(!elevio_stopButton() && elevio_floorSensor() != -1)
        {
            current_state = DOOR_OPEN;
        } else{
            current_state = IDLE;
        }  
        
        break; 

    case(OBSTRUCTION):
        add_order();
        order_light();

        elevio_doorOpenLamp(1);
        elevio_stopLamp(0);

        if(!elevio_obstruction())
        {
            current_state = DOOR_OPEN;
        }   

        break;
        
    }
    
}

