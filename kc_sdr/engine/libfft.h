#ifndef LIBFFT_H
#define LIBFFT_H

class FFT
{
public:
    static const int MAXFFTSIZE = 32768;
    static const int LOG2_MAXFFTSIZE = 15;

    FFT(int b = 1);
    void exec(float xr[], float xi[], bool inv);
    void execSingleBuf(float x[], bool inv);
private:
    int bitreverse[MAXFFTSIZE], bits;
};

#endif // LIBFFT_H

