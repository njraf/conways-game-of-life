#ifndef GAMETHREAD_H
#define GAMETHREAD_H

#include <QThread>

class GameThread : public QThread
{
    Q_OBJECT

private:
    int gameSpeed; // 0-10; (1000ms - gameSpeed * 100) == sleep time between ticks
    bool playing;

public:
    GameThread();
    void run() override;
    void setPlaying(bool p);

public slots:
    void setSpeed(int speed);
};

#endif // GAMETHREAD_H
