package main

// bruk package main på alle filer og bare legg til filene

import (
	"Project/elevio"
	"Project/requests"
	"Project/watchdog"

	//"fmt"
	"log"
	"time"
)

func main() {

	numFloors := 4
	numButtons := 3

	var elevator requests.Elevator

	elevio.Init("localhost:15657", numFloors)
	// requests.DeleteAllLights(numFloors, numButtons)
	// elevio.SetDoorOpenLamp(false)
	// elevio.SetStopLamp(false)
	// requests.ClearAllOrders(numFloors, numButtons, elevator)
	requests.Initialize(elevator, numFloors, numButtons)

	var direction elevio.MotorDirection = elevio.MD_Stop
	// elevio.SetMotorDirection(direction)

	drv_buttons := make(chan elevio.ButtonEvent)
	drv_floors := make(chan int)
	drv_obstr := make(chan bool)
	drv_stop := make(chan bool)
	ch_nextOrder := make(chan bool, 1)

	dog := watchdog.New(3 * time.Second)

	go elevio.PollButtons(drv_buttons)
	go elevio.PollFloorSensor(drv_floors)
	go elevio.PollObstructionSwitch(drv_obstr)
	go elevio.PollStopButton(drv_stop)

	for {
		select {

		// modify internal state orderMatrix
		// set upouts:
		// - doorLight

		case <-dog.Event():
			dog.Stop()
			elevio.SetDoorOpenLamp(false)

			ch_nextOrder <- true

		case <-ch_nextOrder:
			requests.ChooseDirection(elevator)
			pair := requests.ChooseDirection(elevator)
			elevator.Dirn = pair.Dirn
			elevator.Behaviour = pair.Behaviour
			elevio.SetMotorDirection(elevator.Dirn)
			println("new dir: ", elevator.Dirn)
			println("behaviour: ", elevator.Behaviour)

		case newButtonPress := <-drv_buttons:
			b := newButtonPress.Button
			f := newButtonPress.Floor

			switch elevator.Behaviour {

			case requests.EB_DoorOpen:

				if requests.ShouldClearImmediately(elevator, f, b) {
					elevio.SetDoorOpenLamp(true)
					elevio.SetButtonLamp(b, f, false)
					//dog.Reset(3 * time.Second)
					dog.Stop() // SPØR OM RESETEN
					go dog.Start()

				} else {
					elevator.Requests[f][b] = true
					elevio.SetButtonLamp(b, f, true)

				}
			case requests.EB_Moving:
				elevator.Requests[f][b] = true
				elevio.SetButtonLamp(b, f, true)

			case requests.EB_Idle:
				elevator.Requests[f][b] = true
				elevio.SetButtonLamp(b, f, true)
				pair := requests.ChooseDirection(elevator)
				elevator.Dirn = pair.Dirn
				elevator.Behaviour = pair.Behaviour

				println("new dir: ", elevator.Dirn)
				println("behaviour: ", elevator.Behaviour)

				switch pair.Behaviour {
				case requests.EB_DoorOpen:
					go dog.Start()
					elevio.SetDoorOpenLamp(true)

				case requests.EB_Moving:
					elevio.SetMotorDirection(elevator.Dirn)

				case requests.EB_Idle:
					break
				}
			}

		case newFloor := <-drv_floors:

			elevator.Floor = newFloor

			elevio.SetFloorIndicator(newFloor)

			log.Println("passing at floor", newFloor+1)

			switch elevator.Behaviour {
			case requests.EB_Moving:
				if requests.ShouldStop(elevator) {
					elevio.SetMotorDirection(elevio.MD_Stop)
					elevator = requests.ClearAtCurrentFloorInCurrentDirection(elevator)
					log.Println("floor: ", elevator.Floor)

					for b := 0; b < numButtons; b++ {
						if !elevator.Requests[elevator.Floor][b] {
							elevio.SetButtonLamp(elevio.ButtonType(b), elevator.Floor, false)
						}
					}

					go dog.Start() // blir stort problem hvis dog.start ikke blir ferdig før vi kommer inn i casen på nytt
					elevio.SetDoorOpenLamp(true)

				}
			default:
				break
			}

		case obstr := <-drv_obstr:

			if obstr {
				log.Println("obstruction")
				elevio.SetMotorDirection(elevio.MD_Stop)
			} else {
				log.Println("obstruction is over")
				elevio.SetMotorDirection(direction)
			}

		case stop := <-drv_stop:
			if stop {
				elevio.SetStopLamp(true)
				log.Println("stop")
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

//elevio.SetButtonLamp(elevio.BT_Cab, newFloor, false)

/*if elevator.Dirn == elevio.MD_Up{
	elevio.SetButtonLamp(elevio.BT_HallUp, newFloor, false)
}

if elevator.Dirn == elevio.MD_Down{
	elevio.SetButtonLamp(elevio.BT_HallDown, newFloor, false)
}*/
