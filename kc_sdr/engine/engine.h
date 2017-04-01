#ifndef ENGINE_H
#define ENGINE_H

#include <QThread>
#include <QVector>
#include "engine/interface.h"
#include "spec/mousetrace.h"
#include "thirdPartLib/fftw3/fftw3.h"

class Engine : public QThread
{
    Q_OBJECT
public:
    explicit Engine(QObject * parent = 0);
    void stop();
    void engineStatusTrace(void);
    qint16          i16_simAmp;
    quint16         u16_simFreq;
    void recvBufSet(void);
signals:
    void fftGenerated(intptr_t u32_addr, quint16 u16_size);
    void connected2Device(void);
    void disconnected2Device(void);
protected:
    void run();
private:
    Interface           *p_interfaceTcp;
    volatile bool       b_stopped;
    quint16             u16_fftSize;

    float               *pf32_IQ;
    QVector<float>      dataIqBuf1;
    QVector<float>      dataIqBuf2;
    QVector<float>      fftBuf;

    qint16              *pi16_IQ;
    QVector<qint16>     i16_dataIqBuf1;
    QVector<qint16>     i16_dataIqBuf2;

    fftw_complex        *p_fftIn;
    fftw_complex        *p_fftOut;
    fftw_plan           fftPlan;

    bool                b_isConnected;
    bool                b_isBufferOne;
    bool                b_isFftValid;
    dataTypeEnum        en_iqType;
    void doFft(void);
    void resetFftSize(quint16 u16_size);
    void refFftDisplay(void);
private slots:
    void startGetIq(void);
    void iqGet(void);
};

#endif // ENGINE_H
