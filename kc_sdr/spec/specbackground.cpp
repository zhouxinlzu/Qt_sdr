#include "specbackground.h"
#include <QPainter>

#define DEBUG_BACKGROUND    1

#define HL_NUMBER           (5)
#define VL_NUMBER           (5)

#if DEBUG_BACKGROUND
#include <QDebug>
#endif

SpecBackground::SpecBackground(quint16 u16_width, quint16 u16_height)
{
    f64_penWidth = 1;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        u16_width + f64_penWidth,
                        u16_height + f64_penWidth);
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
    qreal   f64_hlDiv = u16_height / (HL_NUMBER + 1);
    qreal   f64_vlDiv = u16_width / (VL_NUMBER + 1);

    QPen outline(Qt::gray, 0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    painter->setPen(outline);
    QRectF rectangle = *p_rect;
    painter->drawRect(rectangle);
    painter->setPen(pen);

    for(quint8 i = 0; i < HL_NUMBER; i++)
    {
        painter->drawLine(0, (i + 1) * f64_hlDiv, u16_width, (i + 1) * f64_hlDiv);
    }
    for(quint8 i = 0; i < VL_NUMBER; i++)
    {
        painter->drawLine((i + 1) * f64_vlDiv, 0, (i + 1) * f64_vlDiv, u16_height);
    }
}

