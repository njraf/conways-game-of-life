#include "gameviewmodel.h"

GameViewModel::GameViewModel()
{
    turn = 0;
    gameSpeed = 0;
    playing = false;
    liveCells = new std::vector<Cell*>();
    pendingCells = new std::vector<Cell*>();
    initialCells = new std::vector<Cell*>();
    connect(&turnTimer, &QTimer::timeout, this, &GameViewModel::tick);
}

GameViewModel::~GameViewModel() {
    turnTimer.stop();
    for (int i = 0; i < liveCells->size(); i++) {
        delete liveCells->at(i);
    }

    for (int i = 0; i < pendingCells->size(); i++) {
        delete pendingCells->at(i);
    }

    for (int i = 0; i < initialCells->size(); i++) {
        delete initialCells->at(i);
    }

    liveCells->clear();
    pendingCells->clear();
    initialCells->clear();

    delete liveCells;
    delete pendingCells;
    delete initialCells;
}

void GameViewModel::tick() {
    turn++;
    determineNextState();
    draw();
    //printLiveCells();

    emit nextTurn(QString::number(turn));
}

void GameViewModel::determineNextState() {
    // for each live cell
    for (int i = 0; i < liveCells->size(); i++) {
        Cell *liveCell = liveCells->at(i);
        int liveNeighbors = countLiveNeightborsOf(liveCell);
        if (liveNeighbors == 2 || liveNeighbors == 3) {
            // stay alive if has 2 or 3 live neighbors
            liveCell->setNextState(true);
        } else {
            // die if has more or less neighbors
            liveCell->setNextState(false);
        }
        insertUnique(liveCell, pendingCells);

        // for each neighbor
        for (int c = -1; c <= 1; c++) {
            for (int r = -1; r <= 1; r++) {
                if (r == 0 && c == 0) continue; // if self

                // check if neighbor is in liveCell list
                auto neighborIt = std::find_if(liveCells->begin(), liveCells->end(), [=](Cell *cell) { return (cell->getPoint().r == r) && (cell->getPoint().c == c); });
                if (neighborIt != liveCells->end()) {
                    continue;
                }

                // a dead neighbor to a living cell
                Cell *neighbor = new Cell(liveCell->getPoint().r + r, liveCell->getPoint().c + c);
                neighbor->setAlive(false);

                int liveNeighbors = countLiveNeightborsOf(neighbor);
                if (liveNeighbors == 3) {
                    // give dead cell life
                    neighbor->setNextState(true);
                    insertUnique(neighbor, pendingCells);
                } else {
                    delete neighbor;
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
        Cell *cell = pendingCells->at(i);
        cell->setAlive(cell->getNextState());
        if (cell->getAlive()) {
            insertUnique(cell, liveCells);
        }
    }

    pendingCells->clear();
}

int GameViewModel::countLiveNeightborsOf(Cell* cell) {
    int liveNeighbors = 0;

    // for each neighbor
    for (int c = -1; c <= 1; c++) {
        for (int r = -1; r <= 1; r++) {
            if (r == 0 && c == 0) continue; // if self

            // check if neighbor is in the liveCells list
            for (std::vector<Cell*>::iterator it = liveCells->begin(); it != liveCells->end(); it++) {
                if ((*it)->getPoint().r == cell->getPoint().r + r && (*it)->getPoint().c == cell->getPoint().c + c) {
                    liveNeighbors++;
                }
            }
        }
    }
    //std::cout << "Cell* (" << cell.getPoint().r << "," << cell.getPoint().c << ") has " << liveNeighbors << " live neighbors" << std::endl;
    return liveNeighbors;
}

bool GameViewModel::insertUnique(Cell* cell, std::vector<Cell*> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [=](Cell *c) { return (cell->getPoint().r == c->getPoint().r) && (cell->getPoint().c == c->getPoint().c); });
    bool found = (list->end() != listIt);

    if (!found) {
        list->push_back(cell);
    }

    return !found; // whether the cell was pushed to the list
}

bool GameViewModel::removeUnique(Cell* cell, std::vector<Cell*> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [=](Cell *c) { return (cell->getPoint().r == c->getPoint().r) && (cell->getPoint().c == c->getPoint().c); });
    bool found = (list->end() != listIt);

    if (found) {
        Cell *c = *listIt;
        list->erase(listIt);
        delete c;
    }

    return found; // whether the cell was removed from the list
}

bool GameViewModel::removeUnique(int r, int c, std::vector<Cell*> *list) {
    auto listIt = std::find_if(list->begin(), list->end(), [=](Cell *cell) { return (r == cell->getPoint().r) && (c == cell->getPoint().c); });
    bool found = (list->end() != listIt);

    if (found) {
        Cell *cell = *listIt;
        list->erase(listIt);
        delete cell;
    }

    return found;
}

void GameViewModel::placeCell(Cell* cell) {
    cell->setAlive(true);
    insertUnique(cell, liveCells);
    emit liveCellsUpdated();
}

void GameViewModel::printLiveCells() {
    std::cout << "Step " << turn << std::endl;
    for (int i = 0; i < liveCells->size(); i++) {
        Cell *cell = liveCells->at(i);
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

std::vector<Cell*> *GameViewModel::getLiveCells() {
    return liveCells;
}

std::vector<Cell*> *GameViewModel::getInitCells() {
    return initialCells;
}

// meant for player interaction
void GameViewModel::toggleAlive(int r, int c) {
    if (liveCells->end() == std::find_if(liveCells->begin(), liveCells->end(), [=](Cell *cell) { return (cell->getPoint().r == r) && (cell->getPoint().c == c); })) {
        insertUnique(new Cell(r, c), liveCells);
        emit liveCellsUpdated();
    } else {
        removeUnique(r, c, liveCells);
        emit liveCellsUpdated();
    }
}



void GameViewModel::play() { //TODO: delete cells?
    if (playing) {
        return;
    }
    playing = true;
    initialCells->clear();
    for (int i = 0; i < liveCells->size(); i++) {
        initialCells->push_back(liveCells->at(i));
    }

    turnTimer.start(1000 - 100 * gameSpeed);
}

void GameViewModel::stop() {
    playing = false;
    turnTimer.stop();
}

void GameViewModel::next() {
    tick();
}

void GameViewModel::reset() { //TODO: delete cells?
    turn = 0;
    liveCells->clear();
    for (int i = 0; i < initialCells->size(); i++) {
        liveCells->push_back(initialCells->at(i));
    }
    emit nextTurn(QString::number(turn));
}

void GameViewModel::clear() { //TODO: delete cells?
    for (int i = 0; i < liveCells->size(); i++) {
        //delete liveCells->at(i);
    }
    liveCells->clear();

    for (int i = 0; i < initialCells->size(); i++) {
        //delete initialCells->at(i);
    }
    initialCells->clear();
}



