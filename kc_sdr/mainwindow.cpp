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


    connect(p_engineThread, &Engine::fftGenerated, ui->waveform, &SpecWave::recvFftValue, Qt::QueuedConnection);

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

void MainWindow::on_ampSlider_valueChanged(int value)
{
    p_engineThread->i16_simAmp = value;
    ui->ampLineEdit->setText(QString::number(value, 10));
}

void MainWindow::on_freqSlider_valueChanged(int value)
{
    p_engineThread->u16_simFreq = value;
    ui->freqLineEdit->setText(QString::number(value, 10));
}

void MainWindow::on_ampLineEdit_editingFinished()
{
    p_engineThread->i16_simAmp = ui->ampLineEdit->text().toInt();
    ui->ampSlider->setValue(p_engineThread->i16_simAmp);
}

void MainWindow::on_freqLineEdit_editingFinished()
{
    p_engineThread->u16_simFreq = ui->freqLineEdit->text().toInt();
    ui->freqSlider->setValue( p_engineThread->u16_simFreq);
}
