#ifndef DOCKSPEC_H
#define DOCKSPEC_H

#include <QDockWidget>
#include "specwave.h"

class DockSpec : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockSpec(QWidget *parent = 0);
    SpecWave *waveform;
private:

};

#endif // DOCKSPEC_H
