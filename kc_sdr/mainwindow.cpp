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
    p_leftStatus = new QLabel(this);
    ui->statusBar->addWidget(p_leftStatus);
    status_disconnected();

    connect(p_engineThread, &Engine::fftGenerated, ui->waveform, &SpecWave::recvFftValue, Qt::QueuedConnection);
    connect(p_engineThread, &Engine::connected2Device, this, &MainWindow::status_connected);
    connect(p_engineThread, &Engine::disconnected2Device, this, &MainWindow::status_disconnected);

    p_engineThread->start();


    ui->ampLineEdit->setText(QString::number(ui->ampSlider->value(), 10));
    ui->freqLineEdit->setText(QString::number(ui->freqSlider->value(), 10));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    p_engineThread->stop();
}

void MainWindow::on_action_SPEC_triggered()
{
    ui->SpecWidget->show();

}

void MainWindow::status_connected()
{
    p_leftStatus->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    p_leftStatus->setText("Connected to device.");
}

void MainWindow::status_disconnected()
{
    p_leftStatus->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    p_leftStatus->setText("Can't connect to device.");
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
