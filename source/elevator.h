#include "orderHandler.h"
#include <stdbool.h>
typedef struct Elevator {
    int floor;
    enum direction elevatorDirection;
    bool doorOpen;
    bool stopped;
    bool obstructed;
}Elevator;


void elevatorMove(Order order);

void updateFloor();

void doorAction();

void stopElevator();

void updateFloorLights();

void updateStopLights();

void updateOrderHandler(int floor);