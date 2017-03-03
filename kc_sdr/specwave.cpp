#include "specwave.h"
#include <QPainter>

#include <QGraphicsRectItem>
#include <QGraphicsView>

#define DEBUG_SPEC    1

#if DEBUG_SPEC
#include <QDebug>
#endif

SpecWave::SpecWave(QWidget *parent) : QWidget(parent)
{
    resize(800,500);

    p_scene = new QGraphicsScene(this);
    p_background = new SpecBackground;

    p_scene->setSceneRect(0, 0, 400, 300);
    p_scene->setBackgroundBrush(QColor(0, 0, 0, 255));
    p_scene->addItem(p_background);

    QGraphicsView *view = new QGraphicsView(p_scene, this);
    view->resize(500,500);
    view->show();
    qDebug() << p_scene->itemAt(50, 50, QTransform());
}


void SpecWave::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawLine(QPoint(50,20), QPoint(100, 100));
}

