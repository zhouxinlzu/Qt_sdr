#include "dockspec.h"

DockSpec::DockSpec(QWidget *parent):QDockWidget(parent)
{
    waveform = new SpecWave(this);
}
