/**Non-Parallel recursive FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include "prints.h"


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



void fft_help(double complex *swap, double complex *out, int len, int step)
{
    if(step >= len) {
      return;
    }

    fft_help(out, swap, len, step*2); //swap arrays so we can twiddle without crosstalk
    fft_help(out+step, swap+step, len, step*2);

    for(int k=0; k<len/2; k+=step) {
      double complex twiddle = cexp(-2*PI*I*k/len)*out[2*k + step];
      swap[k] = out[2*k] + twiddle;
      swap[k + len/2] = out[2*k] - twiddle;
    }
}

void fft(double complex *in, double complex *out, int len)
{
    for(int i=0; i<len; i++) {
      out[i] = in[i];
    }

    fft_help(out, in, len, 1);
}
