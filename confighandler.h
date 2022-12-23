#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "cell.h"

class ConfigHandler : public QObject
{
    Q_OBJECT
public:
    ~ConfigHandler();
    static ConfigHandler *getInstance();
    void saveBoard(QString fileName, std::vector<Cell*> *liveCells, std::vector<Cell*> *pendingCells, std::vector<Cell*> *initialCells, std::vector<Cell*> *prevCells);
    void loadBoard(QString fileName, QStringList &lines);
    QStringList getConfigNames();

private:
    ConfigHandler();

    static ConfigHandler *instance;
    QDir dir;
};

#endif // CONFIGHANDLER_H
