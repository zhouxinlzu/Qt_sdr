#ifndef SPECMARKER_H
#define SPECMARKER_H
#include <QGraphicsItem>
#include <QPen>
#include "mousetrace.h"


class SpecMarker : public QGraphicsItem
{
public:
    SpecMarker(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void waveValGet(intptr_t valAddr);
private:
    qreal   f64_penWidth;
    QPen    pen;
    QRectF  *p_rect;
    struct disValCfgStr *pstr_disVal;
};

#endif // SPECMAKER_H
