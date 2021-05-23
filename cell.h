#ifndef SQUARE_H
#define SQUARE_H

struct Point {
    int x, y;
};

class Cell
{
private:
    bool alive;
    bool nextState;
    Point point;

public:
    Cell();
    Cell(int x, int y);
    Cell(int x, int y, bool _alive);
    bool getAlive();
    void setAlive(bool _alive);
    bool getNextState();
    void setNextState(bool _state);
    Point getPoint();

    bool operator==(Cell cell);
};

#endif // SQUARE_H
