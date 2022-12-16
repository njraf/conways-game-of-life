#include "cellWidget.h"

#include <QStyleOption>
#include <QPainter>

CellWidget::CellWidget(QWidget *parent) : QWidget(parent)
{
    point.r = 0;
    point.c = 0;
}

CellWidget::CellWidget(int r, int c, QWidget *parent) : QWidget(parent)
{
    point.r = r;
    point.c = c;
}

CellWidget::~CellWidget() {

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

//bool CellWidget::operator==(CellWidget *cell) {
//    if (point.r == cell->getPoint().r && point.c == cell->getPoint().c) return true;
//    else return false;
//}
