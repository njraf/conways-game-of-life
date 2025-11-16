
#include <QDebug>
#include "confighandler.h"

ConfigHandler *ConfigHandler::instance = nullptr;

ConfigHandler::ConfigHandler()
{
    dir = QDir("configs");
    dir.setFilter(QDir::Files);
    if (!dir.exists()) {
        dir.mkdir(dir.absolutePath());
    }
}

ConfigHandler::~ConfigHandler() {

}

ConfigHandler *ConfigHandler::getInstance() {
    if (instance == nullptr) {
        instance = new ConfigHandler();
    }
    return instance;
}

void ConfigHandler::saveBoard(QString fileName, std::vector<std::shared_ptr<Cell>> *liveCells, std::vector<std::shared_ptr<Cell>> *pendingCells, std::vector<std::shared_ptr<Cell>> *initialCells, std::vector<std::shared_ptr<Cell>> *prevCells) {
    if (!dir.exists()) {
        dir.mkdir(dir.absolutePath());
    }
    QFile file(dir.dirName() + '/' + fileName);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream textStream(&file);

    for (auto cell : *liveCells) {
        textStream << "L," << cell->getPoint().r << ',' << cell->getPoint().c << '\n';
    }

    for (auto cell : *pendingCells) {
        textStream << "N," << cell->getPoint().r << ',' << cell->getPoint().c << '\n';
    }

    for (auto cell : *initialCells) {
        textStream << "I," << cell->getPoint().r << ',' << cell->getPoint().c << '\n';
    }

    for (auto cell : *prevCells) {
        textStream << "P," << cell->getPoint().r << ',' << cell->getPoint().c << '\n';
    }

    file.close();
}

void ConfigHandler::loadBoard(QString fileName, QStringList &lines) {
    if (!dir.exists()) {
        dir.mkdir(dir.absolutePath());
    }

    if (!fileName.endsWith(".txt")) {
        fileName.append(".txt");
    }

    QFile file(dir.dirName() + '/' + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd()) {
        lines.push_back(file.readLine());
    }
}

QStringList ConfigHandler::getConfigNames() {
    if (!dir.exists()) {
        return QStringList();
    }
    QStringList fileNames;
    QFileInfoList files = dir.entryInfoList();
    for (auto file : files) {
        fileNames.push_back(file.fileName().split('.')[0]);
    }

    return fileNames;
}

