#include "specmarker.h"
#include <QDebug>

SpecMarker::SpecMarker(quint16 u16_width, quint16 u16_height)
{
    f64_penWidth = 1;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        u16_width + f64_penWidth,
                        u16_height + f64_penWidth);
    pen = QPen(Qt::white, 0, Qt::DashDotLine, Qt::SquareCap, Qt::BevelJoin);
}

QRectF SpecMarker::boundingRect() const
{
    QRectF  rect = *p_rect;

    return rect;
}
void SpecMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void SpecMarker::waveValGet(intptr_t valAddr)
{
    pstr_disVal = (struct disValCfgStr *)valAddr;
}
