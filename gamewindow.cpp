#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playStop()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetClear()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(&viewModel, SIGNAL(nextTurn(QString)), ui->turnCounter, SLOT(setText(QString)));
    connect(&viewModel, SIGNAL(nextTurn(QString)), this, SLOT(draw()));
    connect(ui->speedSlider, SIGNAL(valueChanged(int)), &viewModel, SLOT(setSpeed(int)));

    // Menu Actions
    connect(ui->actionRandom, SIGNAL(triggered()), this, SLOT(generateRandom()));
    connect(ui->actionR_pentomino, SIGNAL(triggered()), this, SLOT(generateRPentomino()));
    connect(ui->actionDie_Hard, SIGNAL(triggered()), this, SLOT(generateDieHard()));
    connect(ui->actionAcorn, SIGNAL(triggered()), this, SLOT(generateAcorn()));

    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QWidget *cell = new Cell(x, y);
            QString styleSheet = cell->styleSheet();
            styleSheet += "background-color: #DCDCDC;";
            cell->setStyleSheet(styleSheet);

            QObject::connect(cell, SIGNAL(clicked(Cell*)), this, SLOT(toggleAlive(Cell*)));

            ui->board->addWidget(cell, x, y);
        }
    }
}

GameWindow::~GameWindow()
{
    viewModel.stop();
    delete ui;
}

// player toggle only
void GameWindow::toggleAlive(Cell *cell) {
    if (viewModel.isPlaying()) return;

    cell->setAlive(!cell->getAlive());

    QWidget *item = (QWidget*)cell;
    QString styleSheet = item->styleSheet();
    if (!cell->getAlive()) {
        styleSheet = "background-color: #DCDCDC;";
        viewModel.removeUnique(cell, viewModel.getLiveCells());
        viewModel.removeUnique(cell, viewModel.getInitCells());
    } else {
        styleSheet = "background-color: yellow;";
        viewModel.insertUnique(cell, viewModel.getLiveCells());
        viewModel.insertUnique(cell, viewModel.getInitCells());
    }
    item->setStyleSheet(styleSheet);
}

void GameWindow::playStop() {
    if (viewModel.isPlaying()) {
        viewModel.stop();
        ui->playButton->setText("Start");
    } else {
        viewModel.play();
        ui->playButton->setText("Stop");
        ui->resetButton->setText("Reset");
    }
}

void GameWindow::next() {
    viewModel.next();
    ui->resetButton->setText("Reset");
}

void GameWindow::resetClear() {
    if (viewModel.getTurn() > 0) {
        viewModel.reset();
        ui->turnCounter->setText("0");
        if (!viewModel.isPlaying()) {
            ui->resetButton->setText("Clear");
        }
    } else {
        viewModel.clear();
        draw();
    }
}

void GameWindow::draw() {
    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QLayoutItem *layoutItem = ui->board->itemAtPosition(x, y);
            QWidget *item = dynamic_cast<QWidgetItem*>(layoutItem)->widget();
            QString styleSheet = item->styleSheet();
            if (!contains(viewModel.getLiveCells(), (Cell*)item)) {
                styleSheet = "background-color: #DCDCDC;";
            } else {
                styleSheet = "background-color: yellow;";
            }
            item->setStyleSheet(styleSheet);
        }
    }
}

bool GameWindow::contains(std::vector<Cell*> *alive, Cell *cell) {
    bool found = false;
    for (int i = 0; i < alive->size(); i++) {
        if (alive->at(i)->getPoint().x == cell->getPoint().x && alive->at(i)->getPoint().y == cell->getPoint().y) {
            found = true;
            break;
        }
    }

    return found;
}



// menu actions

void GameWindow::generateRandom() {
    if (viewModel.isPlaying()) return;

    viewModel.reset();
    viewModel.clear();

    srand(time(0));
    int numCells = rand() % 8 + 5;

    for (int i = 0; i < numCells; i++) {
        int rRow = (DIMENSIONS / 2) + (rand() % 5) - 2;
        int rCol = (DIMENSIONS / 2) + (rand() % 5) - 2;

        QLayoutItem *layoutItem = ui->board->itemAtPosition(rRow, rCol);
        Cell *cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
        viewModel.insertUnique(cell, viewModel.getInitCells());
        viewModel.insertUnique(cell, viewModel.getLiveCells());
    }

    draw();
}

void GameWindow::generateRPentomino() {
    if (viewModel.isPlaying()) return;

    viewModel.reset();
    viewModel.clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2, DIMENSIONS/2);
    Cell *anchor = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(anchor, viewModel.getInitCells());
    viewModel.insertUnique(anchor, viewModel.getLiveCells());

    int anchorX = anchor->getPoint().x;
    int anchorY = anchor->getPoint().y;

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY);
    Cell *cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+2, anchorY);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY-1);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX, anchorY+1);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    draw();
}

void GameWindow::generateDieHard() {
    if (viewModel.isPlaying()) return;

    viewModel.reset();
    viewModel.clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2 - 5, DIMENSIONS/2 - 5);
    Cell *anchor = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(anchor, viewModel.getInitCells());
    viewModel.insertUnique(anchor, viewModel.getLiveCells());

    int anchorX = anchor->getPoint().x;
    int anchorY = anchor->getPoint().y;

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY);
    Cell *cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX, anchorY-1);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+4);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+5);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX-1, anchorY+5);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+6);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    draw();
}

void GameWindow::generateAcorn() {
    if (viewModel.isPlaying()) return;

    viewModel.reset();
    viewModel.clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2, DIMENSIONS/2);
    Cell *anchor = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(anchor, viewModel.getInitCells());
    viewModel.insertUnique(anchor, viewModel.getLiveCells());

    int anchorX = anchor->getPoint().x;
    int anchorY = anchor->getPoint().y;

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+1);
    Cell *cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+2);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY+3);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX-1, anchorY-2);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY-2);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());

    layoutItem = ui->board->itemAtPosition(anchorX+1, anchorY-3);
    cell = (Cell*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel.insertUnique(cell, viewModel.getInitCells());
    viewModel.insertUnique(cell, viewModel.getLiveCells());


    draw();
}








