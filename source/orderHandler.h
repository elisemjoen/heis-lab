#include "orders.h"


void sortOrders(enum direction dir, int floor, Order orderList[]);

void addOrder(Order order, Order orderList[]);

void deleteOrder(Order orderList);

void emptyList(Order orderList);

Order getOrder(int floor, int button);

void updateOrderLigths(Order orderList);