#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <iostream>
#include <vector>
#include <QTimer>
#include <qdebug.h>
#include "cell.h"

class GameViewModel : public QObject
{
    Q_OBJECT
private:
    std::vector<Cell*> *liveCells;
    std::vector<Cell*> *pendingCells; // liveCells for next turn
    std::vector<Cell*> *initialCells;
    std::vector<Cell*> *prevCells; // live cells from the turn
    int gameSpeed; // 0-10; (1000ms - gameSpeed * 100) == sleep time between ticks
    int turn;
    bool playing;
    QTimer turnTimer;

public:
    GameViewModel();
    ~GameViewModel();
    void tick(); // one frame of game loop
    void determineNextState(); // determines value of Cell*.nextState
    void draw(); // sets Square.alive to Cell*.nextState
    int countLiveNeightborsOf(Cell* cell);
    bool insertUnique(Cell* cell, std::vector<Cell*> *list); // insert cell only if the cell does not exist in the list already
    bool removeUnique(Cell* cell, std::vector<Cell*> *list);
    bool removeUnique(int x, int y, std::vector<Cell*> *list);
    void placeCell(Cell* cell);
    void printLiveCells(); // cmd line print x,y locations of live cells
    bool isPlaying();
    int getTurn();
    std::vector<Cell*> *getLiveCells();
    std::vector<Cell*> *getInitCells();
    std::vector<Cell*> *getPrevCells();
    void toggleAlive(int x, int y);
    void saveLiveCells(); // copy liveCells to prevCells

    void play();
    void next();
    void reset();
    void clear();

signals:
    void nextTurn(QString turn);
    void liveCellsUpdated();
    void gameStarted();
    void gameStopped();

public slots:
    void setSpeed(int speed);
    void stop();
};

#endif // GAMEVIEWMODEL_H
