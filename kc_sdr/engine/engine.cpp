#include "engine.h"
#include <QtMath>
#include <cstdint>

#define ENGINE_DBG  1
#define SIMULATE   0

#if ENGINE_DBG
#include <QDebug>
#endif

#define IQ_PAIR     (204800)

Engine::Engine(QObject *parent):QThread(parent)
{
    p_interfaceTcp = new Interface;

    en_iqType = p_interfaceTcp->dataTypeGet();
    resetFftSize(1024);
    recvBufSet();


    p_interfaceTcp->connect2Server();
    b_isBufferOne = true;
    b_isConnected = false;
    b_stopped = false;
    b_isFftValid = false;


    i16_simAmp = 10;
    u16_simFreq = 100;

    connect(p_interfaceTcp, &Interface::startTransfer, this, &Engine::startGetIq);
    connect(p_interfaceTcp, &Interface::receivedPacket, this, &Engine::iqGet);
}
void Engine::run()
{
    while(b_stopped == false)
    {
        engineStatusTrace();
#if SIMULATE
        iqGet();
        doFft();
#else

        if(b_isConnected == true)
        {
            refFftDisplay();
        }
        else
        {

        }
#endif
        msleep(50);
    }
    b_stopped = false;
#if ENGINE_DBG
    qDebug() << "engine thread stop!";
#endif
}
void Engine::stop()
{
    p_interfaceTcp->disConnect2Server();
    b_stopped = true;
    while(Engine::isRunning() == true)
    {

    }
}

void Engine::engineStatusTrace()
{
    if(p_interfaceTcp->connectStatus() == true)
    {
        b_isConnected = true;
        emit connected2Device();
    }
    else
    {
        b_isConnected = false;
        emit disconnected2Device();
    }
}

void Engine::recvBufSet(void)
{
    switch(en_iqType)
    {
    case INT_16:
    {
        p_interfaceTcp->recvBufSet((intptr_t)i16_dataIqBuf1.data());
    }break;
    case FLOAT_32:
    {
        p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf1.data());
    }break;
    default:break;
    }
}

void Engine::resetFftSize(quint16 u16_size)
{
    u16_fftB = qFloor(qLn(u16_size) / qLn(2));
    u16_fftSize = qRound(qPow(2, u16_fftB));

    alg_fft = FFT(u16_fftB);
    p_interfaceTcp->dataIqPair(IQ_PAIR);
    switch(en_iqType)
    {
    case INT_16:
    {
        i16_dataIqBuf1.resize(2 * IQ_PAIR);
        i16_dataIqBuf2.resize(2 * IQ_PAIR);
        dataIqBuf1.resize(2 * IQ_PAIR);
        fftBuf.resize(IQ_PAIR);
        pi16_IQ = i16_dataIqBuf1.data();
        pf32_IQ = dataIqBuf1.data();
#if ENGINE_DBG
    qDebug() << "buf1 addr" << i16_dataIqBuf1.data() << "buf2 addr" << i16_dataIqBuf2.data();
    qDebug() << "get ffB:" << u16_fftB << " fftSize:" << u16_fftSize;
#endif
    }break;
    case FLOAT_32:
    {
        dataIqBuf1.resize(2 * IQ_PAIR);
        dataIqBuf2.resize(2 * IQ_PAIR);
        fftBuf.resize(IQ_PAIR);
        pf32_IQ = dataIqBuf1.data();
#if ENGINE_DBG
    qDebug() << "buf1 addr" << dataIqBuf1.data() << "buf2 addr" << dataIqBuf2.data();
    qDebug() << "get ffB:" << u16_fftB << " fftSize:" << u16_fftSize;
#endif
    }break;
    default:break;
    }



}

void Engine::startGetIq()
{
#if ENGINE_DBG
    qDebug() << "send command: get iq";
#endif
    p_interfaceTcp->send2Server(CMD_GET_IQ, 0);
}

void Engine::iqGet()
{
#if SIMULATE
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
#else
    if(b_isBufferOne == true)
    {
        b_isBufferOne = false;
        switch(en_iqType)
        {
        case INT_16:
        {
           p_interfaceTcp->recvBufSet((intptr_t)i16_dataIqBuf2.data());
           pi16_IQ = i16_dataIqBuf1.data();
        } break;
        case FLOAT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf2.data());
            pf32_IQ = dataIqBuf1.data();
        } break;
        default:break;
        }
    }
    else
    {
        b_isBufferOne = true;
        switch(en_iqType)
        {
        case INT_16:
        {
           p_interfaceTcp->recvBufSet((intptr_t)i16_dataIqBuf1.data());
           pi16_IQ = i16_dataIqBuf2.data();
        } break;
        case FLOAT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf1.data());
            pf32_IQ = dataIqBuf2.data();
        } break;
        default:break;
        }
    }
    doFft();
#endif
}

void Engine::doFft()
{

    switch(en_iqType)
    {
    case INT_16:
    {
        for(int i = 0; i < IQ_PAIR; i++)
        {
            pf32_IQ[2 * i] = pi16_IQ[2 * i];
            pf32_IQ[2 * i + 1] = pi16_IQ[2 * i + 1];
        }
        //add window function here
//        for(int i = 0; i < IQ_PAIR / u16_fftSize; i++)
//        {
//            alg_fft.execSingleBuf(pf32_IQ + u16_fftSize * i, false);
//        }
        alg_fft.execSingleBuf(pf32_IQ, false);
    }break;
    case FLOAT_32:
    {
        //add window function here
        alg_fft.execSingleBuf(pf32_IQ, false);
    }break;
    }
    for(int i = 0; i < IQ_PAIR / u16_fftSize; i++)
    {
        float *pf32_buf = pf32_IQ + i * u16_fftSize;
        uint32_t u32_index = i * u16_fftSize;
        for(int j = 0; j < u16_fftSize; j++)
        {
            //! log(sqrt(x)) = (1/2)log(x)
            float f32_lev = pf32_buf[2 * j] * pf32_buf[2 * j] + pf32_buf[2 * j + 1] * pf32_buf[2 * j + 1];
            //!sqrt(a)--> dB = 20 * log(a) = 10 * log(a)
            fftBuf[u32_index + j] = 10 * log(f32_lev);
        }
    }
    b_isFftValid = true;
    qDebug() << "fft end";
}

void Engine::refFftDisplay()
{
    if(b_isFftValid == true)
    {
        b_isFftValid = false;
        emit fftGenerated((intptr_t)fftBuf.data(), u16_fftSize );
    }
}
