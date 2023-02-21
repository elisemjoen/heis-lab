#include "orders.h"

typedef struct OrderScheduler{
    Order orderList[3];
}OrderHandler;


void sortOrders(enum direction dir, int floor);

void addOrder(Order order);

void deleteOrder();

void emptyList();

void getOrder();

void updateOrderLigths();