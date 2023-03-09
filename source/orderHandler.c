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