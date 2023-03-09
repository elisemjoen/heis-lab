#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "elevator.h"
#include "stateMachine.h"


// For debugging:
char* to_state(enum states state) {
    switch(state) {
        case(0):
            return "Initialize";
            break;
        case(1):
            return "AtFloor";
            break;
        case(2):
            return "Moving";
            break;
        case(3):
            return "OpenDoor";
            break;
        case(4):
            return "Stop";
            break;
    }
}


void elevator_movement(Elevator* elevator) {
    if (elevator->currentTarget == -1 | elevator->currentTarget == elevator->floor) {
        elevio_motorDirection(DIRN_STOP);
        elevator->elevatorDirection = NONE;
    } else if (elevator->currentTarget < elevator->floor) {
        elevio_motorDirection(DIRN_DOWN);
        elevator->elevatorDirection = DOWN;
    } else if (elevator->currentTarget > elevator->floor) {
        elevio_motorDirection(DIRN_UP);
        elevator->elevatorDirection = UP;
    }

}


void button_check(Elevator* elevator){
    for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                
                if (btnPressed) {
                    Order currentOrder = getOrder(f, b);
                    // Add order
                    elevatorMove(elevator, currentOrder);
                    
                    // Remember to turn of the light
                    elevio_buttonLamp(f, b, btnPressed);    
                }
                
                            
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


    // Count down variable
    time_t countDown = clock();
    

    while(1){
        

        //---------------- Basic elevator functionality ------------------
        // Set floor counter
        int floor = elevio_floorSensor();
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
        //printf("current state: %d, new state: %d\n", stateMachine, newState);
        switch(stateMachine){
            case(Initialize):
                if (floor != -1) {
                    // Initialize to AtFloor
                    elevator->elevatorDirection = NONE;
                    elevator_movement(elevator);
                    stateMachine = AtFloor;
                }
                break;


            case(AtFloor):
                if(elevio_stopButton()) {
                    stateMachine = Stop;
                    elevio_stopLamp(1);
                    // Delete all orders

                    break;
                }

                // order at floor
                if (elevator->currentTarget != -1 && elevator->floor == elevator->currentTarget) {
                    countDown = clock();
                    // Delete order
                    // Turn on lights
                    elevio_doorOpenLamp(1);
                    stateMachine = OpenDoor;
                }
                
                if(elevator->currentTarget != -1) {
                    stateMachine = Moving;
                    elevator_movement(elevator);
                }

                break;

            case(Moving):
                if (elevio_stopButton()) {
                    elevator->elevatorDirection = NONE;
                    elevator->currentTarget = -1;
                    elevator_movement(elevator);
                    stateMachine = Stop;
                    elevio_stopLamp(1);

                    // Delete all orders
                    elevator->currentTarget = -1;

                    break;
                }

                if (floor != -1) {
                    elevator->floor = floor;
                    if (elevator->currentTarget == floor) {
                        stateMachine = AtFloor;
                        elevator->currentTarget = -1;
                        elevator_movement(elevator);
                    }
                }
                
                break;
            
            case(OpenDoor):
                {
                    double tick = (double)(clock() - countDown) / CLOCKS_PER_SEC;
                    // printf("tick: %f\n", tick);
                    if (tick > 0.02) {
                        stateMachine = AtFloor;
                        // remove target
                        elevator->currentTarget = -1;
                        elevator_movement(elevator);
                    }
                }
                
                break;


            case(Stop):
                {
                    if (elevio_stopButton()) countDown = clock();
                    double tick = (double)(clock() - countDown) / CLOCKS_PER_SEC;
                    // printf("tick: %f\n", tick);
                    if (tick > 0.02) {
                        stateMachine = AtFloor;
                        elevio_stopLamp(0);
                    }
                    break;
                }

        }
        printf("Current state: %s \n", to_state(stateMachine));



        //---------------- Order planning --------------------------------
        
        // time_t before = clock();
        button_check(elevator);
        // double interval = (clock()- before)/((double) CLOCKS_PER_SEC);
        // double total = (double) before / CLOCKS_PER_SEC;
        // printf("function takes %f and %f\n", total, interval);
        
        
    

        // Wait 
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    free(elevator);

    return 0;
}


