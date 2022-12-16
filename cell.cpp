#include "cell.h"

Cell::Cell()
{
    point.r = 0;
    point.c = 0;
    alive = true;
    nextState = false;
}

Cell::Cell(int r, int c)
{
    point.r = r;
    point.c = c;
    alive = true;
    nextState = false;
}

//Cell::Cell(int x, int y, bool _alive)
//{
//    point.x = x;
//    point.y = y;
//    alive = _alive;
//    nextState = false;
//}

Cell::~Cell() {

}

bool Cell::getAlive() {
    return alive;
}

void Cell::setAlive(bool _alive) {
    alive =  _alive;
}

bool Cell::getNextState() {
    return nextState;
}

void Cell::setNextState(bool _state) {
    nextState = _state;
}

Point Cell::getPoint() {
    return point;
}
