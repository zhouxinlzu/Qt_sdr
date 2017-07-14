#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spec/specwave.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! 数据产生引擎
    p_engineThread = new Engine(this);
    //! 显示连接状态
    p_leftStatus = new QLabel(this);
    ui->statusBar->addWidget(p_leftStatus);
    status_disconnected();

    //! 连接信号与槽
    connect(p_engineThread, &Engine::fftGenerated, ui->waveform, &SpecWave::recvFftValue, Qt::QueuedConnection);
    connect(p_engineThread, &Engine::connected2Device, this, &MainWindow::status_connected);
    connect(p_engineThread, &Engine::disconnected2Device, this, &MainWindow::status_disconnected);

    //! 启动引擎
    p_engineThread->start();

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

bool MainWindow::freqGet(const char *pi8_buf, quint64 *pu64_freq)
{
    bool b_return = true;
    char *pi8_unit = NULL;

    qreal f64_freq = strtod(pi8_buf, &pi8_unit);
    const char *ci8_par[3] = {"G", "M", "k"};
    qint8 i8_index = -1;
    quint8 i = 0;
    for(i = 0; i < 3; i++)
    {
        if(strcasecmp(pi8_unit, ci8_par[i]) == 0)
        {
            i8_index = i;
            break;
        }
    }
    quint64 u64_freq = 0;
    switch (i8_index)
    {
    case 0: u64_freq = (quint64)(f64_freq * GHZ);break;
    case 1: u64_freq = (quint64)(f64_freq * MHZ);break;
    case 2: u64_freq = (quint64)(f64_freq * KHZ);break;
    default: u64_freq = (quint64)(f64_freq);break;
    }

    *pu64_freq = u64_freq;

    return b_return;
}

void MainWindow::on_line_freq_editingFinished()
{
    qDebug() << qPrintable(ui->line_freq->text());

    quint64 u64_freq = 0;
    freqGet(qPrintable(ui->line_freq->text()), &u64_freq);
    qDebug() << u64_freq;
    p_engineThread->freqSet(u64_freq);
}

void MainWindow::on_sideband_lsb_clicked()
{

    qDebug() << "lsb selected";
    p_engineThread->sideBand(SB_LSB);
}

void MainWindow::on_sideband_usb_clicked()
{

    qDebug() << "usb selected";
    p_engineThread->sideBand(SB_USB);
}

void MainWindow::on_if_unamp_clicked()
{

    qDebug() << "if unamp selected";
    p_engineThread->ifAmp(IF_UNAMP);
}

void MainWindow::on_if_ampx1_clicked()
{
    qDebug() << "if amp * 1 selected";
    p_engineThread->ifAmp(IF_AMP_S);
}

void MainWindow::on_if_ampx2_clicked()
{

    qDebug() << "if amp * 2 selected";
    p_engineThread->ifAmp(IF_AMP_D);
}

void MainWindow::on_pre_amp_clicked()
{

    qDebug() << "pre amp selected";
    p_engineThread->prescxi(PRE_AMP);
}

void MainWindow::on_pre_att_clicked()
{

    qDebug() << "pre att selected";
     p_engineThread->prescxi(PRE_ATT);
}

void MainWindow::on_pre_unatt_clicked()
{

    qDebug() << "pre unatt selected";
    p_engineThread->prescxi(PRE_UNATT);
}
