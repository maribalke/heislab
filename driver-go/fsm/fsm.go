package fsm

import "Driver-go/elevio"

func onRequestButtonPress(f int, b elevio.ButtonType) {

	switch ElevatorBehaviour.Behaviour {

	case EB_DoorOpen:
		
		if ShouldClearImmediately {println("Hei") // start timer 
		} else {
			Elevator.Requests[f][b] = 1}

		case EB_Moving:
			Elevator.Requests[f][b] = 1

		case EB_Idle:
			Elevator.Requests[f][b] = 1

		}
}

func fsm(newButtonPressChan <-chan elevio.ButtonEvent, floorSensorChan <-chan int){ // legg til timer etterhvert
	elevator := elevio.Elevator
	for {
		select{
		case newButtonPress := <-newButtonPressChan:
			onRequestButtonPress()

			case newFloor := <-floorSensorChan


		}
	}
}