package main

// bruk package main på alle filer og bare legg til filene

import (
	"Project/elevio"
	"Project/requests"
)

func main() {

	numFloors := 4

	elevio.Init("localhost:15657", numFloors)

	var d elevio.MotorDirection = elevio.MD_Up
	elevio.SetMotorDirection(d)

	drv_buttons := make(chan elevio.ButtonEvent)
	drv_floors := make(chan int)
	drv_obstr := make(chan bool)
	drv_stop := make(chan bool)

	go elevio.PollButtons(drv_buttons)
	go elevio.PollFloorSensor(drv_floors)
	go elevio.PollObstructionSwitch(drv_obstr)
	go elevio.PollStopButton(drv_stop)
	

	for {
		select {
		case newButtonPress := <- drv_buttons:
			requests.OnRequestButtonPress(newButtonPress.Floor, newButtonPress.Button)

		case newFloor := <- drv_floors:
			prev := -1
			if newFloor != -1 && newFloor != prev { // blir ikke dette dobbelt opp av samme spørsmål?
				requests.OnFloorArrival(newFloor)
			}
			prev = newFloor

		case obstr := <- drv_obstr:
			if obstr {
				elevio.SetMotorDirection(elevio.MD_Stop)
			} else {
				elevio.SetMotorDirection(d)
			}

		case s := <- drv_stop:
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
