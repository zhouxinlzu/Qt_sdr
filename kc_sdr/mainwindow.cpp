#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spec/specwave.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->hide();
    p_engineThread = new Engine(this);


    connect(p_engineThread, &Engine::fftGenerated, ui->SpecWidget->waveform, &SpecWave::recvFftValue, Qt::QueuedConnection);

    p_engineThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_SPEC_triggered()
{
    ui->SpecWidget->show();

}
