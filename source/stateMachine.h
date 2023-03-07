#include "driver/elevio.h"

enum states {
    Initialize,
    AtFloor,
    Moving,
    OpenDoor,
    Stop
};

enum stateChanges {
    Init_OpenDoor,

    AtFloor_AtFloor,
    AtFloor_Moving,
    AtFloor_OpenDoor,
    AtFloor_Stop,

    Moving_AtFloor,
    Moving_OpenDoor,
    Moving_Stop,

    OpenDoor_AtFloor,
    OpenDoor_Stop,

    Stop_AtFloor,
    Stop_Moving,
    Stop_OpenDoor,

    NoChange,

    StateError,
    UnknownState
    
};


enum stateChanges changeState(enum states currentState, enum states newState);

void initialize_atFloor();

void atFloor_moving();
void atFloor_stop();

void moving_atFloor();
void moving_stop();

void stop_atFloor();
void stop_moving();
