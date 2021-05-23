#include "gameviewmodel.h"

GameViewModel::GameViewModel()
{
    turn = 0;
    gameSpeed = 0;
}

void GameViewModel::tick() {
    turn++;
    determineNextState();
    draw();
    printLiveCells();
}

void GameViewModel::determineNextState() {
    // for each live cell
    for (int i = 0; i < liveCells.size(); i++) {
        Cell *liveCell = &liveCells[i];
        int liveNeighbors = countLiveNeightborsOf(*liveCell);
        if (liveNeighbors == 2 || liveNeighbors == 3) {
            // stay alive if has 2 or 3 live neighbors
            liveCell->setNextState(true);
        } else {
            // die if has more or less neighbors
            liveCell->setNextState(false);
        }
        insertUnique(*liveCell, &pendingCells);

        // for each neighbor
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                if (x == 0 && y == 0) continue; // if self
                Cell neighbor(liveCell->getPoint().x + x, liveCell->getPoint().y + y);

                // check if neighbor is in liveCell list
                for (int ii = 0; ii < liveCells.size(); ii++) {
                    if (neighbor == liveCells[ii]) {
                        neighbor.setAlive(true);
                        break;
                    }
                }

                if (!neighbor.getAlive()) {
                    int liveNeighbors = countLiveNeightborsOf(neighbor);
                    if (liveNeighbors == 3) {
                        // give dead cell life
                        neighbor.setNextState(true);
                        pendingCells.push_back(neighbor);
                        insertUnique(neighbor, &pendingCells);
                    }
                }
            }
        }
    }
}

void GameViewModel::draw() {
    // undraw live cells
    liveCells.clear();

    // draw new live cells by setting Cell.alive to Cell.nextState
    for (int i = 0; i < pendingCells.size(); i++) {
        Cell *cell = &pendingCells[i];
        cell->setAlive(cell->getNextState());
        if (cell->getAlive()) insertUnique(*cell, &liveCells);
    }

    pendingCells.clear();
}

int GameViewModel::countLiveNeightborsOf(Cell cell) {
    int liveNeighbors = 0;

    // for each neighbor
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x == 0 && y == 0) continue; // if self

            // check if neighbor is in the liveCells list
            for (std::vector<Cell>::iterator it = liveCells.begin(); it != liveCells.end(); it++) {
                if (it->getPoint().x == cell.getPoint().x + x && it->getPoint().y == cell.getPoint().y + y) {
                    liveNeighbors++;
                }
            }
        }
    }
    //std::cout << "Cell (" << cell.getPoint().x << "," << cell.getPoint().y << ") has " << liveNeighbors << " live neighbors" << std::endl;
    return liveNeighbors;
}

bool GameViewModel::insertUnique(Cell cell, std::vector<Cell> *list) {
    bool found = false;
    for (int i = 0; i < list->size(); i++) {
        if (cell.getPoint().x == list->at(i).getPoint().x && cell.getPoint().y == list->at(i).getPoint().y)
        {
            found = true;
            break;
        }
    }

    if (!found) {
        list->push_back(cell);
    }
    return !found;
}

void GameViewModel::placeCell(Cell cell) {
    cell.setAlive(true);
    insertUnique(cell, &liveCells);
}

void GameViewModel::printLiveCells() {
    std::cout << "Step " << turn << std::endl;
    for (int i = 0; i < liveCells.size(); i++) {
        Cell *cell = &liveCells[i];
        std::cout << "(" << cell->getPoint().x << "," << cell->getPoint().y << ")" << std::endl;
    }
    std::cout << std::endl;
}
