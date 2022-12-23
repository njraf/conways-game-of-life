#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include <QInputDialog>
#include "cellWidget.h"
#include "gameviewmodel.h"
#include "loadconfigdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void makeBoard();

private:
    Ui::GameWindow *ui;
    GameViewModel *viewModel;
    int ROWS = 30;
    int COLS = ROWS + (ROWS / 2);

public slots:
    void toggleAlive(CellWidget *cell); // only for player's toggle
    void playStop();
    void next();
    void resetClear();
    void draw();
    void clearBoard();

    // menu actions
    void generateRandom();
    void generateRPentomino();
    void generateDieHard();
    void generateAcorn();
};
#endif // GAMEWINDOW_H
