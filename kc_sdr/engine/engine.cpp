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
    static float phase = 0.0f;
    static float freq = 0.2f;
    static int dir = 1;
    static int chunkSize = 16;
    static float noise = 0.1;
    static float amp = 100;
    static float fstep = 0.1;

    for(int i = 0; i < u16_fftSize; i++)
    {
        pf32_IQ[2 * i] = amp * cos(2 * M_PI * (float)i * 50 / (float)u16_fftSize);
        pf32_IQ[2 * i + 1] = amp * sin(2 * M_PI * (float)i * 50 / (float)u16_fftSize);
    }

    //for(int i = 0; i < u16_fftSize / 2; i++)
    //{
    //    float nphase = noise * ((float)qrand() / RAND_MAX - 0.5f);
    //    fftBuf[i] = amp * sin(phase + nphase);
    //    fftBuf[i] = amp * sin(2 * M_PI * (float)i * 20 / ((float)u16_fftSize / 2));
    //}

    //for(int i = 0; i < chunkSize; i++)
    //{
    //    for(int i = 0; i < u16_fftSize; i++)
    //    {
    //        float nphase = noise * ((float)qrand() / RAND_MAX - 0.5f);

    //        //numerical oscillator
    //        float si = amp * cos(phase + nphase);
    //        float sq = amp * sin(phase + nphase);


    //        pf32_IQ[2 * i] = si;
    //        pf32_IQ[2 * i + 1] = sq;

    //        //phase accumulator
    //        phase += freq;
    //        if(phase > 2 * M_PI) phase -= 2.0f * (float)M_PI;
    //        else if(phase < -2 * M_PI) phase += 2.0f * (float)M_PI;

    //        //triangle freq sweep
    //        freq += fstep * dir;
    //        if(dir > 0)
    //        {
    //            if(freq >=  M_PI)
    //            {
    //                freq =  2*M_PI - freq;
    //                dir = -1;
    //            }
    //        }
    //        else
    //        {
    //            if(freq <= -M_PI)
    //            {
    //                freq = -2*M_PI - freq;
    //                dir = 1;
    //            }
    //        }
    //    }
    //}

#if ENGINE_DBG
    qDebug() << "Frequency(" << freq << ")";
#endif
}

void Engine::doFft()
{
   // add window function here
    alg_fft = FFT(u16_fftB);
    alg_fft.execSingleBuf(pf32_IQ, false);
    for(int i = 0; i < u16_fftSize; i++)
    {
        float f32_lev = sqrt(pf32_IQ[2 * i] * pf32_IQ[2 * i] + pf32_IQ[2 * i + 1] * pf32_IQ[2 * i + 1]);
        //float f32_lev = sqrt(ibuf[2 * i] * ibuf[2 * i] + qbuf[2 * i]* qbuf[2 * i]);
        qDebug() << "lev =" << f32_lev;
        fftBuf[i] = 1 * log(f32_lev);
    }
#if ENGINE_DBG
    //qDebug() << "fft generated->" << fftBuf.data();
#endif
    emit fftGenerated((quint32)fftBuf.data(), fftBuf.size() / 2);
}
