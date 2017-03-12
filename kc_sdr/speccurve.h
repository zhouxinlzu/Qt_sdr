#ifndef SPECCURVE_H
#define SPECCURVE_H

#include <QGraphicsItem>
#include <QPen>

class SpecCurve : public QGraphicsItem
{
public:
    SpecCurve(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect()const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal   f64_penWidth;
    QRectF  *p_rect;
    QPen    pen;
};

#endif // SPECCURVE_H
