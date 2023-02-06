package requests

import (
	"Project/elevio"
)

type dirnBehaviourPair struct {
	Dirn      elevio.MotorDirection
	Behaviour ElevatorBehaviour
}

func requests_Above(e Elevator) bool {
	for f := e.Floor + 1; f < elevio.NUM_FLOORS; f++ {
		for b := 0; b < elevio.NUM_BUTTONS; b++ {
			if e.Requests[f][b] {
				return true
			}
		}
	}
	return false
}

func requests_Below(e Elevator) bool {
	for f := 0; f < e.Floor; f++ {
		for b := 0; b < elevio.NUM_BUTTONS; b++ {
			if e.Requests[f][b] {
				return true
			}
		}
	}
	return false
}

func requests_here(e Elevator) bool {
	for b := 0; b < elevio.NUM_BUTTONS; b++ {
		if e.Requests[e.Floor][b] {
			return true
		}
	}
	return false
}

func ShouldClearImmediately(e Elevator, f int, b elevio.ButtonType) bool {
	if e.Floor == f &&
		((e.Dirn == elevio.MD_Up && b == elevio.BT_HallUp) ||
			(e.Dirn == elevio.MD_Down && b == elevio.BT_HallDown) ||
			e.Dirn == elevio.MD_Stop ||
			b == elevio.BT_Cab) {
		return true
	} else {
		return false
	}
}

func ClearAtCurrentFloor(e Elevator) Elevator {

	switch e.ClearVariant {

	case CV_All:
		for btn := 0; btn < elevio.NUM_BUTTONS; btn++ {
			e.Requests[e.Floor][btn] = false
		}
	case CV_InDirn:
		e.Requests[e.Floor][elevio.BT_Cab] = false

		switch e.Dirn {
		case elevio.MD_Up:
			if !requests_Above(e) && !e.Requests[e.Floor][elevio.BT_HallUp] {
				e.Requests[e.Floor][elevio.BT_HallDown] = false
			}
			e.Requests[e.Floor][elevio.BT_HallUp] = false
		case elevio.MD_Down:
			if !requests_Below(e) && !e.Requests[e.Floor][elevio.BT_HallDown] {
				e.Requests[e.Floor][elevio.BT_HallUp] = false
			}
			e.Requests[e.Floor][elevio.BT_HallUp] = false
			e.Requests[e.Floor][elevio.BT_HallDown] = false
		case elevio.MD_Stop:

		default:
			e.Requests[e.Floor][elevio.BT_HallUp] = false
			e.Requests[e.Floor][elevio.BT_HallDown] = false
		}
	}
	return e
}

func ChooseDirection(e Elevator) dirnBehaviourPair {
	switch e.Dirn {
	case elevio.MD_Up:
		if requests_Above(e) {
			return dirnBehaviourPair{elevio.MD_Up, EB_Moving}
		} else if requests_here(e) {
			return dirnBehaviourPair{elevio.MD_Down, EB_DoorOpen}
		} else if requests_Below(e) {
			return dirnBehaviourPair{elevio.MD_Down, EB_Moving}
		} else {
			return dirnBehaviourPair{elevio.MD_Stop, EB_Idle}
		}

	case elevio.MD_Down:
		if requests_Below(e) {
			return dirnBehaviourPair{elevio.MD_Down, EB_Moving}
		} else if requests_here(e) {
			return dirnBehaviourPair{elevio.MD_Up, EB_DoorOpen}
		} else if requests_Above(e) {
			return dirnBehaviourPair{elevio.MD_Up, EB_Moving}
		} else {
			return dirnBehaviourPair{elevio.MD_Stop, EB_Idle}
		}

	case elevio.MD_Stop:
		if requests_here(e) {
			return dirnBehaviourPair{elevio.MD_Stop, EB_DoorOpen}
		} else if requests_Above(e) {
			return dirnBehaviourPair{elevio.MD_Up, EB_Moving}
		} else if requests_Below(e) {
			return dirnBehaviourPair{elevio.MD_Down, EB_Moving}
		} else {
			return dirnBehaviourPair{elevio.MD_Stop, EB_Idle}
		}

	default:
		return dirnBehaviourPair{elevio.MD_Stop, EB_Idle}
	}
}

func ShouldStop(e Elevator) bool {
	switch e.Dirn {
	case elevio.MD_Down:
		return e.Requests[e.Floor][elevio.BT_HallDown] ||
			e.Requests[e.Floor][elevio.BT_Cab] ||
			!requests_Below(e)

	case elevio.MD_Up:
		return e.Requests[e.Floor][elevio.BT_HallUp] ||
			e.Requests[e.Floor][elevio.BT_Cab] ||
			!requests_Above(e)

	case elevio.MD_Stop:
		return true

	default:
		return true
	}
}
