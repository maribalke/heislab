package fsm

import (
	"Project/elevio"
	"Project/requests"
)


func onRequestButtonPress(f int, b elevio.ButtonType) {
	var e Elevator //spør
	
	switch e.Behaviour { //ElevatorBehaviour.Behaviour

	case EB_DoorOpen:
		
		if requests.ShouldClearImmediately {println("Hei") // start timer 
		} else {
			e.Requests[f][b] = 1}

	case EB_Moving:
			e.Requests[f][b] = 1

	case EB_Idle:
			e.Requests[f][b] = 1
			pair := requests.ChooseDirection(e)
			e.dirn = pair.dirn
			e.behaviour = pair.behaviour

		switch pair.behaviour {
			case EB_DoorOpen:
				elevio.SetDoorOpenLamp(true) // la inn timer i denne, må testes
				e = requests.ClearAtCurrentFloor(e)

			case EB_Moving:
				elevio.SetMotorDirection(e.dirn) 
			
			case EB_Idle:
				break
			}
	}
}

func OnFloorArrival(newFloor int) {
	var e Elevator
	e.Floor = newFloor

	elevio.SetFloorIndicator(newFloor)

	switch e.Behaviour {
	case EB_Moving:
		if requests.ShouldStop(e) {
			elevio.SetMotorDirection(elevio.MD_Stop)
			elevio.SetDoorOpenLamp(true)
			e = requests.ClearAtCurrentFloor(e)
			// setAllButtonsLights
			e.Behaviour = EB_DoorOpen
		}
	}
}

func StateMachine(newButtonPressChan <-chan elevio.ButtonEvent, floorSensorChan <-chan int, obstruction <-chan bool, stop <-chan bool,d elevio.MotorDirection){ // legg til timer etterhvert
	// elevator := elevio.Elevator
	for {
		select{
		case newButtonPress := <-newButtonPressChan:
			onRequestButtonPress(newButtonPress.Floor, newButtonPress.Button)

		case newFloor := <-floorSensorChan:
			prev := -1
			if newFloor != -1 && newFloor != prev { // blir ikke dette dobbelt opp av samme spørsmål?
				OnFloorArrival(newFloor)
			}
			prev = newFloor
				
		case obstr := <-obstruction:
			if obstr {
				elevio.SetMotorDirection(elevio.MD_Stop)
			} else {
				elevio.SetMotorDirection(d)
			}
		
		case s := <-stop:
			if s {
				elevio.SetStopLamp(true)
				elevio.SetMotorDirection(elevio.MD_Stop)
				for f := 0; f < elevio.NUM_FLOORS; f++ {
					for b := elevio.ButtonType(0); b < 3; b++ {
						elevio.SetButtonLamp(b, f, false)
					}
				}
				
			}
		}
	}
}