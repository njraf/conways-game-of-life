#include "gamethread.h"

GameThread::GameThread()
{
    gameSpeed = 0;
    playing = false;
}

void GameThread::run() {
    while (playing) {



        msleep(1000 - gameSpeed * 100);
    }

}

void GameThread::setPlaying(bool p) {
    playing = p;
}

void GameThread::setSpeed(int speed) {
    gameSpeed = speed;
}
