#include "speccurve.h"
#include <QPainter>
#include <QDebug>
SpecCurve::SpecCurve(quint16 u16_width, quint16 u16_height)
{
    f64_penWidth = 1;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        u16_width + f64_penWidth,
                        u16_height + f64_penWidth);
    pen = QPen(Qt::yellow, 0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
}
QRectF SpecCurve::boundingRect() const
{
    QRectF  rect = *p_rect;

    return rect;
}

void SpecCurve::curveGet(QPointF *p_buf, quint16 u16_size)
{
    p_curve = p_buf;
    u16_curveSize = u16_size;
}

void SpecCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen);
    painter->drawPolyline((const QPointF *)p_curve, u16_curveSize);
}

