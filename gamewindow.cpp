#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
    , viewModel(new GameViewModel())
{
    ui->setupUi(this);
    viewModel->setSpeed(ui->speedSlider->value());

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playStop()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetClear()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(viewModel, SIGNAL(nextTurn(QString)), ui->turnCounter, SLOT(setText(QString)));
    connect(viewModel, SIGNAL(nextTurn(QString)), this, SLOT(draw()));
    connect(viewModel, SIGNAL(liveCellsUpdated()), this, SLOT(draw()));
    connect(ui->speedSlider, SIGNAL(valueChanged(int)), viewModel, SLOT(setSpeed(int)));

    // Menu Actions
    connect(ui->actionRandom, SIGNAL(triggered()), this, SLOT(generateRandom()));
    connect(ui->actionR_pentomino, SIGNAL(triggered()), this, SLOT(generateRPentomino()));
    connect(ui->actionDie_Hard, SIGNAL(triggered()), this, SLOT(generateDieHard()));
    connect(ui->actionAcorn, SIGNAL(triggered()), this, SLOT(generateAcorn()));

    // create default board
    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QWidget *cell = new CellWidget(x, y);
            QString styleSheet = cell->styleSheet();
            styleSheet += "background-color: #DCDCDC;";
            cell->setStyleSheet(styleSheet);

            QObject::connect(cell, SIGNAL(clicked(CellWidget*)), this, SLOT(toggleAlive(CellWidget*)));

            ui->board->addWidget(cell, x, y);
        }
    }
}

GameWindow::~GameWindow()
{
    viewModel->stop();
    delete viewModel;
    delete ui;
}

// player toggle only
void GameWindow::toggleAlive(CellWidget *cell) {
    if (viewModel->isPlaying()) return;

    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);
}

void GameWindow::playStop() {
    if (viewModel->isPlaying()) {
        viewModel->stop();
        ui->playButton->setText("Start");
    } else {
        viewModel->play();
        ui->playButton->setText("Stop");
        ui->resetButton->setText("Reset");
    }
}

void GameWindow::next() {
    if (!viewModel->isPlaying()) {
        viewModel->next();
        ui->resetButton->setText("Reset");
    }
}

void GameWindow::resetClear() {
    if (viewModel->getTurn() > 0) {
        viewModel->reset();
        ui->turnCounter->setText("0");
        if (!viewModel->isPlaying()) {
            ui->resetButton->setText("Clear");
        }
    } else {
        if (viewModel->isPlaying()) {
            playStop();
        }
        viewModel->clear();
        draw();
    }
}

void GameWindow::draw() {
    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QLayoutItem *layoutItem = ui->board->itemAtPosition(x, y);
            QWidget *item = dynamic_cast<QWidgetItem*>(layoutItem)->widget();
            QString styleSheet = item->styleSheet();
            if (!contains(viewModel->getLiveCells(), (CellWidget*)item)) {
                styleSheet = "background-color: #DCDCDC;";
            } else {
                styleSheet = "background-color: yellow;";
            }
            item->setStyleSheet(styleSheet);
        }
    }
}

bool GameWindow::contains(std::vector<Cell*> *alive, CellWidget *cell) {
    bool found = false;
    for (int i = 0; i < alive->size(); i++) {
        if (alive->at(i)->getPoint().r == cell->getPoint().r && alive->at(i)->getPoint().c == cell->getPoint().c) {
            found = true;
            break;
        }
    }

    return found;
}



// menu actions

void GameWindow::generateRandom() {
    if (viewModel->isPlaying()) return;

    viewModel->reset();
    viewModel->clear();

    srand(time(0));
    int numCells = rand() % 8 + 5; // 5 - 12

    for (int i = 0; i < numCells; i++) {
        int rRow = (DIMENSIONS / 2) + (rand() % 5) - 2; // center +/- 2
        int rCol = (DIMENSIONS / 2) + (rand() % 5) - 2; // center +/- 2
        viewModel->toggleAlive(rRow, rCol);
    }
}

void GameWindow::generateRPentomino() {
    if (viewModel->isPlaying()) return;

    viewModel->reset();
    viewModel->clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2, DIMENSIONS/2);
    CellWidget *anchor = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();

    int anchorR = anchor->getPoint().r;
    int anchorC = anchor->getPoint().c;
    viewModel->toggleAlive(anchorR, anchorC);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC);
    CellWidget *cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+2, anchorC);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC-1);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR, anchorC+1);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);
}

void GameWindow::generateDieHard() {
    if (viewModel->isPlaying()) return;

    viewModel->reset();
    viewModel->clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2 - 5, DIMENSIONS/2 - 5);
    CellWidget *anchor = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();

    int anchorR = anchor->getPoint().r;
    int anchorC = anchor->getPoint().c;
    viewModel->toggleAlive(anchorR, anchorC);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC);
    CellWidget *cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR, anchorC-1);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+4);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+5);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR-1, anchorC+5);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+6);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);
}

void GameWindow::generateAcorn() {
    if (viewModel->isPlaying()) return;

    viewModel->reset();
    viewModel->clear();

    QLayoutItem *layoutItem = ui->board->itemAtPosition(DIMENSIONS/2, DIMENSIONS/2);
    CellWidget *anchor = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();

    int anchorR = anchor->getPoint().r;
    int anchorC = anchor->getPoint().c;
    viewModel->toggleAlive(anchorR, anchorC);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+1);
    CellWidget *cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+2);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC+3);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR-1, anchorC-2);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC-2);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);

    layoutItem = ui->board->itemAtPosition(anchorR+1, anchorC-3);
    cell = (CellWidget*)dynamic_cast<QWidgetItem*>(layoutItem)->widget();
    viewModel->toggleAlive(cell->getPoint().r, cell->getPoint().c);
}








