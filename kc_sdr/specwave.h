#ifndef SPECWAVE_H
#define SPECWAVE_H

#include <QWidget>
#include <QGraphicsScene>
#include "specbackground.h"
#include "speccurve.h"

struct disValCfgStr
{
    qint16  i16_ref;
    qreal   f64_resolution;
};
class SpecWave : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWave(QWidget *parent = 0);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
private:
    SpecBackground *p_background;
    SpecCurve *p_curve;
    QGraphicsScene *p_scene;
    struct disValCfgStr str_disValCfg;
};

#endif // SPECWAVE_H
