#include <stdbool.h>
typedef struct Elevator {
    int floor;
    int currentTarget;
    enum direction elevatorDirection;
    bool doorOpen;
    bool stopped;
    bool obstructed;
}Elevator;


void elevatorMove(Elevator *elevator, Order order);

void updateFloor();

void doorAction();

void stopElevator();

void updateFloorLights();

void updateStopLights();

void updateOrderHandler(int floor);