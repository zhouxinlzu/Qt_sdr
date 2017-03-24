#ifndef SPECVIEW_H
#define SPECVIEW_H

#include <QGraphicsView>

class SpecView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SpecView(QWidget *parent = 0);
    void displayAreaGet(quint16 u16_width, quint16 u16_height);
protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:

public slots:
private:
    quint16     u16_disWidth;
    quint16     u16_disHeight;
};

#endif // SPECVIEW_H
