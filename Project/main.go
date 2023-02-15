package main

// bruk package main på alle filer og bare legg til filene

import (
	"Project/elevio"
	"Project/requests"
	"Project/watchdog"
	"log"
	"time"
)

func main() {

	numFloors := 4

	elevio.Init("localhost:15657", numFloors)
	elevio.SetDoorOpenLamp(false)
	elevio.SetStopLamp(false)

	var direction elevio.MotorDirection = elevio.MD_Stop
	elevio.SetMotorDirection(direction)

	var elevator requests.Elevator

	drv_buttons := make(chan elevio.ButtonEvent)
	drv_floors := make(chan int)
	drv_obstr := make(chan bool)
	drv_stop := make(chan bool)
	//timeOut := make(chan bool, 1)

	dog := watchdog.New(3 * time.Second)

	go elevio.PollButtons(drv_buttons)
	go elevio.PollFloorSensor(drv_floors)
	go elevio.PollObstructionSwitch(drv_obstr)
	go elevio.PollStopButton(drv_stop)

	// husk å lag en chan som sørger for å gjøre ting når vi fortsatt har lagret bestillinger

	for {
		select {

		// modify internal state orderMatrix
		// set upouts:
		// - doorLight

		case <-dog.Event():
			elevio.SetDoorOpenLamp(false)
			elevator = requests.ClearAtCurrentFloorInCurrentDirection(elevator)
			// setAllButtonsLights
			elevator.Behaviour = requests.EB_Idle
			log.Println("door closed")
			dog.Stop()

		case newButtonPress := <-drv_buttons:
			b := newButtonPress.Button
			f := newButtonPress.Floor

			switch elevator.Behaviour { //ElevatorBehaviour.Behaviour

			case requests.EB_DoorOpen:

				if requests.ShouldClearImmediately(elevator, f, b) {
					dog.Reset(3 * time.Second)
					elevio.SetDoorOpenLamp(true)

					elevator.Behaviour = requests.EB_Idle // usikker på denne

				} else {
					// set orderMatrix
					elevator.Requests[f][b] = true
				}
			// set orderMatrix
			case requests.EB_Moving:
				elevator.Requests[f][b] = true

			// set orderMatrix
			// set doorLamp
			// set motorDirection
			//
			case requests.EB_Idle:
				elevator.Requests[f][b] = true
				pair := requests.ChooseDirection(elevator)
				elevator.Dirn = pair.Dirn
				elevator.Behaviour = pair.Behaviour

				log.Println("etter idle: ", elevator.Behaviour)

				switch pair.Behaviour {
				case requests.EB_DoorOpen:
					log.Println("door open")
					go dog.Start()
					elevio.SetDoorOpenLamp(true)

				case requests.EB_Moving:
					elevio.SetMotorDirection(elevator.Dirn)
					log.Println("elevator moving")

				case requests.EB_Idle:
					break
				}
			}

		// set floorIndicator
		// set motor direction
		// set lamp
		case newFloor := <-drv_floors:

			elevator.Floor = newFloor

			elevio.SetFloorIndicator(newFloor)

			log.Println("passing at floor", newFloor+1)

			switch elevator.Behaviour {
			case requests.EB_Moving:
				if requests.ShouldStop(elevator) {
					log.Println("elevator stopped at floor")
					elevio.SetMotorDirection(elevio.MD_Stop)
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
