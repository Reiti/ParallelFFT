/**Basic DFT implementation
  *@author Michael Reitgruber
*/

#include <math.h>
#include <stdio.h>
#include <complex.h>

#define PI 3.14159265358979323846


/*Calculates the DFT of the input vector using the naive O(N^2) algorithm*/
void dft(double complex *in, double complex *out, int len);

int main(int argc, char *argv[])
{
    //TODO Handle generator input and different size arrays

    double complex in[8] = {1,2,3,4,5,6,7,8};
    double complex out[8] = {0};
    dft(in, out, 8);
    for(int i=0; i<8; i++) {
        printf("%.4f %+.4fi\n", creal(out[i]), cimag(out[i]));
    }

}

void dft(double complex *in, double complex *out, int len)
{
    for(int i=0; i<len; i++) {
        double complex acc = 0 + 0*I;
        for(int j=0; j<len; j++) {
            acc+=in[j] * cexp(((-2) * PI * I * i * j)/len);;
        }
        out[i]=acc;
    }
}

