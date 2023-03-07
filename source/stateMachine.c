#include <stdio.h>
#include "stateMachine.h"
#include "orderHandler.h"



enum stateChanges changeState(enum states currentState, enum states newState) {
    switch(currentState) {
        case Initialize:
            if (newState == OpenDoor) return Init_OpenDoor;
            break;
        case AtFloor:
            if (newState == AtFloor) return AtFloor_AtFloor;
            if (newState == Moving) return AtFloor_Moving;
            if (newState == OpenDoor) return AtFloor_OpenDoor;
            if (newState == Stop) return AtFloor_Stop;
            break;
        case Moving:
            if (newState == AtFloor) return Moving_AtFloor;
            if (newState == OpenDoor) return Moving_OpenDoor;
            if (newState == Stop) return Moving_Stop;
            break;
        case OpenDoor:
            if (newState == AtFloor) return OpenDoor_AtFloor;
            if (newState == Moving) return StateError;
            if (newState == Stop) return OpenDoor_Stop;
            break;
        case Stop:
            if (newState == AtFloor) return Stop_AtFloor;
            if (newState == Moving) return Stop_Moving;
            if (newState == OpenDoor) return Stop_OpenDoor;
            break;
    }
    if (currentState != newState) {
        printf("unrecognized state change from %d to %d \n", currentState, newState);
        return UnknownState;
    }
    
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
