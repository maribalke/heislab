package requests

import (
	"Driver-go/elevio"
	"Driver-go/fsm"
)

type dirnBehaviourPair struct {
	dirn elevio.MotorDirection
	behaviour fsm.ElevatorBehaviour
}

func requests_Above(e fsm.Elevator) bool {
	for f:= e.Floor+1; f < elevio.NUM_FLOORS; f++ {
		for b:= 0; b < elevio.NUM_BUTTONS; b++ {
			if e.Requests[f][b] {
				return true
			}
		}
	}
	return false
}

func requests_Below(e fsm.Elevator) bool {
	for f:= 0; f < e.Floor; f++ {
		for b:= 0; b < elevio.NUM_BUTTONS; b++ {
			if e.Requests[f][b] {
				return true
			}
		}
	}
	return false
}

func requests_here(e fsm.Elevator) bool{
    for b:= 0; b < elevio.NUM_BUTTONS; b++{
        if e.Requests[e.Floor][b] {
            return true
        }
    }
    return false
}

func ShouldClearImmediately(e fsm.Elevator, f int, b elevio.ButtonType) bool {
	if 
		(e.Floor == f && 
		((e.Dirn == elevio.MD_Up && b == elevio.BT_HallUp) ||
		(e.Dirn == elevio.MD_Down && b == elevio.BT_HallDown) ||
		e.Dirn == elevio.MD_Stop ||
		b == elevio.BT_Cab)) {
			return true
		} else { return false}
}

func chooseDirection(e fsm.Elevator) {
	switch e.Dirn {
	case elevio.MD_Up:
		return // fortsett her:)
		
	}
}
