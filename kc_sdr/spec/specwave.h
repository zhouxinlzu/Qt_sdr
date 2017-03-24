#ifndef SPECWAVE_H
#define SPECWAVE_H

#include <QWidget>
#include <QGraphicsScene>
#include "specbackground.h"
#include "speccurve.h"
#include "specscene.h"
#include "specview.h"

struct disValCfgStr
{
    qint16  i16_ref;
    qreal   f64_resolution;
    quint16 u16_width;
    quint16 u16_height;
    float   f32_min;
};
class SpecWave : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWave(QWidget *parent = 0);

signals:

public slots:
    void recvFftValue(quint32 u32_addr, quint16 u16_size);
private:
    SpecView *p_view;
    SpecBackground *p_background;
    SpecCurve *p_curve;
    SpecScene *p_scene;
    struct disValCfgStr str_disValCfg;
    QPointF *p_curveDisBuf;
    bool    b_isMalloc;
};

#endif // SPECWAVE_H
