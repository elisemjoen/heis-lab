#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "elevator.h"
#include "stateMachine.h"


/// @brief 
/// @return 
int main(){

    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
    
    // Startup

    // Initialize State Machine
    enum states StateMachine = Initialize;

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

    int floor = elevio_floorSensor();
    if(floor == -1){
        elevio_motorDirection(DIRN_DOWN);
        if(floor != -1){
            elevio_motorDirection(DIRN_STOP);
        }
    }

    

    while(1){
        int floor = elevio_floorSensor();
        if (floor != -1){
            elevator->floor = floor;
            if (elevator->currentTarget == elevator->floor) {
                    elevio_motorDirection(DIRN_STOP);
            } 
        }
        printf("floor: %d, target: %d\n",floor, elevator->currentTarget);




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

        if (elevator->elevatorDirection == UP) {
            elevio_motorDirection(DIRN_UP);
        } else if (elevator->elevatorDirection == DOWN) {
            elevio_motorDirection(DIRN_DOWN);
        } else if (elevator->elevatorDirection == NONE) {
            elevio_motorDirection(DIRN_STOP);
        }
        

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
