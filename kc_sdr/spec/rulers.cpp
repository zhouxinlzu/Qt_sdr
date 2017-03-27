#include "rulers.h"
#include <QPainter>
#include "mousetrace.h"
#include <QDebug>

Rulers::Rulers(quint16 u16_width, quint16 u16_height, bool b_ruler)
{
    f64_penWidth = 1;
    p_rect = new QRectF(0 - f64_penWidth / 2,
                        0 - f64_penWidth / 2,
                        u16_width + f64_penWidth,
                        u16_height + f64_penWidth);
    pen = QPen(Qt::gray, 0, Qt::DashDotDotLine, Qt::SquareCap, Qt::BevelJoin);

    b_isAmpRuler = b_ruler;
}

void Rulers::waveValGet(intptr_t valAddr)
{
    pstr_disVal = (struct disValCfgStr *)valAddr;
}

QRectF Rulers::boundingRect() const
{
    QRectF  rect = *p_rect;

    return rect;
}

void Rulers::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(b_isAmpRuler == true)
    {
        quint16 u16_height = p_rect->height();
        quint16 u16_width = p_rect->width();
        qreal   f64_hlDiv = u16_height / (pstr_disVal->u8_hlNum + 1);

        painter->setPen(pen);

        for(quint8 i = 0; i < pstr_disVal->u8_hlNum; i++)
        {
            QRectF pos;
            pos.setX(0);
            //pos.setY((i + 0.5f) * f64_hlDiv);
            pos.setY(i * f64_hlDiv);

            float f32_amp = pstr_disVal->f32_disRef - pstr_disVal->f64_ampResolution *
                            (i + 1) * f64_hlDiv;
            qDebug() <<"rect" << *p_rect << "pos "<< pos.y() <<"hldiv" << f64_hlDiv <<"amp" << f32_amp;
            //painter->drawText(pos, QString::number(f32_amp,'f', 2));
            painter->drawText(pos, "12");
            painter->drawLine(0, (i + 1) * f64_hlDiv, u16_width, (i + 1) * f64_hlDiv);
        }
    }
    else
    {

    }
}
