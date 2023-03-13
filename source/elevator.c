#include "elevator.h"

void elevatorMove(Elevator *elevator, Order order){
    elevator->currentTarget = order.destination;
};