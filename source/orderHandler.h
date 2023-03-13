#include "orders.h"
#include "elevator.h"

void delete_all_orders();

void elevatorfloor_tofalse(int floor);

Order getOrder(int floor, int button);

int next_stop(Elevator* elevator);

void take_order(Order order);


//void sortOrders(enum direction dir, int floor, Order orderList[]);

//void addOrder(Order order, Order orderList[]);

//void deleteOrder(Order orderList);

//void emptyList(Order orderList);

//void updateOrderLigths(Order orderList);