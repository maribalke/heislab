#include "fsm.h"

state current_state = INIT;

direction current_direction = DIRECTION_NONE;

direction previous_direction = DIRECTION_NONE;

position current_position; 

position last_floor_position; 

position previous_floor_position; 

position next_stop;



void fsm_update_last_floor()
{
    current_position = elevio_floorSensor();
    if (current_position != -1)
    {
        previous_floor_position = last_floor_position;
        last_floor_position = current_position;
    }
}



void fsm_go_to(int next_stop)
{
    if (next_stop > last_floor_position)
    {
        elevio_motorDirection(DIRN_UP);
    }
    if (next_stop < last_floor_position)
    {
        elevio_motorDirection(DIRN_DOWN);
    }

    if (next_stop == last_floor_position && current_position == -1 && previous_direction == DIRECTION_UP) {
        elevio_motorDirection(DIRN_DOWN);
        current_direction = DIRECTION_DOWN;
        next_stop = last_floor_position + 1;
    }
    if (next_stop == last_floor_position && current_position == -1 && previous_direction == DIRECTION_DOWN) {
        elevio_motorDirection(DIRN_UP);
        current_direction = DIRECTION_UP;
        next_stop = last_floor_position - 1;
    }
    

}


void fsm_find_directon()
{
    next_stop = queue_get_floor();
    if (next_stop > last_floor_position)
    {
        current_direction = DIRECTION_UP;
    }
    if (next_stop < last_floor_position) 
    {
        current_direction = DIRECTION_DOWN;
    }
    if (next_stop == last_floor_position)
    {
        current_direction = DIRECTION_NONE;
    }
}

void fsm_set_next_stop()
{
    if (current_direction == DIRECTION_UP)
    {
        for (int f = next_stop + 1; f < N_FLOORS; ++f)
        {

            if (queue[f][1] == 1)
            { 
                next_stop = f;
            }
            if (queue[f][2] == 1)
            { 
                next_stop = f;
            }
        }

        if (current_direction == DIRECTION_DOWN)
        {
            for (int f = 0; f < next_stop; ++f)
            {
                if (queue[f][0] == 1)
                { 
                    next_stop = f;
                }

                if (queue[f][2] == 1)
                { 
                    next_stop = f;
                }
            }
        }
    }
}

int fsm_valid_stop()
{
    for(int b = 0; b < N_BUTTONS; b++){
        if (queue[last_floor_position][b] == 1 && current_position != -1)
    {
        return 1;
    }
    }
    return 0;
}



void fsm_run()
{
    fsm_update_last_floor(); 

    switch (current_state)
    {


    case (INIT):
        queue_set_initial_condition();
        current_state = DOOR_OPEN;
        break;

    case (IDLE):
        queue_add_order();
        elevio_stopLamp(0);

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

    case (MOVING):
        queue_add_order();
        
        if (current_direction == DIRECTION_NONE)
        {
            fsm_find_directon(); 
        }

        
        if (last_floor_position == next_stop && current_position != -1) 
        {
            current_state = DOOR_OPEN;
            current_direction = DIRECTION_NONE;
        }

        fsm_set_next_stop();


        if (fsm_valid_stop())
        {
            current_state = DOOR_OPEN;
        }
        if (fsm_valid_stop() && elevio_obstruction())
        {
            current_state = OBSTRUCTION;
        }

        fsm_go_to(next_stop);


        if(elevio_stopButton()){
            current_state = EMERGENCY_STOP;
            previous_direction = current_direction;
            current_direction = DIRECTION_NONE;
        }

        break;

    case (DOOR_OPEN):
        queue_add_order();

        elevio_motorDirection(DIRN_STOP);
        
        int f = elevio_floorSensor();
        queue_delete_order(f);
            
        if(elevio_stopButton()){
            current_state = EMERGENCY_STOP;
        } 
        
        if(elevio_obstruction()){
            current_state = OBSTRUCTION;
        } 
        else{
            elevio_doorOpenLamp(0);
            current_state = IDLE;
        }

        timer_start();
        break;
        
    
    case(EMERGENCY_STOP):
        elevio_motorDirection(DIRN_STOP);
        queue_delete_all_orders();
  
        
        while(elevio_stopButton()){
            elevio_stopLamp(1);
            if(elevio_floorSensor() != -1){
                elevio_doorOpenLamp(1);
            }
        }
        elevio_stopLamp(0);

        if(elevio_floorSensor() != -1)
        {
            current_state = DOOR_OPEN;
        } else{
            current_state = IDLE;
        }  
        
    break; 

    case(OBSTRUCTION):
        while(elevio_obstruction()){
            queue_add_order();
            elevio_doorOpenLamp(1);
        }
        
        if(!elevio_obstruction())
        {
            current_state = DOOR_OPEN;
        }   

    break;
    }
}

