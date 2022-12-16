#include "cellWidget.h"

#include <QStyleOption>
#include <QPainter>

CellWidget::CellWidget(QWidget *parent) : QWidget(parent)
{
    point.r = 0;
    point.c = 0;
    alive = false;
    nextState = false;
}

CellWidget::CellWidget(int r, int c, QWidget *parent) : QWidget(parent)
{
    point.r = r;
    point.c = c;
    alive = false;
    nextState = false;
}

//CellWidget::CellWidget(int x, int y, bool _alive)
//{
//    point.x = x;
//    point.y = y;
//    alive = _alive;
//    nextState = false;
//}

CellWidget::~CellWidget() {

}

bool CellWidget::getAlive() {
    return alive;
}

void CellWidget::setAlive(bool _alive) {
    alive =  _alive;
}

bool CellWidget::getNextState() {
    return nextState;
}

void CellWidget::setNextState(bool _state) {
    nextState = _state;
}

BoardPoint CellWidget::getPoint() {
    return point;
}

void CellWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
}

void CellWidget::paintEvent(QPaintEvent *)
     {
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }

bool CellWidget::operator==(CellWidget *cell) {
    if (point.r == cell->getPoint().r && point.c == cell->getPoint().c) return true;
    else return false;
}
