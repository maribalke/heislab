case(MOVING):
            //printf("Moving\n");

            add_order();
            order_light(); 
            

            if(get_current_direction() == DIRN_STOP){
                elevator_direction(floor_request, current_floor); 
            }

            if(elevio_floorSensor() != -1){
                printf("Floor\n");
                
                //set_current_direction(DIRN_DOWN);
                switch (get_current_direction())
                {
                case DIRN_DOWN:
                    printf("dir Down\n");
                    for(int f = 0; f < current_floor; ++f){
                        if((queue[f][1] == 1) || (queue[f][2] == 1)){
                            printf("løkke\n");
                            if(elevio_floorSensor() == f){
                                printf("door open\n");
                                current_state = DOOR_OPEN;
                            }
                        }
                    }
                    
                    break;

                case DIRN_UP:
                    printf("dir up\n");
                    for(int f = current_floor; f < N_FLOORS; ++f){
                        if((queue[f][0] == 1) || (queue[f][2] == 1)){
                            if(elevio_floorSensor() == f){
                                current_state = DOOR_OPEN;
                            }
                        }
                    }
                    break;

                // default:
                //     printf("break\n");
                //     break;
                
                }
            }

            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
                set_current_direction(DIRN_STOP);
            }
            break;