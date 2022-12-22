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
    connect(viewModel, &GameViewModel::gameStarted, this, [=] {
        ui->playButton->setText("Stop");
        ui->resetButton->setText("Reset");
    });
    connect(viewModel, &GameViewModel::gameStopped, this, [=] { ui->playButton->setText("Start"); });
    connect(viewModel, &GameViewModel::resetBoard, this, [=] {
        clearBoard();
        ui->turnCounter->setText("0");
        if (!viewModel->isPlaying()) {
            ui->resetButton->setText("Clear");
        }
        draw();
    });
    connect(viewModel, &GameViewModel::clearBoard, this, [=] { clearBoard(); });

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
    } else {
        viewModel->play();
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
    } else {
        viewModel->clear();
    }
}

void GameWindow::draw() {
    // get UI cells at (r,c) from live and prev cell lists
    // an intercection of prev and live will remain alive
    // the remaining prev cell should die, the remaining live cells should resurrect
    std::vector<Cell*> *liveCells = viewModel->getLiveCells();
    std::vector<Cell*> *prevCells = viewModel->getPrevCells();
    std::vector<Cell*> cellsToKill(prevCells->size());
    std::vector<Cell*> cellsToRes(liveCells->size());
    auto killIt = std::copy_if(prevCells->begin(), prevCells->end(), cellsToKill.begin(), [=](Cell *cell) {
        // copy if cell not found in both prev and live cell lists
        auto liveIt = std::find_if(liveCells->begin(), liveCells->end(), [=](Cell *otherCell) {
            return ((cell->getPoint().r == otherCell->getPoint().r) && (cell->getPoint().c == otherCell->getPoint().c));
        });
        return (liveIt == liveCells->end());
    });
    auto resIt = std::copy_if(liveCells->begin(), liveCells->end(), cellsToRes.begin(), [=](Cell *cell) {
        // copy if cell not found in both prev and live cell lists
        auto prevIt = std::find_if(prevCells->begin(), prevCells->end(), [=](Cell *otherCell) {
            return ((cell->getPoint().r == otherCell->getPoint().r) && (cell->getPoint().c == otherCell->getPoint().c));
        });
        return (prevIt == prevCells->end());
    });

    cellsToKill.resize(std::distance(cellsToKill.begin(), killIt));
    cellsToRes.resize(std::distance(cellsToRes.begin(), resIt));

    // kill cells
    for (auto k : cellsToKill) {
        Point point = k->getPoint();
        if ((point.r >= DIMENSIONS) || (point.c >= DIMENSIONS) || (point.r < 0) || (point.c < 0))
            continue;

        QLayoutItem *layoutItem = ui->board->itemAtPosition(point.r, point.c);
        QWidget *item = dynamic_cast<QWidgetItem*>(layoutItem)->widget();
        QString styleSheet = item->styleSheet();
        styleSheet = "background-color: #DCDCDC;";
        item->setStyleSheet(styleSheet);
    }

    // resurrect cells
    for (auto res : cellsToRes) {
        Point point = res->getPoint();
        if ((point.r >= DIMENSIONS) || (point.c >= DIMENSIONS) || (point.r < 0) || (point.c < 0))
            continue;

        QLayoutItem *layoutItem = ui->board->itemAtPosition(point.r, point.c);
        QWidget *item = dynamic_cast<QWidgetItem*>(layoutItem)->widget();
        QString styleSheet = item->styleSheet();
        styleSheet = "background-color: yellow;";
        item->setStyleSheet(styleSheet);
    }
}

void GameWindow::clearBoard() {
    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QLayoutItem *layoutItem = ui->board->itemAtPosition(x, y);
            QWidget *item = dynamic_cast<QWidgetItem*>(layoutItem)->widget();
            QString styleSheet = item->styleSheet();
            styleSheet = "background-color: #DCDCDC;";
            item->setStyleSheet(styleSheet);
        }
    }
}


// menu actions

void GameWindow::generateRandom() {
    if (viewModel->isPlaying()) return;

    viewModel->reset();
    viewModel->clear();
    clearBoard();

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
    clearBoard();

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
    clearBoard();

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
    clearBoard();

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








