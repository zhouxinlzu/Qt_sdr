#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "engine/engine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
private slots:
    void on_action_SPEC_triggered();
    void status_connected(void);
    void status_disconnected(void);

    void on_line_freq_editingFinished();
private:
    Ui::MainWindow *ui;
    Engine *p_engineThread;
    QLabel *p_leftStatus;

    bool freqGet(const char *pi8_buf, quint64 *pu64_freq);
};

#endif // MAINWINDOW_H
