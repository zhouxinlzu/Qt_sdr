#include "specbackground.h"
#include <QPainter>

#define DEBUG_BACKGROUND    1

#if DEBUG_BACKGROUND
#include <QDebug>
#endif

SpecBackground::SpecBackground()
{
    f64_penWidth = 1;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        100 + f64_penWidth,
                        100 + f64_penWidth);
    pen = QPen(Qt::gray, 0, Qt::DashDotDotLine, Qt::SquareCap, Qt::BevelJoin);
}

QRectF SpecBackground::boundingRect() const
{
    QRectF  rect = *p_rect;

    return rect;
}

void SpecBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    quint16 u16_height = p_rect->height();
    quint16 u16_width = p_rect->width();

#if DEBUG_BACKGROUND
    qDebug() << "get background height" << u16_height << "width" << u16_width <<endl;
#endif
    painter->setPen(pen);
    painter->drawLine(0, 20, u16_width, 20);
}
