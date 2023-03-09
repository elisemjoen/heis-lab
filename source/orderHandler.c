#include<stdbool.h>
#include "orderHandler.h"


void addOrder(Order order, Order orderList[]) {
}

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


bool elevator_up[] = {false, false, false};

bool elevator_down[] = {false, false, false};

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

//Gives the next stop that the elevator travels to
int next_stop(enum direction dir) {
    if (dir == UP){
        for(int i; i < 4; i++){
            if(elevator_up[i] == true || elevator_inside[i] == true || elevator_down[i] == true){
                if(i > floor){
                    return i;
                }
            }
        }
    }else if(dir == DOWN){
        for(int j; j < 4; j++){
            if(elevator_down[j] == true || elevator_inside[j] == true){
                if(j < floor){
                    return j;
                }
            }
        }
    }
    return 0;
}

//Called at arrival to a floor
void elevatorfloor_tofalse() {
    elevator_inside[floor] = false;
    elevator_up[floor] = false;
    elevator_down[floor] = false;
}

//In main
/*

if(next_stop(DOWN) == 0){
    //Switch direction
    //Maybe have a function to check if there are any more orders
}


*/

