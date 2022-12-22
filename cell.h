#ifndef CELL_H
#define CELL_H

#include <QObject>

struct Point {
    int r, c;
};

class Cell : public QObject
{
    Q_OBJECT

private:
    bool alive;
    bool nextState;
    Point point;

public:
    Cell();
    Cell(int r, int c);
    Cell(const Cell &cell);
    //Cell(int x, int y, bool _alive);
    virtual ~Cell();
    bool getAlive();
    void setAlive(bool _alive);
    bool getNextState();
    void setNextState(bool _state);
    Point getPoint() const;

    bool operator==(const Cell &cell) const;
};

#endif // CELL_H
