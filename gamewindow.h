#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "cell.h"
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
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::GameWindow *ui;
    GameViewModel viewModel;

public slots:
    void toggleAlive(Cell *cell);
    void playStop();
    void next();
    void resetClear();
};
#endif // GAMEWINDOW_H
