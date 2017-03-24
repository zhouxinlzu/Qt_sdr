#ifndef SPECCURVE_H
#define SPECCURVE_H

#include <QGraphicsItem>
#include <QPen>

class SpecCurve : public QGraphicsItem
{
public:
    SpecCurve(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect()const;
    void curveGet(QPointF  *p_buf, quint16 u16_size);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:

private:
    qreal   f64_penWidth;
    QRectF  *p_rect;
    QPointF  *p_curve;
    quint16 u16_curveSize;
    QPen    pen;
};

#endif // SPECCURVE_H
