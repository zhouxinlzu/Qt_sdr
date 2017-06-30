#include "engine.h"
#include <QtMath>
#include <cstdint>
#define ENGINE_DBG  1
#define BOARD_SIMULATE 1

#if ENGINE_DBG
#include <QDebug>
#include <QTime>
#endif

#define IQ_PAIR     (1024)

Engine::Engine(QObject *parent):QThread(parent)
{
    p_interfaceTcp = new Interface;

    en_iqType = p_interfaceTcp->dataTypeGet();
    resetFftSize(1024);
    recvBufSet();

#if !SIMULATE
    p_interfaceTcp->connect2Server();
#endif
    b_isBufferOne = true;
    b_isConnected = false;
    b_stopped = false;
    b_isFftValid = false;

#if SIMULATE
    i16_simAmp = 1000;
    u16_simFreq = 10;
#endif

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
        refFftDisplay();
#else

        if(b_isConnected == true)
        {
            refFftDisplay();
        }
        else
        {

        }
#endif
        msleep(30);
    }
    b_stopped = false;
#if ENGINE_DBG
    qDebug() << "engine thread stop!";
#endif
}
void Engine::stop()
{
    p_interfaceTcp->send2Server(CMD_STOP_IQ, 0);
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
    case FLOAT_32:
    {
        p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf1.data());
    }break;
    case UINT_32:
    {
        p_interfaceTcp->recvBufSet((intptr_t)u32_dataFftBuf1.data());
    }break;
    default:break;
    }
}

void Engine::resetFftSize(quint16 u16_size)
{
#if USE_FFTW
    if((p_fftIn != NULL) || (p_fftOut != NULL))
    {

        fftw_destroy_plan(fftPlan);
        fftw_free(p_fftIn);
        fftw_free(p_fftIn);
    }
#endif
    u16_fftSize = u16_size;
    qDebug() << "set fftsize = " << u16_fftSize;
#if USE_FFTW
    p_fftIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * u16_fftSize);
    p_fftOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * u16_fftSize);
    fftPlan = fftw_plan_dft_1d(u16_fftSize, p_fftIn, p_fftOut, FFTW_FORWARD, FFTW_MEASURE);
#endif

    p_interfaceTcp->dataIqPair(IQ_PAIR);
    dataIqBuf1.resize(2 * IQ_PAIR);
    fftBuf.resize(IQ_PAIR);

    pf32_IQ = dataIqBuf1.data();
    switch(en_iqType)
    {
    case FLOAT_32:
    {
        dataIqBuf2.resize(2 * IQ_PAIR);
    }break;
    case UINT_32:
    {
        u32_dataFftBuf1.resize(IQ_PAIR);
        u32_dataFftBuf2.resize(IQ_PAIR);
        pu32_fftBuf = u32_dataFftBuf1.data();
    }break;
    default:break;
    }
}

void Engine::freqSet(quint64 u64_freq)
{
    qDebug() << "set frequency = " << u64_freq;
    p_interfaceTcp->send2Server(CMD_SET_FREQ, u64_freq);
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

    for(int j = 0; j < IQ_PAIR / u16_fftSize; j++)
    {
        float *pf32_buf = pf32_IQ + j * u16_fftSize * 2;
        for(int i = 0; i < u16_fftSize; i++)
        {
            float nphase = noise * ((float)qrand() / RAND_MAX - 0.5f);
            pf32_buf[2 * i] = (float)i16_simAmp * 0.001f  * cos(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize) + 0.01 * cos(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize + nphase);
            pf32_buf[2 * i + 1] = (float)i16_simAmp * 0.001f * sin(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize)+ 0.01 * sin(2 * M_PI * (float)i * u16_simFreq / (float)u16_fftSize + nphase);
        }
    }
#else
    if(b_isBufferOne == true)
    {
        b_isBufferOne = false;
        switch(en_iqType)
        {
        case FLOAT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf2.data());
            pf32_IQ = dataIqBuf1.data();
        } break;
        case UINT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)u32_dataFftBuf2.data());
            pu32_fftBuf = u32_dataFftBuf1.data();
        }
        default:break;
        }
    }
    else
    {
        b_isBufferOne = true;
        switch(en_iqType)
        {
        case FLOAT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)dataIqBuf1.data());
            pf32_IQ = dataIqBuf2.data();
        } break;
        case UINT_32:
        {
            p_interfaceTcp->recvBufSet((intptr_t)u32_dataFftBuf1.data());
            pu32_fftBuf = u32_dataFftBuf2.data();
        }break;
        default:break;
        }
    }
    doFft();
#endif
}

