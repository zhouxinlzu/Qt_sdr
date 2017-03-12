#include "engine.h"
#include <QDebug>

Engine::Engine(QObject *parent):QThread(parent)
{
    b_stopped = false;
}
void Engine::run()
{
    qreal i= 0;
    while(b_stopped == false)
    {
        qDebug() << QString("in MyThread: %1").arg(i);
        msleep(1000);
        i++;
    }
    b_stopped = false;
}
void Engine::stop()
{
    b_stopped = true;
}
