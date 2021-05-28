#include "cell.h"

#include <QStyleOption>
#include <QPainter>

Cell::Cell(QWidget *parent) : QWidget(parent)
{
    point.x = 0;
    point.y = 0;
    alive = false;
    nextState = false;
}

Cell::Cell(int x, int y, QWidget *parent) : QWidget(parent)
{
    point.x = x;
    point.y = y;
    alive = false;
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

void Cell::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
}

void Cell::paintEvent(QPaintEvent *)
     {
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }

bool Cell::operator==(Cell *cell) {
    if (point.x == cell->getPoint().x && point.y == cell->getPoint().y) return true;
    else return false;
}
