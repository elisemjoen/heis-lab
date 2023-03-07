#include "stateMachine.h"
#include "orderHandler.h"



enum stateChanges changeState(enum states currentState, enum states newState) {
    switch(currentState) {
        case Initialize:
            if (newState == AtFloor) return Init_AtFloor;
            break;
        case AtFloor:
            if (newState == Moving) return AtFloor_Moving;
            if (newState == OpenDoor) return AtFloor_OpenDoor;
            if (newState == Stop) return AtFloor_Stop;
            break;
        case Moving:
            if (newState == AtFloor) return Moving_AtFloor;
            if (newState == OpenDoor) return Error;
            if (newState == Stop) return Moving_Stop;
            break;
        case OpenDoor:
            if (newState == AtFloor) return OpenDoor_AtFloor;
            if (newState == Moving) return Error;
            if (newState == Stop) return OpenDoor_Stop;
            break;
        case Stop:
            if (newState == AtFloor) return Stop_AtFloor;
            if (newState == Moving) return Stop_Moving;
            if (newState == OpenDoor) return Stop_OpenDoor;
            break;
    }
    printf("unrecognized state change %d to %d \n", currentState, newState);
    return NoChange;
}

void initialize_atFloor() {
    
}

void atFloor_moving() {

}

void atFloor_stop() {

}

void moving_atFloor() {

}

void moving_stop() {

}

void stop_atFloor() {

}

void stop_moving() {

}
