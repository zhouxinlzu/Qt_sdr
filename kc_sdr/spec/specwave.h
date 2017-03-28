#ifndef SPECWAVE_H
#define SPECWAVE_H

#include <QWidget>
#include <QGraphicsScene>
#include "specbackground.h"
#include "speccurve.h"
#include "specscene.h"
#include "specview.h"
#include "mousetrace.h"
#include "rulers.h"


class SpecWave : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWave(QWidget *parent = 0);
signals:

public slots:
    void recvFftValue(quint32 u32_addr, quint16 u16_size);
private:
    SpecView            *p_view;
    SpecBackground      *p_background;
    SpecCurve           *p_curve;
    SpecScene           *p_scene;
    Rulers              *p_ampRuler;
    Rulers              *p_freqRuler;
    struct disValCfgStr str_disValCfg;
    QPointF             *p_curveDisBuf;
    MouseTrace          *p_mouse;
    bool                b_isMalloc;
};

#endif // SPECWAVE_H
