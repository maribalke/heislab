package requests

import (
	"Project/elevio"
	"Project/fsm"
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

func ClearAtCurrentFloor(e fsm.Elevator) fsm.Elevator {
	switch e.ClearRequestVariant{

	case e.ClearRequestVariant.CV_All:
		for btn:=0; btn < elevio.NUM_BUTTONS; btn++ {
			e.requests[e.Floor][btn] = 0
		}
	case e.ClearRequestVariant.CV_InDirn:
		e.requests[e.Floor][elevio.BT_Cab] = 0

		switch e.dirn {
		case elevio.MD_Up:
			if !requests_Above(e) && !e.requests[e.floor][elevio.BT_HallUp] {
				e.requests[e.floor][elevio.BT_HallDown] = 0
			}
			e.requests[e.floor][elevio.BT_HallUp] = 0
		case elevio.MD_Down:
			if !requests_Below(e) && !e.requests[e.floor][elevio.BT_HallDown] {
				e.requests[e.floor][elevio.BT_HallUp] = 0
			}
			e.requests[e.floor][elevio.BT_HallUp] = 0
			e.requests[e.floor][elevio.BT_HallDown] = 0
		case elevio.MD_Stop:
		default:
			e.requests[e.floor][elevio.BT_HallUp]
			e.requests[e.floor][elevio.BT_HallDown]
		}

	}
	return e;
}

func ChooseDirection(e fsm.Elevator) dirnBehaviourPair {
	switch e.Dirn {
	case elevio.MD_Up:
		if requests_Above(e) { return dirnBehaviourPair{elevio.MD_Up,   elevio.EB_Moving} 
		} else if requests_here(e){ return dirnBehaviourPair{elevio.MD_Down,   elevio.EB_DoorOpen}
		} else if requests_Below(e){ return dirnBehaviourPair{elevio.MD_Down,   elevio.EB_Moving}
		} else {return dirnBehaviourPair {elevio.MD_Stop, elevio.EB_Idle}  }
	
	case elevio.MD_Down:
		if requests_Below(e) { return dirnBehaviourPair{elevio.MD_Down,   elevio.EB_Moving} 
		} else if requests_here(e){ return dirnBehaviourPair{elevio.MD_Up,   elevio.EB_DoorOpen}
		} else if requests_Above(e){ return dirnBehaviourPair{elevio.MD_Up,   elevio.EB_Moving}
		} else {return dirnBehaviourPair {elevio.MD_Stop, elevio.EB_Idle}  }

	case elevio.MD_Stop:
		if requests_here(e) { return dirnBehaviourPair{elevio.MD_Stop,   elevio.EB_DoorOpen} 
		} else if requests_Above(e){ return dirnBehaviourPair{elevio.MD_Up,   elevio.EB_Moving}
		} else if requests_Below(e){ return dirnBehaviourPair{elevio.MD_Down,   elevio.EB_Moving}
		} else {return dirnBehaviourPair {elevio.MD_Stop, elevio.EB_Idle}  }
		
	default: return dirnBehaviourPair {elevio.MD_Stop,elevio.EB_Idle}
	}
}

func ShouldStop(e fsm.Elevator) int{
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
	default:
		return 1
	}
}