#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <QWidget>
#include <QMouseEvent>

struct BoardPoint {
    int r, c;
};

class CellWidget : public QWidget
{
    Q_OBJECT

private:
    BoardPoint point;

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    CellWidget(QWidget *parent = nullptr);
    CellWidget(int r, int c, QWidget *parent = nullptr);
    virtual ~CellWidget();
    BoardPoint getPoint();
    void paintEvent(QPaintEvent *) override;

    //bool operator==(CellWidget *cell);

signals:
    void clicked(CellWidget *cell);
};

#endif // SQUARE_H
