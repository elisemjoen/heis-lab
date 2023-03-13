#include<stdbool.h>
#include "orderHandler.h"


bool elevator_up[] = {false, false, false, false};

bool elevator_down[] = {false, false, false, false};

bool elevator_inside[] = {false, false, false, false};

// for debugging
void print_orders() {
        printf("u: ");
        for(int j = 0; j < 4; j++) {
            printf("%d ", elevator_up[j]);
        }

        printf("d: ");
        for(int j = 0; j < 4; j++) {
            printf("%d ", elevator_down[j]);
        }

        printf("i: ");
        for(int j = 0; j < 4; j++) {
            printf("%d ", elevator_inside[j]);
        }

    printf("\n"); 
}

//Makes order
Order getOrder(int floor, int button) {
    enum direction orderDirection = NONE;

    Order newOrder = {button, orderDirection, floor};
    return newOrder;
}


//Takes an order
void take_order(Order order) {
    if(order.orderType == Internal) {
        elevator_inside[order.destination] = true;
        }
    if (order.orderType == Up) {
        elevator_up[order.destination] = true;
    }
    if (order.orderType == DOWN) {
        elevator_down[order.destination] = true;
    }
}

int max_floor = 3;
int min_floor = 0;

//Gives the next stop that the elevator travels to
int next_stop(Elevator* elevator) {
    if(elevator->elevatorDirection == NONE){
        //Iterates through floors above current floor
        for(int k = elevator->floor + 1; k <= max_floor; k++){
            if(elevator_up[k] == true || elevator_down[k] == true || elevator_inside[k] == true){
                return k;
            }
        }
        //Iterates through floors below current floor
        for(int h = elevator->floor - 1; h >= min_floor; h--){
            if(elevator_up[h] == true || elevator_down[h] == true || elevator_inside[h] == true){
                return h;
            }
        }
    }

    if (elevator->elevatorDirection == UP){
        //Iterates through floors above current floor
        for(int i = elevator->floor + 1; i <= max_floor; i++){
            if(elevator_up[i] == true || elevator_inside[i] == true){
                return i;
            }
        }
    }

    if(elevator->elevatorDirection == DOWN){
        //Iterates through floors below current floor
        for(int j = elevator->floor - 1; j >= min_floor; j--){
            if(elevator_down[j] == true || elevator_inside[j] == true){
                    return j;
            }
        }
    }

    // Check oposites
    if (elevator->elevatorDirection == UP){
        //Iterates through floors above current floor
        for(int i = elevator->floor + 1; i <= max_floor; i++){
            if(elevator_down[i] == true){
                return i;
            }
        }
        
    }

    if(elevator->elevatorDirection == DOWN){
        //Iterates through floors below current floor
        for(int j = elevator->floor - 1; j >= min_floor; j--){
            if(elevator_up[j] == true){
                    return j;
            }
        }
    }


    return -1;
}

//Called at arrival to a floor
void elevatorfloor_tofalse(int floor) {
    elevator_inside[floor] = false;
    elevator_up[floor] = false;
    elevator_down[floor] = false;
}

//Deletes all orders. Used when stop button is pressed.
void delete_all_orders(){
    for(int i = 0; i < 4; i++){
        elevator_inside[i] = false;
        elevator_up[i] = false;
        elevator_down[i] = false;
    }
}

//In main
/*

if(next_stop(DOWN) == 0){
    //Switch direction
    //Maybe have a function to check if there are any more orders
}


*/
