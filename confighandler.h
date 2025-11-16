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
    void saveBoard(QString fileName, std::vector<std::shared_ptr<Cell>> *liveCells, std::vector<std::shared_ptr<Cell>> *pendingCells, std::vector<std::shared_ptr<Cell>> *initialCells, std::vector<std::shared_ptr<Cell>> *prevCells);
    void loadBoard(QString fileName, QStringList &lines);
    QStringList getConfigNames();

private:
    ConfigHandler();

    static ConfigHandler *instance;
    QDir dir;
};

#endif // CONFIGHANDLER_H
