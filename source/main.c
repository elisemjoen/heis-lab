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

    // Initialize State Machine
    enum states stateMachine = Initialize;
    enum states newState = Initialize;

    // Start initialization
    elevio_motorDirection(DIRN_DOWN);

    

    while(1){

        //---------------- Basic elevator functionality ------------------
        // Set floor counter
        int floor = elevio_floorSensor();
        if (floor != -1){
            elevator->floor = floor;
        }
        // printf("current floor: %d\n", floor);
        //printf("floor: %d, target: %d\n",floor, elevator->currentTarget);
        
        // Stops and obstructs
        if(elevio_obstruction()){
            // Maybe add as its own state

        } else {
            elevio_stopLamp(0);
        }

        //---------------- Set new state ---------------------------------


        //---------------- Execute state change --------------------------
        printf("current state: %d, new state: %d\n", stateMachine, newState);
        switch(stateMachine){
            case(Initialize):
                if (floor != -1) {
                    // Initialize to AtFloor
                    elevio_motorDirection(DIRN_STOP);
                    stateMachine = AtFloor;
                }
                break;


            case(AtFloor):
                elevator_movement(elevator);
                if ( elevator->elevatorDirection != NONE) stateMachine = Moving;
                break;

            case(Moving):
                stateMachine = OpenDoor;
                elevio_motorDirection(DIRN_STOP);
                break;
            
            case(OpenDoor):
                stateMachine = Stop;
                elevio_motorDirection(DIRN_STOP);
                break;


            case(Stop):
                elevator_movement(elevator);
                break;
            
            default:

                break;

        }
        //printf("Current state: %d, new state: %d\n", stateMachine, newState);



        //---------------- Order planning --------------------------------
        time_t before = clock();
        button_check(elevator);
        double interval = (clock()- before)/CLOCKS_PER_SEC;
        printf("function takes %f\n", interval);
        
        
    

        // Wait 
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    free(elevator);

    return 0;
}


