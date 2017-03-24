#include "specview.h"
#include <QResizeEvent>
#include <QGraphicsItem>
#include <QMouseEvent>
#define VIEW_DBG 1

#if VIEW_DBG
#include <QDebug>
#endif

SpecView::SpecView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void SpecView::displayAreaGet(quint16 u16_width, quint16 u16_height)
{
    u16_disHeight = u16_height;
    u16_disWidth = u16_width;
}

void SpecView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

#if VIEW_DBG
    qDebug() << "width = "<< width() <<"scale" << width() / (float)u16_disWidth;
#endif
    QGraphicsView::resetMatrix();
    scale(QGraphicsView::width() / (float)u16_disWidth, QGraphicsView::height() / (float)u16_disHeight);
}

void SpecView::mousePressEvent(QMouseEvent *event)
{
    QPoint viewPos = event->pos();
    qDebug() << "viewPos:" << viewPos;
    QPointF scenePos = mapToScene(viewPos);
    qDebug() << "scenePos:" << scenePos;
    QGraphicsItem *item = scene()->itemAt(scenePos,QTransform());
    if(item)
    {
        QPointF itemPos = item->mapFromScene(scenePos);
        qDebug() << "itemPos:" << itemPos;
    }
}
