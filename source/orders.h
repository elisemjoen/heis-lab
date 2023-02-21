
enum direction {UP, DOWN};


typedef struct Order{
    enum {Internal, External} orderType;
    enum direction pressedDirection;
} Order;


