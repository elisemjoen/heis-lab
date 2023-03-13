
enum direction {UP, DOWN, NONE};


typedef struct Order{
    enum {Up, Down, Internal} orderType;
    enum direction pressedDirection;
    int destination;
} Order;


