/* libfft.c - fast Fourier transform library
**
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#include <engine/libfft.h>
#include <cmath>



/* initfft - initialize for fast Fourier transform
**
** b    power of two such that 2**nu = number of samples
*/
FFT::FFT(int b ) {
    {
        register int i, j, k;

        bits = b;
        if ( bits > LOG2_MAXFFTSIZE )
        {
            return;
        }

        for ( i = ( 1 << bits ) - 1; i >= 0; --i )
        {
            k = 0;
            for ( j = 0; j < bits; ++j )
            {
                k *= 2;
                if ( i & ( 1 << j ) )
                    k += 1;
            }
            bitreverse[i] = k;
        }
    }
}

/* fft - a fast Fourier transform routine
**
** xr   real part of data to be transformed
** xi   imaginary part (normally zero, unless inverse transform in effect)
** inv  flag for inverse
*/

void FFT::exec(float xr[],float xi[],bool inv )
{
    int n, n2, i, k, kn2, l, p;
    float ang, s, c, tr, ti;
    double ds, dc;

    n = 1 << bits;
    n2 = n / 2;

    for ( l = 0; l < bits; ++l )
    {
        for ( k = 0; k < n; k += n2 )
        {
            for( i = 0; i < n2; ++i, ++k )
            {
                p = bitreverse[k / n2];
                ang = 6.283185 * p / n;

                c = cos( ang );
                s = sin( ang );

                kn2 = k + n2;
                if ( inv )
                    s = -s;
                tr = xr[kn2] * c + xi[kn2] * s;
                ti = xi[kn2] * c - xr[kn2] * s;
                xr[kn2] = xr[k] - tr;
                xi[kn2] = xi[k] - ti;
                xr[k] += tr;
                xi[k] += ti;
            }
        }
        n2 /= 2;
    }

    for ( k = 0; k < n; ++k )
    {
        i = bitreverse[k];
        if ( i <= k )
            continue;
        tr = xr[k];
        ti = xi[k];
        xr[k] = xr[i];
        xi[k] = xi[i];
        xr[i] = tr;
        xi[i] = ti;
    }

    /* Finally, multiply each value by 1/n, if this is the forward transform. */
    if ( ! inv )
    {
        register float f;

        f = 1.0 / n;
        for( i = 0; i < n ; ++i )
        {
            xr[i] *= f;
            xi[i] *= f;
        }
    }
}
/* fft - a fast Fourier transform routine
**
** x  real and image buffer [r i r i ....]
** inv  flag for inverse
*/

void FFT::execSingleBuf(float x[],bool inv )
{
    int n, n2, i, k, kn2, l, p;
    float ang, s, c, tr, ti;
    //double ds, dc;

    n = 1 << bits;
    n2 = n / 2;

    for ( l = 0; l < bits; ++l )
    {
        for ( k = 0; k < n; k += n2 )
        {
            for( i = 0; i < n2; ++i, ++k )
            {
                p = bitreverse[k / n2];
                ang = 6.283185 * p / n;

                c = cos( ang );
                s = sin( ang );

                kn2 = k + n2;
                if ( inv )
                    s = -s;
                tr = x[2 * kn2] * c + x[2 * kn2 + 1] * s;
                ti = x[2 * kn2 + 1] * c - x[2 * kn2] * s;
                x[2 * kn2] = x[2 * k] - tr;
                x[2 * kn2 + 1] = x[2 * k + 1] - ti;
                x[2 * k] += tr;
                x[2 * k +1] += ti;
            }
        }
        n2 /= 2;
    }

    for ( k = 0; k < n; ++k )
    {
        i = bitreverse[k];
        if ( i <= k )
            continue;
        tr = x[2 * k];
        ti = x[2 * k + 1];
        x[2 * k] = x[ 2 * i];
        x[2 * k + 1] = x[2 * i + 1];
        x[2 * i] = tr;
        x[2 * i + 1] = ti;
    }

    /* Finally, multiply each value by 1/n, if this is the forward transform. */
    if ( ! inv )
    {
        register float f;

        f = 1.0 / n;
        for( i = 0; i < n ; ++i )
        {
            x[2 * i] *= f;
            x[2 * i + 1] *= f;
        }
    }
}
