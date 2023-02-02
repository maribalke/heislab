package fsm

import (
	"Driver-go/elevio"
)

type ElevatorBehaviour string

const (
	EB_Idle = iota
	EB_Moving
	EB_DoorOpen
)

type ClearRequestVariant int

const (
	CV_All = iota
	CV_InDirn
)

type Elevator struct {
	Floor    int
	Dirn     elevio.MotorDirection
	Requests [elevio.NUM_FLOORS][elevio.NUM_BUTTONS] bool
	Behaviour ElevatorBehaviour

	ClearVariant ClearRequestVariant 
}


