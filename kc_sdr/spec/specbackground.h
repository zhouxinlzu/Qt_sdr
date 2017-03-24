#ifndef SPECBACKGROUND_H
#define SPECBACKGROUND_H

#include <QGraphicsItem>
#include <QPen>

class SpecBackground : public QGraphicsItem
{
public:
    SpecBackground(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:

private:
    qreal   f64_penWidth;
    QRectF  *p_rect;
    QPen    pen;
};

#endif // SPECBACKGROUND_H
