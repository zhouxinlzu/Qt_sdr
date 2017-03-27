#ifndef RULERS_H
#define RULERS_H

#include <QGraphicsItem>
#include <QPen>

class Rulers : public QGraphicsItem
{
public:
    Rulers(quint16 u16_width, quint16 u16_height, bool b_ruler);
    void waveValGet(intptr_t valAddr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal   f64_penWidth;
    struct disValCfgStr *pstr_disVal;
    QRectF  *p_rect;
    QPen    pen;
    bool    b_isAmpRuler;
};

#endif // RULERS_H
