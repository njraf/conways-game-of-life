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
    //connect(ui->speedSlider, SIGNAL(valueChanged(int value)), &viewModel, SLOT(setSpeed(int speed)));

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
    viewModel.printLiveCells();

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

            QWidget *item = new Cell(x, y);
            QString styleSheet = item->styleSheet();
            if (!contains(viewModel.getLiveCells(), (Cell*)item)) {
                styleSheet = "background-color: #DCDCDC;";
            } else {
                styleSheet = "background-color: yellow;";
            }
            item->setStyleSheet(styleSheet);

            QObject::connect(item, SIGNAL(clicked(Cell*)), this, SLOT(toggleAlive(Cell*)));

            ui->board->addWidget(item, x, y);
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






