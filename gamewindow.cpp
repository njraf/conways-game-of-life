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

