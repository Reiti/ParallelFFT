/**Complex numbers
  *@author Michael Reitgruber
*/

#include <math.h>
#include <stdio.h>
#include <complex.h>

#define PI 3.14159265358979323846


/*Calculates the DFT of the input vector using the naive O(N^2) algorithm*/
void dft(double complex *out, double complex *in, int len);

int main(int argc, char *argv[])
{
    //TODO Handle generator input
    double complex z1 = 1.0 + 3.0 * I;
    double complex z2 = 1.0 - 4.0 * I;
    double complex sum = z1 + z2;
    double complex W = cexp(PI * I);
    double complex in[9] = {1,2,3,4,5,6,7,8,9};
    double complex out[9] = {0};
    dft(out, in, 9);
    for(int i=0; i<9; i++) {
        printf("%.4f %+.4fi\n", creal(out[i]), cimag(out[i]));
    }

}

void dft(double complex *out, double complex *in, int len)
{
    double norm = sqrt(len);
    for(int i=0; i<len; i++) {
        double complex acc = 0 + 0*I;
        for(int j=0; j<len; j++) {
            acc+=in[j] * cexp(((-2) * PI * I * i * j)/len);;
        }
        out[i]=acc/norm;
    }
}

