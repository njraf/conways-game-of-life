#include "cell.h"

Cell::Cell()
{
    point.x = 0;
    point.y = 0;
    alive = false;
    nextState = false;
}

Cell::Cell(int x, int y)
{
    point.x = x;
    point.y = y;
    alive = false;
    nextState = false;
}

Cell::Cell(int x, int y, bool _alive)
{
    point.x = x;
    point.y = y;
    alive = _alive;
    nextState = false;
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

bool Cell::operator==(Cell cell) {
    if (point.x == cell.getPoint().x && point.y == cell.getPoint().y) return true;
    else return false;
}
