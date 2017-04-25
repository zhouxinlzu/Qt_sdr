#include "mousetrace.h"
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
#include <QPainter>
#include <QMouseEvent>

#define MOUSE_DBG   1

#if MOUSE_DBG
#include <QDebug>
#endif
MouseTrace::MouseTrace(quint16 u16_width, quint16 u16_height)
{
    f64_penWidth = 1;
    f64_mouseX = 0;
    f64_mouseY = 0;
    b_mouseIn = false;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        u16_width + f64_penWidth,
                        u16_height + f64_penWidth);
    pen = QPen(Qt::white, 0, Qt::DashDotLine, Qt::SquareCap, Qt::BevelJoin);

    setAcceptHoverEvents(true);
   // setMouseTracking(true);
}

QRectF MouseTrace::boundingRect() const
{
    QRectF  rect = *p_rect;

    return rect;
}

void MouseTrace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(b_mouseIn == true)
    {
        painter->setPen(pen);
        painter->drawLine(0, f64_mouseY, p_rect->width(), f64_mouseY);
        painter->drawLine(f64_mouseX, 0, f64_mouseX, p_rect->height());

        waveValDis(painter);
    }
    else
    {

    }

}

void MouseTrace::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    setCursor(Qt::CrossCursor);
    b_mouseIn = true;
    update();
}

void MouseTrace::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    f64_mouseX = event->pos().x();
    f64_mouseY = event->pos().y();


        QPointF temp = event->pos() - mouse_offset;
        //qDebug() <<"move" << temp.ry();

    update();
}

void MouseTrace::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    b_mouseIn = false;
    update();
}

void MouseTrace::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int val = event->delta();
    qreal f64_adjVal = pstr_disVal->f64_ampResolution / 4;
    if(val > 0)
    {
        waveAmpResolutionsub(f64_adjVal);

    }
    else
    {
        waveAmpResolutionAdd(f64_adjVal);
    }
}

void MouseTrace::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::SizeVerCursor);
        mouse_offset = event->pos();
    }
}

void MouseTrace::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPointF temp = event->pos() - mouse_offset;
        qDebug() <<"move" << temp.ry();
    }
}

void MouseTrace::waveValGet(intptr_t valAddr)
{
    pstr_disVal = (struct disValCfgStr *)valAddr;
}

void MouseTrace::waveValDis(QPainter *painter)
{
    if(pstr_disVal->pf32_fftBuf == NULL)
    {
        return;
    }

    QString p_str("%1Hz,%2dBm");
    qreal f64_freq = pstr_disVal->f64_strFreq + pstr_disVal->f64_freqResolution * qRound(f64_mouseX);
    qreal f64_dBm = pstr_disVal->pf32_fftBuf[qRound(f64_mouseX)];
    p_str = p_str.arg(f64_freq).arg(f64_dBm);

//    QFont font = painter->font();
//    font.setPointSizeF(font.pointSizeF() * 4);
//    painter->setFont(font);

    qreal f64_xscale = scale() / painter->transform().m11();
    qreal f64_yscale = scale() / painter->transform().m22();
    painter->save();
    painter->scale(f64_xscale, f64_yscale);
    QPointF rectangle;
    rectangle.setX(f64_mouseX / f64_xscale);
    rectangle.setY(f64_mouseY / f64_yscale);
    painter->drawText(rectangle, p_str);
    painter->restore();
}

bool MouseTrace::waveAmpResolutionAdd(qreal f64_val)
{
    bool b_return = true;
    qreal f64_resolutionBack = pstr_disVal->f64_ampResolution;
#if MOUSE_DBG
    qDebug() << "resolution add " << f64_val;
#endif
    pstr_disVal->f64_ampResolution += f64_val;
    if(pstr_disVal->f64_ampResolution < pstr_disVal->f64_ampResolutionMax)
    {
        pstr_disVal->f32_disRef = pstr_disVal->f32_disRef - f64_resolutionBack * qRound(f64_mouseY) +\
                                  pstr_disVal->f64_ampResolution * qRound(f64_mouseY);

        pstr_disVal->f32_disMin = pstr_disVal->f32_disRef - pstr_disVal->u16_height * pstr_disVal->f64_ampResolution;
    }
    else
    {
        b_return = false;
        pstr_disVal->f64_ampResolution = pstr_disVal->f64_ampResolutionMax;
    }
#if MOUSE_DBG
    qDebug() << "resolution" << pstr_disVal->f64_ampResolution\
             << "ref" << pstr_disVal->f32_disRef\
             << "min " << pstr_disVal->f32_disMin;
#endif

    return b_return;
}

bool MouseTrace::waveAmpResolutionsub(qreal f64_val)
{
    bool b_return = true;
    qreal f64_resolutionBack = pstr_disVal->f64_ampResolution;
#if MOUSE_DBG
    qDebug() << "resolution sub " << f64_val;
#endif
    pstr_disVal->f64_ampResolution -= f64_val;
    if(pstr_disVal->f64_ampResolution > pstr_disVal->f64_ampResolutionMin)
    {
        pstr_disVal->f32_disRef = pstr_disVal->f32_disRef - f64_resolutionBack * qRound(f64_mouseY) +\
                                  pstr_disVal->f64_ampResolution * qRound(f64_mouseY);

        pstr_disVal->f32_disMin = pstr_disVal->f32_disRef - pstr_disVal->u16_height * pstr_disVal->f64_ampResolution;
    }
    else
    {
        b_return = false;
        pstr_disVal->f64_ampResolution = pstr_disVal->f64_ampResolutionMin;
    }

#if MOUSE_DBG
    qDebug() << "resolution" << pstr_disVal->f64_ampResolution\
             << "ref" << pstr_disVal->f32_disRef\
             << "min " << pstr_disVal->f32_disMin;
#endif

    return b_return;
}
