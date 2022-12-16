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
    bool alive;
    bool nextState;
    BoardPoint point;

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    CellWidget(QWidget *parent = nullptr);
    CellWidget(int r, int c, QWidget *parent = nullptr);
    //Cell(int x, int y, bool _alive);
    virtual ~CellWidget();
    bool getAlive();
    void setAlive(bool _alive);
    bool getNextState();
    void setNextState(bool _state);
    BoardPoint getPoint();
    void paintEvent(QPaintEvent *) override;

    bool operator==(CellWidget *cell);

signals:
    void clicked(CellWidget *cell);
};

#endif // SQUARE_H
