#include "specwave.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QDebug>
SpecWave::SpecWave(QWidget *parent) : QWidget(parent)
{
    QWidget::resize(1000,1000);
    QGraphicsScene *scene = new QGraphicsScene(this);
scene->setSceneRect(0, 0, 400, 300);
    QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 100, 100);
    scene->addItem(item);
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->resize(500,500);
    view->show();
    qDebug() << scene->itemAt(50, 50, QTransform());
}


void SpecWave::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawLine(QPoint(50,20), QPoint(100, 100));
}

