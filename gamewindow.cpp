#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "gameviewmodel.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    GameViewModel viewModel;

    viewModel.placeCell(Cell(0,0));
    viewModel.placeCell(Cell(-1,0));
    viewModel.placeCell(Cell(0,1));
    viewModel.placeCell(Cell(0,-1));
    viewModel.printLiveCells();

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            QWidget *cell = new QWidget();
            QString styleSheet = cell->styleSheet();
            styleSheet += "background-color: #DCDCDC";
            cell->setStyleSheet(styleSheet);
            ui->board->addWidget(cell, x, y);
        }
    }



    int it = 0;
    while (it < 4) {
        viewModel.tick();
        it++;
    }
}

GameWindow::~GameWindow()
{
    delete ui;
}

