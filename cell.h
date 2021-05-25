#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <QWidget>
#include <QMouseEvent>

struct Point {
    int x, y;
};

class Cell : public QWidget
{
    Q_OBJECT

private:
    bool alive;
    bool nextState;
    Point point;

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    Cell(QWidget *parent = nullptr);
    Cell(int x, int y, QWidget *parent = nullptr);
    //Cell(int x, int y, bool _alive);
    virtual ~Cell();
    bool getAlive();
    void setAlive(bool _alive);
    bool getNextState();
    void setNextState(bool _state);
    Point getPoint();

    bool operator==(Cell *cell);

signals:
    void clicked(Cell *cell);
};

#endif // SQUARE_H
