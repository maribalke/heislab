package requests

import (
	"Project/elevio"
)

func OnRequestButtonPress(f int, b elevio.ButtonType) {
	var e Elevator //spør

	switch e.Behaviour { //ElevatorBehaviour.Behaviour

	case EB_DoorOpen:

		if ShouldClearImmediately(e,f,b) {
			println("Hei") // start timer
		} else {
			e.Requests[f][b] = true
		}

	case EB_Moving:
		e.Requests[f][b] = true

	case EB_Idle:
		e.Requests[f][b] = true
		pair := ChooseDirection(e)
		e.Dirn = pair.Dirn
		e.Behaviour = pair.Behaviour

		switch pair.Behaviour {
		case EB_DoorOpen:
			elevio.SetDoorOpenLamp(true) // la inn timer i denne, må testes
			e = ClearAtCurrentFloor(e)

		case EB_Moving:
			elevio.SetMotorDirection(e.Dirn)

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
		if ShouldStop(e) {
			elevio.SetMotorDirection(elevio.MD_Stop)
			elevio.SetDoorOpenLamp(true)
			e = ClearAtCurrentFloor(e)
			// setAllButtonsLights
			e.Behaviour = EB_DoorOpen
		}
	}
}

