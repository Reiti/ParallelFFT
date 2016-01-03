/**Non-Parallel recursive FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>

#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);

int main(int argc, char *argv[])
{
    //TODO Handle generator input and different size arrays

    double complex in[8] = {1,2,3,4,5,6,7,8};
    double complex out[8] = {0};
    fft(in, out, 8);
    for(int i = 0; i<8; i++) {
        printf("%f %+fi\n", creal(out[i]), cimag(out[i]));
    }

}

void fft(double complex *in, double complex *out, int len)
{
    double complex evens[len/2], odds[len/2];
    double complex evensT[len/2], oddsT[len/2];

    if(len == 1) {
        out[0] = in[0];
        return;
    }

    for(int i=0; i<len/2; i++) {
        evens[i] = in[2*i];
        odds[i] = in[2*i + 1];
    }

    fft(evens, evensT, len/2);
    fft(odds, oddsT, len/2);

    for(int i=0; i<len/2; i++) {
        oddsT[i] = oddsT[i]*cexp(((-2)*PI*i*I)/len);
    }

    for(int i=0; i<len/2; i++) {
        out[i] = evensT[i] + oddsT[i];
        out[i + len/2] = evensT[i] - oddsT[i];
    }

}
