#include "driver/elevio.h"

enum states {
    Initialize,
    AtFloor,
    Moving,
    Stop
};


void changeState(enum states currentState, enum states newState);

void initialize_atFloor();

void atFloor_moving();
void atFloor_stop();

void moving_atFloor();
void moving_stop();

void stop_atFloor();
void stop_moving();
