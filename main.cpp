#include "gamewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.show();
    QFile file(":/colors.qss");
    if(file.open(QFile::ReadOnly)) {
       QString StyleSheet = QLatin1String(file.readAll());
       a.setStyleSheet(StyleSheet);
    }
    return a.exec();
}
