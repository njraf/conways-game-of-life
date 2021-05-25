#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playStop()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetClear()));

    //viewModel.placeCell(new Cell(0,0));
    //viewModel.placeCell(new Cell(-1,0));
    //viewModel.placeCell(new Cell(0,1));
    //viewModel.placeCell(new Cell(0,-1));
    //viewModel.printLiveCells();

    const int DIMENSIONS = 10;

    for (int y = 0; y < DIMENSIONS; y++) {
        for (int x = 0; x < DIMENSIONS; x++) {
            QWidget *cell = new Cell(x, y, this);
            QString styleSheet = cell->styleSheet();
            styleSheet += "background-color: red; border-color: solid green 2px"; // #DCDCDC
            cell->setStyleSheet(styleSheet);

            QObject::connect(cell, SIGNAL(clicked(Cell*)), this, SLOT(toggleAlive(Cell*)));

            ui->board->addWidget(cell, x, y);
            //viewModel.placeCell((Cell*)cell);
        }
    }
    viewModel.printLiveCells();


    int it = 0;
    while (it < 4) {
        //viewModel.tick();
        it++;
    }
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::toggleAlive(Cell *cell) {
    //QWidget *cell2 = new Cell(cell->getPoint().x, cell->getPoint().y);
    Cell *item = (Cell*)ui->board->itemAtPosition(cell->getPoint().x, cell->getPoint().y)->widget();
    QString styleSheet = item->styleSheet();
    styleSheet += "background-color: yellow";
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

void GameWindow::resetClear() {
    if (viewModel.getTurn() == 0) {
        viewModel.reset();
        if (!viewModel.isPlaying()) {
            ui->resetButton->setText("Clear");
        }
    } else {
        viewModel.clear();
        ui->resetButton->setText("Reset");
    }
}






