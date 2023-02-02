package fsm

import (
	"Project/elevio"
	"Project/requests"
)


func onRequestButtonPress(f int, b elevio.ButtonType) {
	var e Elevator //spør
	
	switch e.Behaviour { //ElevatorBehaviour.Behaviour

	case fsm.EB_DoorOpen:
		
		if requests.ShouldClearImmediately {println("Hei") // start timer 
		} else {
			e.Requests[f][b] = 1}

		case fsm.EB_Moving:
			e.Requests[f][b] = 1

		case fsm.EB_Idle:
			e.Requests[f][b] = 1
			pair := requests.ChooseDirection(e)
			e.dirn = pair.dirn
			e.behaviour = pair.behaviour

			switch pair.behaviour {
			case fsm.EB_DoorOpen:
				elevio.SetDoorOpenLamp(1) // la inn timer i denne, må testes
				e = requests.ClearAtCurrentFloor(e)

			case fsm.EB_Moving:
				elevio.SetMotorDirection(e.dirn) // OBS forstår ikke hva som skal gjøres her, spør studass
			
			case fsm.EB_Idle:
				break
			}
	}
}

func fsm(newButtonPressChan <-chan elevio.ButtonEvent, floorSensorChan <-chan int){ // legg til timer etterhvert
	// elevator := elevio.Elevator
	for {
		select{
		case newButtonPress := <-newButtonPressChan:
			onRequestButtonPress(newButtonPress.Floor, newButtonPress.Button)

		case newFloor := <-floorSensorChan:


		}
	}
}