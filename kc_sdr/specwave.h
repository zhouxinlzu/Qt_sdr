#ifndef SPECWAVE_H
#define SPECWAVE_H

#include <QWidget>
#include <QGraphicsScene>
#include "specbackground.h"

class SpecWave : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWave(QWidget *parent = 0);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
private:
    SpecBackground *p_background;
    QGraphicsScene *p_scene;
};

#endif // SPECWAVE_H
