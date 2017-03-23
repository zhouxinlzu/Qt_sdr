#ifndef ENGINE_H
#define ENGINE_H

#include <QThread>
#include <QVector>
#include "engine/libfft.h"

class Engine : public QThread
{
    Q_OBJECT
public:
    explicit Engine(QObject * parent = 0);
    void stop();
signals:
    void fftGenerated(quint32 u32_addr, quint16 u16_size);
protected:
    void run();
private:
    volatile bool   b_stopped;
    quint16         u16_fftSize;
    quint16         u16_fftB;
    float           *pf32_IQ;
    QVector<float>  dataIqBuf1;
    QVector<float>  dataIqBuf2;
    QVector<float>  fftBuf;
    FFT             alg_fft;
    void iqGet(void);
    void doFft(void);
    void resetFftSize(quint16 u16_size);
};

#endif // ENGINE_H