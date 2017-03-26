#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_ampSlider_valueChanged(int value);

    void on_freqSlider_valueChanged(int value);

    void on_ampLineEdit_editingFinished();

    void on_freqLineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    Engine *p_engineThread;
};

#endif // MAINWINDOW_H
