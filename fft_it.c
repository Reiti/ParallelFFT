/**Non-Parallel iterative FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/


#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);

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

int lg(int num)
{
    int c = 0;
    while((num >> 1) != 0) {
        c++;
        num >>= 1;
    }

    return c;
}

uint32_t reverse(uint32_t index, int size)
{
    uint32_t reversed=0;

    for (int i = 0; i < size; ++i) {
        reversed |= ((index >> i) & 1) << (size - i - 1);
    }
    return reversed;
}

void fft(double complex *in, double complex *out, int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/
    for(int i=0; i<len; i++) {
        out[reverse(i,lg(len))] = in[i];
    }

   for(int i = 2; i <= len; i *= 2)  {
        for(int k = 0; k < i/2; k++) {
            double complex omega = cexp(((-2)*k*I*PI)/i);
            for(int j = 0; j < len/i; j++) {
                double complex twiddle = omega * out[j*i + k + i/2];
                out[j*i + k + i/2] = out[j*i + k] - twiddle;
                out[j*i + k] = out[j*i + k] + twiddle;
            }
        }
    }
}
