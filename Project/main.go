package main

// bruk package main på alle filer og bare legg til filene

import (
	"Project/elevio"
	"Project/requests"
	"fmt"
)

func main() {

	numFloors := 4

	elevio.Init("localhost:15657",
		numFloors)
	elevio.SetDoorOpenLamp(false)
	elevio.SetStopLamp(false)

	var direction elevio.MotorDirection = elevio.MD_Stop
	elevio.SetMotorDirection(direction)

	var elevator requests.Elevator

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

		// modify internal state orderMatrix
		// set upouts:
		// - doorLight

		case newButtonPress := <-drv_buttons:
			b := newButtonPress.Button
			f := newButtonPress.Floor

			switch elevator.Behaviour { //ElevatorBehaviour.Behaviour

			case requests.EB_DoorOpen:
				print("kun door open\n")
				// clear orderMatrix
				if requests.ShouldClearImmediately(elevator, f, b) {
					//print("før sleep\n")
					// må åpne dører i tre sek og så lukke
					// time.Sleep(time.Duration(3) * time.Second)
					//print("sleep over\n")
					elevator = requests.ClearAtCurrentFloorInCurrentDirection(elevator)
					elevio.SetDoorOpenLamp(false)
					elevator.Behaviour = requests.EB_Idle

				} else {
					// set orderMatrix
					elevator.Requests[f][b] = true
					//print("Hei")
				}

			// set orderMatrix
			case requests.EB_Moving:
				print("moving")
				elevator.Requests[f][b] = true

			// set orderMatrix
			// set doorLamp
			// set motorDirection
			//
			case requests.EB_Idle:
				print("idle\n")
				elevator.Requests[f][b] = true
				pair := requests.ChooseDirection(elevator)
				//print("tilstand ",pair.Behaviour,"\n")
				elevator.Dirn = pair.Dirn
				elevator.Behaviour = pair.Behaviour

				switch pair.Behaviour {
				case requests.EB_DoorOpen:
					print("idle så door open\n")
					elevio.SetDoorOpenLamp(true)
					elevator = requests.ClearAtCurrentFloorInCurrentDirection(elevator)

				case requests.EB_Moving:
					elevio.SetMotorDirection(elevator.Dirn)

				}
			}

		// set floorIndicator
		// set motor direction
		// set lamp
		case newFloor := <-drv_floors:

			elevator.Floor = newFloor

			elevio.SetFloorIndicator(newFloor)
			//print("t: ",e.Behaviour,"\n")
			switch elevator.Behaviour {
			case requests.EB_Moving:
				if requests.ShouldStop(elevator) {
					elevio.SetMotorDirection(elevio.MD_Stop)
					elevio.SetDoorOpenLamp(true)
					elevator = requests.ClearAtCurrentFloorInCurrentDirection(elevator)
					// setAllButtonsLights
					elevator.Behaviour = requests.EB_DoorOpen
				}
			default:
				break
			}

		case obstr := <-drv_obstr:

			if obstr {
				fmt.Print("obstruction\n")
				elevio.SetMotorDirection(elevio.MD_Stop)
			} else {
				elevio.SetMotorDirection(direction)
			}

		case stop := <-drv_stop:
			if stop {
				elevio.SetStopLamp(true)
				fmt.Print("stop\n")
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
