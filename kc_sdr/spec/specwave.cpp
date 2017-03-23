#include "specwave.h"
#include <QPainter>

#include <QGraphicsRectItem>
#include <QGraphicsView>

#define DEBUG_SPEC    1

#if DEBUG_SPEC
#include <QDebug>
#endif

#define WIDGET_WIDTH        (802)
#define WIDGET_HEIGHT       (602)
#define OUTLINE_WIDTH       (2)

SpecWave::SpecWave(QWidget *parent) : QWidget(parent)
{
    resize(WIDGET_WIDTH,WIDGET_HEIGHT);

    p_scene = new QGraphicsScene(this);
    p_background = new SpecBackground(WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);
    p_curve = new SpecCurve(WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);

    p_scene->setSceneRect(0, 0, WIDGET_WIDTH - OUTLINE_WIDTH, WIDGET_HEIGHT - OUTLINE_WIDTH);
    p_scene->setBackgroundBrush(QColor(0, 0, 0, 255));
    p_scene->addItem(p_background);
    p_scene->addItem(p_curve);

    QGraphicsView *view = new QGraphicsView(p_scene, this);
    view->resize(WIDGET_WIDTH ,WIDGET_HEIGHT);
    view->show();
    qDebug() << p_scene->itemAt(50, 50, QTransform());

    /*!
     * parameter set
     *
     */
    str_disValCfg.i16_ref = 10;
    str_disValCfg.f64_resolution = 0.1f;
    b_isMalloc = false;
}

void SpecWave::recvFftValue(quint32 u32_addr, quint16 u16_size)
{
    float *pf32_fftBuf = (float *)u32_addr;
    quint16 u16_fftSize = u16_size;
#if DEBUG_SPEC
    //qDebug() << "received fft generate signal addr = "<< pf32_fftBuf << "size = " << u16_fftSize ;
#endif
    if(b_isMalloc == true)
    {
#if DEBUG_SPEC
     //   qDebug() << "free display buffer";
#endif
        b_isMalloc = false;
        free(p_curveDisBuf);
    }
    p_curveDisBuf = (QPointF *)malloc(sizeof(QPointF) * u16_size);
    if(p_curveDisBuf != NULL)
    {
        b_isMalloc = true;
        for(quint16 i = 0; i < u16_size; i++)
        {
            quint16 u16_lev = (str_disValCfg.i16_ref - pf32_fftBuf[i]) / str_disValCfg.f64_resolution;
            if(u16_lev >= WIDGET_HEIGHT - OUTLINE_WIDTH)
            {
                u16_lev = WIDGET_HEIGHT - OUTLINE_WIDTH - 1;
            }
            p_curveDisBuf[i].setX(i);
            p_curveDisBuf[i].setY(u16_lev);
        }
        p_curve->curveGet(p_curveDisBuf, u16_size);
        p_curve->update();
    }
    else
    {
#if DEBUG_SPEC
        qDebug() << "can not malloc curve display buffer";
#endif
    }
}

