#include "specwave.h"
#include <QPainter>

#include <QGraphicsRectItem>
#include <QGraphicsView>

#define DEBUG_SPEC    1

#if DEBUG_SPEC
#include <QDebug>
#endif

#define WIDGET_WIDTH        (802)
#define WIDGET_HEIGHT       (602)
#define OUTLINE_WIDTH       (2)

SpecWave::SpecWave(QWidget *parent) : QWidget(parent)
{
    resize(WIDGET_WIDTH,WIDGET_HEIGHT);

    p_scene = new QGraphicsScene(this);
    p_background = new SpecBackground(WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);
    p_curve = new SpecCurve(WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);

    p_scene->setSceneRect(0, 0, WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);
    p_scene->setBackgroundBrush(QColor(0, 0, 0, 255));
    p_scene->addItem(p_background);
    p_scene->addItem(p_curve);

    QGraphicsView *view = new QGraphicsView(p_scene, this);
    view->resize(WIDGET_WIDTH ,WIDGET_HEIGHT);
    view->show();
    qDebug() << p_scene->itemAt(50, 50, QTransform());

    /*!
     * parameter set
     *
     */
    str_disValCfg.i16_ref = 10;
    str_disValCfg.f64_resolution = 1;
}


void SpecWave::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawLine(QPoint(50,20), QPoint(100, 100));
}

