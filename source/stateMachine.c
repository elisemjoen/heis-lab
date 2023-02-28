#include "stateMachine.h"
#include "orderHandler.h"

void changeState(enum states currentState, enum states newState) {
    switch(currentState) {
        case Initialize:
            if (newState == AtFloor) initialize_atFloor();
            break;
        case AtFloor:
            if (newState == Moving) atFloor_moving();
            if (newState == Stop) atFloor_stop();
            break;
        case Moving:
            if (newState == AtFloor) moving_atFloor();
            if (newState == Stop) moving_stop();
            break;
        case Stop:
            if (newState == AtFloor) stop_atFloor();
            if (newState == Moving) stop_moving();
            break;
    }
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
