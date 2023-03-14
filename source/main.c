#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "stateMachine.h"
#include "orderHandler.h"


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



//Checks if the current target is above or below floor. Changes direction accordingly
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

//Checks if a button i pressed and makes order
void button_check(Elevator* elevator){
    for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                
                if (btnPressed) {
                    //Makes order, returns the new order
                    Order currentOrder = getOrder(f, b);

                    //Send order to orderHandler, changes the floor bools
                    take_order(currentOrder);
                    // Add order
                    //elevatorMove(elevator, currentOrder);
                    
                    // Remember to turn off the light
                    elevio_buttonLamp(f, b, btnPressed);    
                }
                
                            
            }
        }
}

void clear_button_lights(int floor) {
    elevio_buttonLamp(floor, 0, 0);
    elevio_buttonLamp(floor, 1, 0);
    elevio_buttonLamp(floor, 2, 0);
}

void clear_all_button_lights() {
    for (int i = 0; i < 4; i++) {
        clear_button_lights(i);
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

    // Initialize lights
    clear_all_button_lights();
    elevio_stopLamp(0);
    elevio_floorIndicator(0);

    // Start initialization
    elevio_motorDirection(DIRN_DOWN);


    // Count down variable
    time_t countDown = clock();
    float delayTime = 0.03;
    

    while(1){

        int floor = elevio_floorSensor();


        //---------------- Execute state change --------------------------
        //printf("current state: %d, new state: %d\n", stateMachine, newState);
        switch(stateMachine){
            case(Initialize):
                if (floor != -1) {
                    // Initialize to AtFloor
                    elevator->elevatorDirection = NONE;
                    elevator_movement(elevator);
                    elevator->floor = floor;
                    stateMachine = AtFloor;
                }
                break;


            case(AtFloor):
                if(elevio_stopButton()) {
                    //Sends to Stop state and deletes all
                    stateMachine = Stop;
                    elevio_doorOpenLamp(1);
                }
                else {
                    elevio_doorOpenLamp(0);

                    elevator->currentTarget = next_stop(elevator);
                    elevator_movement(elevator);

                    // order at floor Unreachable!!!
                    if (elevator->currentTarget != -1 && elevator->floor == elevator->currentTarget) {
                        countDown = clock();


                        // Turns on lights
                        elevio_doorOpenLamp(1);
                        stateMachine = OpenDoor;
                    }

                    
                    if(elevator->currentTarget != -1) {
                        stateMachine = Moving;
                    }
                }

                break;

            case(Moving):

                elevator->currentTarget = next_stop(elevator);
                elevator_movement(elevator);

                if (elevio_stopButton()) {
                    stateMachine = Stop;
                    break;
                }

                if (floor != -1) {
                    elevator->floor = floor;
                    elevio_floorIndicator(elevator->floor);
                    if (elevator->currentTarget == floor) {    

                        countDown = clock();
                        
                        elevio_doorOpenLamp(1);
                        stateMachine = OpenDoor;

                        elevator->currentTarget = -1;
                        elevator_movement(elevator);
                        
                    }
                }
                
                break;
            
            case(OpenDoor):
                {


                    // Deletes order
                    elevatorfloor_tofalse(floor);
                    clear_button_lights(floor);

                    if (elevio_stopButton()) {
                        stateMachine = Stop;
                        break;
                    }

                    if(elevio_obstruction()) {
                        countDown = clock();
                    }
                    double tick = ((double)(clock() - countDown)) / CLOCKS_PER_SEC;
                    // Få dette til å stemme med 3 sekunder
                    if (tick > delayTime) {
                        elevio_doorOpenLamp(0);
                        stateMachine = AtFloor;
                        // remove target
                        elevator_movement(elevator);
                    }
                }
                
                break;


            case(Stop):
                {
     
                    if(elevio_stopButton()) {
                        delete_all_orders();
                        clear_all_button_lights();
                        elevator->currentTarget = next_stop(elevator);
                        elevator_movement(elevator);

                        countDown = clock();
                        elevio_stopLamp(1);
                    } else if (elevio_obstruction()) {
                        countDown = clock();
                        elevio_stopLamp(0);
                    } else {
                        elevio_stopLamp(0);
                    }

                    double tick = (double)(clock() - countDown) / CLOCKS_PER_SEC;
                    // printf("tick: %f, delay time: %f\n", tick, delayTime);

                    // Få dette til å stemme med 3 sekunder
                    if (tick > delayTime) {
                        //printf("here! -----------------------------------------\n");
                        stateMachine = AtFloor;
                        elevio_stopLamp(0);
                    }
                    break;
                }

        }
        // print_orders();
        // printf("Current state: %s , elevator direction: %d\n", to_state(stateMachine), elevator->elevatorDirection);
        // printf("current floor: %d, target floor: %d\n", elevator->floor, elevator->currentTarget);



        //---------------- Order planning --------------------------------
        
        // time_t before = clock();
        if (stateMachine != Initialize) button_check(elevator);
        // double interval = (clock()- before)/((double) CLOCKS_PER_SEC);
        // double total = (double) before / CLOCKS_PER_SEC;
        // printf("function takes %f and %f\n", total, interval);
        
        
    

        // Wait 
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    free(elevator);

    return 0;
}


