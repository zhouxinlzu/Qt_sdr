#ifndef ENGINE_H
#define ENGINE_H

#include <QThread>
#include <QVector>
#include "engine/interface.h"

#define USE_FFTW   0
#define SIMULATE   0

#if USE_FFTW
#include "thirdPartLib/fftw3/fftw3.h"
#endif

#define KHZ         (1000)
#define MHZ         (1000 * KHZ)
#define GHZ         (1000 * MHZ)



class Engine : public QThread
{
    Q_OBJECT
public:
    explicit Engine(QObject * parent = 0);
    void stop();
    void engineStatusTrace(void);
    void recvBufSet(void);
    void resetFftSize(quint16 u16_size);
    void freqSet(quint64 u64_freq);
    void prescxi(preEnum en_pre);
    void ifAmp(ifEnum en_if);
    void sideBand(sbEnum en_sb);
signals:
    void fftGenerated(intptr_t u32_addr, quint16 u16_size);
    void connected2Device(void);
    void disconnected2Device(void);
protected:
    void run();
private:
    void doFft(void);
    void refFftDisplay(void);

    Interface           *p_interfaceTcp;
    volatile bool       b_stopped;
    quint16             u16_fftSize;

    float               *pf32_IQ;
    QVector<float>      dataIqBuf1;
    QVector<float>      dataIqBuf2;
    QVector<float>      fftBuf;

    quint32             *pu32_fftBuf;
    QVector<quint32>     u32_dataFftBuf1;
    QVector<quint32>     u32_dataFftBuf2;

#if USE_FFTW
    fftw_complex        *p_fftIn;
    fftw_complex        *p_fftOut;
    fftw_plan           fftPlan;
#endif

    bool                b_isConnected;
    bool                b_isBufferOne;
    bool                b_isFftValid;
    dataTypeEnum        en_iqType;
#if SIMULATE
    qint16          i16_simAmp;
    quint16         u16_simFreq;
#endif
private slots:
    void startGetIq(void);
    void iqGet(void);
};

#endif // ENGINE_H
