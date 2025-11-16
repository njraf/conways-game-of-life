#include "gameviewmodel.h"

GameViewModel::GameViewModel()
{
    turn = 0;
    gameSpeed = 0;
    playing = false;
    liveCells = new std::vector<std::shared_ptr<Cell>>();
    pendingCells = new std::vector<std::shared_ptr<Cell>>();
    initialCells = new std::vector<std::shared_ptr<Cell>>();
    prevCells = new std::vector<std::shared_ptr<Cell>>();
    connect(&turnTimer, &QTimer::timeout, this, &GameViewModel::tick);
}

GameViewModel::~GameViewModel() {
    turnTimer.stop();

    clear();

    delete liveCells;
    delete pendingCells;
    delete initialCells;
    delete prevCells;
}

void GameViewModel::tick() {
    turn++;
    saveLiveCells();
    determineNextState();
    draw();
    //printLiveCells();

    emit nextTurn(QString::number(turn));
}

void GameViewModel::determineNextState() {
    // for each live cell
    for (int i = 0; i < liveCells->size(); i++) {
        std::shared_ptr<Cell> liveCell = liveCells->at(i);
        int liveNeighbors = countLiveNeighborsOf(liveCell);
        if (liveNeighbors == 2 || liveNeighbors == 3) {
            // stay alive if has 2 or 3 live neighbors
            liveCell->setNextState(true);
        } else {
            // die if has more or less neighbors
            liveCell->setNextState(false);
        }
        std::shared_ptr<Cell> newCell = std::make_shared<Cell>(*liveCell);
        insertUnique(newCell, pendingCells);

        // for each neighbor
        for (int c = -1; c <= 1; c++) {
            for (int r = -1; r <= 1; r++) {
                if (r == 0 && c == 0) continue; // if self

                // a dead neighbor to a living cell
                std::shared_ptr<Cell> neighbor = std::make_shared<Cell>(liveCell->getPoint().r + r, liveCell->getPoint().c + c);
                neighbor->setAlive(false);

                // check if neighbor is in liveCell list
                auto neighborIt = std::find_if(liveCells->begin(), liveCells->end(), [&neighbor](std::shared_ptr<Cell> cell) { return ((cell->getPoint().r == neighbor->getPoint().r) && (cell->getPoint().c == neighbor->getPoint().c)); });
                if (neighborIt != liveCells->end()) {
                    continue;
                }


                int liveNeighbors = countLiveNeighborsOf(neighbor);
                if (liveNeighbors == 3) {
                    // give dead cell life
                    neighbor->setNextState(true);
                    insertUnique(neighbor, pendingCells);
                }
            }
        }
    }
}

void GameViewModel::draw() {
    // undraw live cells
    liveCells->clear();

    // draw new live cells by setting Cell.alive to Cell.nextState
    for (int i = 0; i < pendingCells->size(); i++) {
        std::shared_ptr<Cell> cell = std::make_shared<Cell>(*pendingCells->at(i));
        cell->setAlive(cell->getNextState());
        if (cell->getAlive()) {
            insertUnique(cell, liveCells);
        }
    }

    pendingCells->clear();
}

int GameViewModel::countLiveNeighborsOf(std::shared_ptr<Cell> cell) {
    int liveNeighbors = 0;

    // for each neighbor
    for (int c = -1; c <= 1; c++) {
        for (int r = -1; r <= 1; r++) {
            if (r == 0 && c == 0) continue; // if self

            // check if neighbor is in the liveCells list
            for (std::vector<std::shared_ptr<Cell>>::iterator it = liveCells->begin(); it != liveCells->end(); it++) {
                if ((*it)->getPoint().r == cell->getPoint().r + r && (*it)->getPoint().c == cell->getPoint().c + c) {
                    liveNeighbors++;
                }
            }
        }
    }
    //std::cout << "std::shared_ptr<Cell> (" << cell.getPoint().r << "," << cell.getPoint().c << ") has " << liveNeighbors << " live neighbors" << std::endl;
    return liveNeighbors;
}

bool GameViewModel::insertUnique(std::shared_ptr<Cell> cell, std::vector<std::shared_ptr<Cell>> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [&cell](std::shared_ptr<Cell> c) { return (cell->getPoint().r == c->getPoint().r) && (cell->getPoint().c == c->getPoint().c); });
    bool found = (list->end() != listIt);

    if (!found) {
        list->push_back(cell);
    }

    return !found; // whether the cell was pushed to the list
}

bool GameViewModel::removeUnique(std::shared_ptr<Cell> cell, std::vector<std::shared_ptr<Cell>> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [&cell](std::shared_ptr<Cell> c) { return (cell->getPoint().r == c->getPoint().r) && (cell->getPoint().c == c->getPoint().c); });
    bool found = (list->end() != listIt);

    if (found) {
        std::shared_ptr<Cell> c = *listIt;
        list->erase(std::remove_if(list->begin(), list->end(), [&c](std::shared_ptr<Cell> listCell){ return (listCell == c); }), list->end());
    }

    return found; // whether the cell was removed from the list
}

