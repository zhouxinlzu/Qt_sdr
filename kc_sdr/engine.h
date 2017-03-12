#ifndef ENGINE_H
#define ENGINE_H

#include <QThread>

class Engine : public QThread
{
    Q_OBJECT
public:
    explicit Engine(QObject * parent = 0);
    void stop();
protected:
    void run();
private:
    volatile bool b_stopped;
};

#endif // ENGINE_H
