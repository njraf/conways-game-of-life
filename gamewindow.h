#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include "cellWidget.h"
#include "gameviewmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    GameViewModel *viewModel;
    const int DIMENSIONS = 30;

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