bool GameViewModel::removeUnique(int r, int c, std::vector<std::shared_ptr<Cell>> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [r, c](std::shared_ptr<Cell> cell) { return (r == cell->getPoint().r) && (c == cell->getPoint().c); });
    bool found = (list->end() != listIt);

    if (found) {
        std::shared_ptr<Cell> cell = *listIt;
        list->erase(std::remove_if(list->begin(), list->end(), [&cell](std::shared_ptr<Cell> listCell){ return (listCell == cell); }), list->end());
    }

    return found;
}

void GameViewModel::printLiveCells() {
    std::cout << "Step " << turn << std::endl;
    for (int i = 0; i < liveCells->size(); i++) {
        std::shared_ptr<Cell> cell = liveCells->at(i);
        std::cout << "(" << cell->getPoint().r << "," << cell->getPoint().c << ")" << std::endl;
    }
    std::cout << std::endl;
}

bool GameViewModel::isPlaying() {
    return playing;
}

int GameViewModel::getTurn() {
    return turn;
}

void GameViewModel::setSpeed(int speed) {
    gameSpeed = speed;
    if (playing) {
        turnTimer.stop();
        turnTimer.start(1000 - 100 * gameSpeed);
    }
}

std::vector<std::shared_ptr<Cell>> *GameViewModel::getLiveCells() {
    return liveCells;
}

std::vector<std::shared_ptr<Cell>> *GameViewModel::getInitCells() {
    return initialCells;
}

std::vector<std::shared_ptr<Cell>> *GameViewModel::getPrevCells() {
    return prevCells;
}

// meant for player interaction
void GameViewModel::toggleAlive(int r, int c) {
    bool cellNotFound = (liveCells->end() == std::find_if(liveCells->begin(), liveCells->end(), [r, c](std::shared_ptr<Cell> cell) { return (cell->getPoint().r == r) && (cell->getPoint().c == c); }));
    if (cellNotFound) {
        std::shared_ptr<Cell> newCell = std::make_shared<Cell>(r, c);
        insertUnique(newCell, liveCells);
        emit liveCellsUpdated();
    } else {
        removeUnique(r, c, liveCells);
        std::shared_ptr<Cell> newCell = std::make_shared<Cell>(r, c);
        insertUnique(newCell, prevCells);
        emit liveCellsUpdated();
        removeUnique(r, c, prevCells);
    }
}

void GameViewModel::saveLiveCells() {
    prevCells->clear();
    prevCells->resize(liveCells->size());

    std::transform(liveCells->begin(), liveCells->end(), prevCells->begin(), [](std::shared_ptr<Cell> cell) { return std::make_shared<Cell>(*cell); });

}



void GameViewModel::play() {
    if (playing) {
        return;
    }
    playing = true;
    if (turn == 0) {
        initialCells->clear();
        for (int i = 0; i < liveCells->size(); i++) {
            initialCells->push_back(std::make_shared<Cell>(*liveCells->at(i)));
        }
    }

    turnTimer.start(1000 - 100 * gameSpeed);
    emit gameStarted();
}

void GameViewModel::stop() {
    playing = false;
    turnTimer.stop();
    emit gameStopped();
}

void GameViewModel::next() {
    if (turn == 0) {
        initialCells->clear();
        for (int i = 0; i < liveCells->size(); i++) {
            initialCells->push_back(std::make_shared<Cell>(*liveCells->at(i)));
        }
    }

    tick();
}

void GameViewModel::reset() {
    turn = 0;
    liveCells->clear();

    pendingCells->clear();

    for (int i = 0; i < initialCells->size(); i++) {
        liveCells->push_back(std::make_shared<Cell>(*initialCells->at(i)));
    }

    prevCells->clear();

    emit resetBoard();
}

void GameViewModel::clear() {
    liveCells->clear();

    initialCells->clear();

    pendingCells->clear();

    prevCells->clear();

    emit clearBoard();
}



void GameViewModel::saveConfig(QString fileName) {
    ConfigHandler::getInstance()->saveBoard(fileName, liveCells, pendingCells, initialCells, prevCells);
}

void GameViewModel::loadConfig(QString fileName) {
    QStringList stringList;
    ConfigHandler::getInstance()->loadBoard(fileName, stringList);

    clear();

    for (QString line : stringList) {
        QStringList parts = line.split(',');
        if (parts.size() != 3)
            continue;

        std::shared_ptr<Cell> newCell = std::make_shared<Cell>(parts[1].toInt(), parts[2].toInt());

        if (parts[0] == 'L') {
            insertUnique(newCell, liveCells);
        } else if (parts[0] == 'N') {
            insertUnique(newCell, pendingCells);
        } else if (parts[0] == 'P') {
            insertUnique(newCell, prevCells);
        } else if (parts[0] == 'I') {
            insertUnique(newCell, initialCells);
        }
    }

    turn = 0;
    emit nextTurn(QString::number(turn));
}

