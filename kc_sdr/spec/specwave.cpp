#include "specwave.h"
#include <QPainter>
#include <QHBoxLayout>
#define DEBUG_SPEC    1

#if DEBUG_SPEC
#include <QDebug>
#endif

#define OUTLINE_WIDTH       (2)

SpecWave::SpecWave(QWidget *parent) : QWidget(parent)
{
    /*!
     * parameter set
     *
     */
    str_disValCfg.i16_ref = 10;
    str_disValCfg.f64_resolution = 0.1f;
    str_disValCfg.u16_width = 512;
    str_disValCfg.u16_height = 400;
    str_disValCfg.f32_min = str_disValCfg.i16_ref - str_disValCfg.u16_height * str_disValCfg.f64_resolution;
    b_isMalloc = false;

    p_scene = new SpecScene(this);
    p_background = new SpecBackground(str_disValCfg.u16_width, str_disValCfg.u16_height);
    p_curve = new SpecCurve(str_disValCfg.u16_width, str_disValCfg.u16_height);
    p_scene->setSceneRect(0, 0, str_disValCfg.u16_width , str_disValCfg.u16_height );

    p_scene->setBackgroundBrush(QColor(0, 0, 0, 255));
    p_scene->addItem(p_background);
    p_scene->addItem(p_curve);

    p_view = new SpecView;
    p_view->setScene(p_scene);
    p_view->resize(str_disValCfg.u16_width + OUTLINE_WIDTH, str_disValCfg.u16_height + OUTLINE_WIDTH);
    p_view->displayAreaGet(str_disValCfg.u16_width + OUTLINE_WIDTH, str_disValCfg.u16_height + OUTLINE_WIDTH);
    p_view->show();

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(p_view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    /*!
     * connect
     */
}

void SpecWave::recvFftValue(quint32 u32_addr, quint16 u16_size)
{
    float *pf32_fftBuf = (float *)u32_addr;
#if DEBUG_SPEC
//    qDebug() << "widget size:" << SpecWave::width() << SpecWave::height();
//    qDebug() << "view size:" << p_view->width() << p_view->height();
//    qDebug() << "curve size:" << p_curve->boundingRect();
//    qDebug() << "background size:" << p_background->boundingRect();
//    qDebug() << "scene size:" << p_scene->width() << p_scene->height();
#endif
    if(b_isMalloc == true)
    {
        b_isMalloc = false;
        free(p_curveDisBuf);
    }
    p_curveDisBuf = (QPointF *)malloc(sizeof(QPointF) * u16_size);
    if(p_curveDisBuf != NULL)
    {
        b_isMalloc = true;
        for(quint16 i = 0; i < u16_size; i++)
        {
            if(i == 100)
            qDebug() << "fftval" << pf32_fftBuf[i];
            qint16 i16_lev = str_disValCfg.u16_height - (pf32_fftBuf[i] - str_disValCfg.f32_min) / str_disValCfg.f64_resolution;
            if(i16_lev >=  str_disValCfg.u16_height)
            {
                i16_lev =  str_disValCfg.u16_height;
            }
            else if(i16_lev < 0)
            {
                i16_lev = 0;
            }
            p_curveDisBuf[i].setX(i);
            p_curveDisBuf[i].setY(i16_lev);
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

