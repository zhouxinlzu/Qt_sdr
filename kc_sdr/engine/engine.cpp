#include "engine.h"
#include <QtMath>

#define ENGINE_DBG  1

#if ENGINE_DBG
#include <QDebug>
#endif

Engine::Engine(QObject *parent):QThread(parent)
{
    b_stopped = false;
    resetFftSize(1024);

    i16_simAmp = 10;
    u16_simFreq = 100;
}
void Engine::run()
{
    while(b_stopped == false)
    {
        iqGet();
        doFft();
        msleep(50);
    }
    b_stopped = false;
}
void Engine::stop()
{
    b_stopped = true;
}

void Engine::resetFftSize(quint16 u16_size)
{
    u16_fftB = qFloor(qLn(u16_size) / qLn(2));
    u16_fftSize = qRound(qPow(2, u16_fftB));

    dataIqBuf1.resize(2 * u16_fftSize);
    dataIqBuf2.resize(2 * u16_fftSize);
    fftBuf.resize(u16_fftSize);
    alg_fft = FFT(u16_fftB);
    pf32_IQ = dataIqBuf1.data();

#if ENGINE_DBG
    qDebug() << "get ffB:" << u16_fftB << " fftSize:" << u16_fftSize;
#endif
}

void Engine::iqGet()
{
    static float noise = 0.7;

    for(int i = 0; i < u16_fftSize; i++)
    {
        float nphase = noise * ((float)qrand() / RAND_MAX - 0.5f);
        pf32_IQ[2 * i] = (float)i16_simAmp * 0.001f  * cos(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize) + 5 * cos(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize + nphase);
        pf32_IQ[2 * i + 1] = (float)i16_simAmp * 0.001f * sin(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize)+ 5 * sin(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize + nphase);
    }
#if ENGINE_DBG
   // qDebug() << "Frequency(" << freq << ")";
#endif
}

void Engine::doFft()
{
    //add window function here
    alg_fft.execSingleBuf(pf32_IQ, false);
    for(int i = 0; i < u16_fftSize; i++)
    {
        float f32_lev = sqrt(pf32_IQ[2 * i] * pf32_IQ[2 * i] + pf32_IQ[2 * i + 1] * pf32_IQ[2 * i + 1]);
        fftBuf[i] = 10 * log(f32_lev);
    }

#if ENGINE_DBG
#endif
    emit fftGenerated((quint32)fftBuf.data(), fftBuf.size() / 2);
}
