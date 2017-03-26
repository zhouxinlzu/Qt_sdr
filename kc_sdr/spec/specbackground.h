#ifndef SPECBACKGROUND_H
#define SPECBACKGROUND_H

#include <QGraphicsItem>
#include <QPen>
#include "spec/mousetrace.h"

class SpecBackground : public QGraphicsItem
{
public:
    SpecBackground(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void waveValGet(intptr_t valAddr);
public slots:

private:
    qreal   f64_penWidth;
    struct disValCfgStr *pstr_disVal;
    QRectF  *p_rect;
    QPen    pen;
};

#endif // SPECBACKGROUND_H
