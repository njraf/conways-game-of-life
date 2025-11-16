#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <iostream>
#include <vector>
#include <QTimer>
#include <qdebug.h>
#include "cell.h"
#include "confighandler.h"

class GameViewModel : public QObject
{
    Q_OBJECT
private:
    std::vector<std::shared_ptr<Cell>> *liveCells;
    std::vector<std::shared_ptr<Cell>> *pendingCells; // liveCells for next turn
    std::vector<std::shared_ptr<Cell>> *initialCells;
    std::vector<std::shared_ptr<Cell>> *prevCells; // live cells from the turn
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
    int countLiveNeighborsOf(std::shared_ptr<Cell> cell);
    bool insertUnique(std::shared_ptr<Cell> cell, std::vector<std::shared_ptr<Cell>> *list); // insert cell only if the cell does not exist in the list already
    bool removeUnique(std::shared_ptr<Cell> cell, std::vector<std::shared_ptr<Cell>> *list);
    bool removeUnique(int x, int y, std::vector<std::shared_ptr<Cell>> *list);
    void printLiveCells(); // cmd line print x,y locations of live cells
    bool isPlaying();
    int getTurn();
    std::vector<std::shared_ptr<Cell>> *getLiveCells();
    std::vector<std::shared_ptr<Cell>> *getInitCells();
    std::vector<std::shared_ptr<Cell>> *getPrevCells();
    void toggleAlive(int x, int y);
    void saveLiveCells(); // copy liveCells to prevCells

    void play();
    void next();
    void reset();
    void clear();

    void saveConfig(QString fileName);
    void loadConfig(QString fileName);

signals:
    void nextTurn(QString turn);
    void liveCellsUpdated();
    void gameStarted();
    void gameStopped();
    void resetBoard();
    void clearBoard();

public slots:
    void setSpeed(int speed);
    void stop();
};

#endif // GAMEVIEWMODEL_H
