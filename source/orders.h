
enum direction {UP, DOWN, NONE};


typedef struct Order{
    enum {Internal, External} orderType;
    enum direction pressedDirection;
    int destination;
} Order;


