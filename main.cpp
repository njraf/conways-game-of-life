#include "gamewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.setWindowTitle("Conway's Game of Life");
    w.showMaximized();
    return a.exec();
}
