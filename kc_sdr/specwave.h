#ifndef SPECWAVE_H
#define SPECWAVE_H

#include <QWidget>

class SpecWave : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWave(QWidget *parent = 0);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SPECWAVE_H
