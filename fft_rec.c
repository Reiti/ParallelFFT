/**Non-Parallel recursive FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include "prints.h"


#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len, int offset, int step);

int main(int argc, char *argv[])
{
    //TODO Handle generator input and different size arrays
    double complex in[8] = {1,2,3,4,5,6,7,8};
    double complex out[8] = {0};
    fft(in, out, 8, 0, 1);
    for(int i = 0; i<8; i++) {
        printf("%f %+fi\n", creal(out[i]), cimag(out[i]));
    }

}


void fft(double complex *in, double complex *out, int len, int offset, int step)
{
    if(len == 1) {
      out[0] = in[offset];
      return;
    }

    double complex *evenT = out;
    double complex *oddT = out+len/2;
    fft(in, evenT, len/2, offset, step*2);
    fft(in, oddT, len/2, offset+step, step*2);

    for(int i=0; i<len/2; i+=1) {
      double complex twiddle = cexp(-2*PI*I*i/len) * oddT[i];

      out[i] = evenT[i] + twiddle;
      out[i + len/2] = evenT[i] -twiddle;
    }

}
