#ifndef MOUSETRACE_H
#define MOUSETRACE_H

#include <QGraphicsItem>
#include <QPen>
#include <cstdint>

struct disValCfgStr
{
    float   f32_disRef;
    float   f32_disMin;

    qreal   f64_ampResolution;
    qreal   f64_ampResolutionMax;
    qreal   f64_ampResolutionMin;

    qreal   f64_freqResolution;

    quint16 u16_width;
    quint16 u16_height;

    float   *pf32_fftBuf;

    qreal   f64_strFreq;

    quint8  u8_hlNum;
    quint8  u8_vlNum;

    quint8  u8_ampRulerWidth;
    quint8  u8_freqRulerHeight;
};
class MouseTrace : public QGraphicsItem
{
public:
    MouseTrace(quint16 u16_width, quint16 u16_height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void waveValGet(intptr_t valAddr);
private:
    bool    b_mouseIn;
    qreal   f64_penWidth;
    qreal   f64_mouseX;
    qreal   f64_mouseY;
    QRectF  *p_rect;
    QPen    pen;
    QPointF  mouse_offset;
    struct disValCfgStr *pstr_disVal;
    void waveValDis(QPainter *painter);
    bool waveAmpResolutionAdd(qreal f64_val);
    bool waveAmpResolutionsub(qreal f64_val);

};

#endif // MOUSETRACE_H
