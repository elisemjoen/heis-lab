#include<stdbool.h>
#include "orderHandler.h"

// temporary variable


void addOrder(Order order, Order orderList[]) {
}

//Makes order
Order getOrder(int floor, int button) {
    int buttonType;
    if (button == 2) {
        buttonType = 0;
    } else {
        buttonType = 1;
    }
    enum direction orderDirection = NONE;

    Order newOrder = {buttonType, orderDirection, floor};
    return newOrder;
}


bool elevator_up[] = {false, false, false, false};

bool elevator_down[] = {false, false, false, false};

bool elevator_inside[] = {false, false, false, false};


//Takes an order
void take_order(Order order) {
    if(order.orderType == Internal){
        elevator_inside[order.destination] = true;
    }else if (order.orderType == External){
        if(order.pressedDirection == UP){
            elevator_up[order.destination] = true;
        }else if (order.pressedDirection == DOWN){
            elevator_down[order.destination] = true;
        }
    }
}
int max_floor = 3;
int min_floor = 0;
//Gives the next stop that the elevator travels to
int next_stop(Elevator* elevator) {
    if(elevator->elevatorDirection == NONE){
        //Iterates through floors above current floor
        for(int k = elevator->floor + 1; k <= max_floor; k++){
            if(elevator_up[k] == true || elevator_inside[k] == true){
                if(k > elevator->floor){
                    return k;
                }
            }
        }
        //Iterates through floors below current floor
        for(int h = elevator->floor - 1; h >= min_floor; h--){
            if(elevator_down[h] == true || elevator_inside[h] == true){
                if(h < elevator->floor){
                    return h;
                }
            }
        }
        return 0;
    }
    if (elevator->elevatorDirection == UP){
        //Iterates through floors above current floor
        for(int i = elevator->floor + 1; i <= max_floor; i++){
            if(elevator_up[i] == true || elevator_inside[i] == true){
                if(i > elevator->floor){
                    return i;
                }
            }
        }
    }
    if(elevator->elevatorDirection == DOWN){
        //Iterates through floors below current floor
        for(int j = elevator->floor - 1; j >= min_floor; j--){
            if(elevator_down[j] == true || elevator_inside[j] == true){
                if(j < elevator->floor){
                    return j;
                }
            }
        }
    }
    return 0;
}

//Called at arrival to a floor
void elevatorfloor_tofalse(int floor) {
    elevator_inside[floor] = false;
    elevator_up[floor] = false;
    elevator_down[floor] = false;
}

//Deletes all orders. Used when stop button is pressed.
void delete_all_orders(){
    for(int i = 0; i < 3; i++){
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
