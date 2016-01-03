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

int main(int argc, char *argv[])
{
    double complex in[8] = {1,2,3,4,5,6,7,8};
    double complex out[8] = {0};
    //fft(in, out, 8);

    for(int i=0; i<8; i++) {
        printf("%d\n", reverse(i, 3));
    }
   /* for(int i = 0; i<8; i++) {
        printf("%f %+fi\n", creal(out[i]), cimag(out[i]));
    }*/
}

uint32_t reverse(uint32_t index, int size)
{
    uint32_t reversed=0;

    for (int i = 0; i < size; ++i) {
        reversed |= ((index >> i) & 1) << (size - i - 1);
    }
    return reversed;
}
