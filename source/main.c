#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "elevator.h"
#include "stateMachine.h"



void elevator_movement(Elevator* elev) {
    if (elev->elevatorDirection == UP) {
        elevio_motorDirection(DIRN_UP);
    } else if (elev->elevatorDirection == DOWN) {
        elevio_motorDirection(DIRN_DOWN);
    } else if (elev->elevatorDirection == NONE) {
        elevio_motorDirection(DIRN_STOP);
    }
}


void button_check(Elevator* elevator){
    for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                
                if (btnPressed) {
                    Order currentOrder = getOrder(f, b);
                    elevatorMove(elevator, currentOrder);
                }
                
                if (elevator->currentTarget == -1 | elevator->currentTarget == elevator->floor) {
                    elevator->elevatorDirection = NONE;
                } else if (elevator->currentTarget < elevator->floor) {
                    elevator->elevatorDirection = DOWN;
                } else if (elevator->currentTarget > elevator->floor) {
                    elevator->elevatorDirection = UP;
                }

                elevio_buttonLamp(f, b, btnPressed);
            }
        }
}




/// @brief 
/// @return 
int main(){

    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
    
    // Startup

    // Initialize State Machine
    enum states stateMachine = Initialize;
    enum states newState = Initialize;
    // Initiate elevator struct
    Elevator *elevator = malloc(sizeof(*elevator));
    *elevator = (Elevator) {
        .floor = -1,
        .currentTarget = -1,
        .elevatorDirection = NONE,
        .doorOpen = false,
        .stopped = false,
        .obstructed = false
    };

    elevio_motorDirection(DIRN_DOWN);

    /*
    int floor = printf("unrecognized state change %d to %d \n", currentState, newState); != -1){
            elevio_motorDirection(DIRN_STOP);
        }
    }
    */
    

    

    while(1){

        //---------------- Basic elevator functionality ------------------
        // Set floor counter
        int floor = elevio_floorSensor();
        if (floor != -1){
            elevator->floor = floor;
        }
        printf("current floor: %d\n", floor);
        //printf("floor: %d, target: %d\n",floor, elevator->currentTarget);

        // Stops and obstructs
        if(elevio_obstruction()){
            // Maybe add as its own state

        } else {
            elevio_stopLamp(0);
        }

        //---------------- Set new state ---------------------------------
        if (stateMachine == Initialize && floor == -1);
        else if (elevio_stopButton()) newState = Stop;
        else if (elevator->currentTarget == elevator->floor) {
            elevio_motorDirection(DIRN_STOP);
            newState = OpenDoor;
        } else newState = Moving;

        //---------------- Execute state change --------------------------
        enum stateChanges stateC = changeState(stateMachine, newState);
        printf("current state: %d, new state: %d\n", stateMachine, newState);
        switch(stateC){
            case(Init_OpenDoor):
                stateMachine = OpenDoor;
                elevio_motorDirection(DIRN_STOP);
                break;




            case(AtFloor_Moving):
                stateMachine = Moving;
                break;

            case(AtFloor_OpenDoor):
                stateMachine = OpenDoor;
                elevio_motorDirection(DIRN_STOP);
                break;
            
            case(AtFloor_Stop):
                stateMachine = Stop;
                elevio_motorDirection(DIRN_STOP);
                break;


            case(AtFloor_AtFloor):
                elevator_movement(elevator);
                break;
            
            case(Moving_AtFloor):
                stateMachine = AtFloor;
                break;
            
            case(Moving_OpenDoor):
                stateMachine = OpenDoor;
                elevio_motorDirection(DIRN_STOP);
                break;

            case(Moving_Stop):
                stateMachine = Stop;
                elevio_motorDirection(DIRN_STOP);
                break;




            case(OpenDoor_AtFloor):
                stateMachine = AtFloor;
                elevator_movement(elevator);
                break;

            case(OpenDoor_Stop):
                stateMachine = Stop;
                elevio_motorDirection(DIRN_STOP);
                break;



            
            case(Stop_AtFloor):
                stateMachine = AtFloor;
                break;
            
            case(Stop_Moving):
                stateMachine = Moving;
                break;

            case(Stop_OpenDoor):
                stateMachine = OpenDoor;
                elevio_motorDirection(DIRN_STOP);
                break;




            default:

                break;

        }
        //printf("Current state: %d, new state: %d\n", stateMachine, newState);



        //---------------- Order planning --------------------------------
        button_check(elevator);
        
        
    

        // Wait 
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    free(elevator);

    return 0;
}