void Engine::doFft()
{
#if SIMULATE
    #if USE_FFTW
    for(int j = 0; j < IQ_PAIR / u16_fftSize; j++)
    {
        float *pf32_buf = pf32_IQ + j * u16_fftSize * 2;
        for(int i = 0 ; i < u16_fftSize; i++)
        {
          p_fftIn[i][0] =   pf32_buf[2 * i];
          p_fftIn[i][1] =   pf32_buf[2 * i + 1];
        }
        fftw_execute(fftPlan);
        for(int i = 0; i < u16_fftSize; i++)
        {
            //float f32_hanni = 0.5 * (1 - cos(2*M_PI*i/(u16_fftSize -1)));
            pf32_buf[2 * i] =  p_fftOut[i][0];
            pf32_buf[2 * i + 1] =  p_fftOut[i][1];
        }
    }
    for(int i = 0; i < IQ_PAIR / u16_fftSize; i++)
    {
        float *pf32_buf = pf32_IQ + i * u16_fftSize * 2;
        uint32_t u32_index = i * u16_fftSize;
        for(int j = 0; j < u16_fftSize; j++)
        {
            //! log(sqrt(x)) = (1/2)log(x)
            float f32_lev = pf32_buf[2 * j] * pf32_buf[2 * j] + pf32_buf[2 * j + 1] * pf32_buf[2 * j + 1];
            //!sqrt(a)--> dB = 20 * log(a) = 10 * log(a)
            fftBuf[u32_index + j] = 10 * log(f32_lev);
        }
    }
    #else
    for(int i = 0; i < IQ_PAIR / u16_fftSize; i++)
    {
        float *pf32_buf = pf32_IQ + i * u16_fftSize * 2;
        uint32_t u32_index = i * u16_fftSize;
        for(int j = 0; j < u16_fftSize; j++)
        {
            fftBuf[u32_index + j] = pf32_buf[2 * j];
        }
    }
    #endif
#else
    switch(en_iqType)
    {
    case FLOAT_32:
    {
        for(int j = 0; j < IQ_PAIR / u16_fftSize; j++)
        {
#if USE_FFTW
            float *pf32_buf = pf32_IQ + j * u16_fftSize * 2;
            for(int i = 0 ; i < u16_fftSize; i++)
            {
              p_fftIn[i][0] =   pf32_buf[2 * i];
              p_fftIn[i][1] =   pf32_buf[2 * i + 1];
            }
            fftw_execute(fftPlan);
            for(int i = 0; i < u16_fftSize; i++)
            {
                //float f32_hanni = 0.5 * (1 - cos(2*M_PI*i/(u16_fftSize -1)));
                pf32_buf[2 * i] =  p_fftOut[i][0];
                pf32_buf[2 * i + 1] =  p_fftOut[i][1];
            }
#endif
        }
    }break;
    case UINT_32:
    {
#if BOARD_SIMULATE
                for(int i = 0; i < IQ_PAIR; i++)
                {
                    fftBuf[i] = (float)pu32_fftBuf[i];
                }
#else
                for(int i = 0; i < 1024; i++)
                {
        //            if(pu32_fftBuf[index]  < pu32_fftBuf[i])
        //            {
        //                index = i;
        //            }
                    if((i >= 40)&&(i <= 60))
                     qDebug() << pu32_fftBuf[i];
                }

                for(int i = 0; i < IQ_PAIR; i++)
                {

                    int integer = (int)(pu32_fftBuf[i] >> 16);
                    int fractional = (int)(pu32_fftBuf[i] & 0xffff);
                    fftBuf[i] = (float)integer + (float)fractional / 65536;
                    //fftBuf[i] *= 20;
                }
#endif
    }break;
    default:break;
    }
    if(en_iqType != UINT_32)
    {
        for(int i = 0; i < IQ_PAIR / u16_fftSize; i++)
        {
            float *pf32_buf = pf32_IQ + i * u16_fftSize * 2;
            uint32_t u32_index = i * u16_fftSize;
            for(int j = 0; j < u16_fftSize; j++)
            {
                //! log(sqrt(x)) = (1/2)log(x)
                float f32_lev = pf32_buf[2 * j] * pf32_buf[2 * j] + pf32_buf[2 * j + 1] * pf32_buf[2 * j + 1];
                //!sqrt(a)--> dB = 20 * log(a) = 10 * log(a)
                fftBuf[u32_index + j] = 10 * log(f32_lev);
            }
        }
    }
#endif

    b_isFftValid = true;
}

void Engine::refFftDisplay()
{
    if(b_isFftValid == true)
    {
        b_isFftValid = false;
        emit fftGenerated((intptr_t)fftBuf.data(), u16_fftSize );
    }
}
