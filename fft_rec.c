/**Non-Parallel recursive FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>

#define SINDEX(x,i) (x).data[(x).offset + (x).step * i]

typedef struct {
    double complex *data;
    int step;
    int offset;
} step_vec;

/*Implements the Cooley-Tukey algorithm for the FFT recursively*/
void fft(step_vec *in, double complex *out, int len);

int main(int argc, char *argv[])
{
    double complex in_c[4] = {1,2,3,4};
    double complex out[4] = {0};
    step_vec in;
    in.data = in_c;
    in.step = 1;
    in.offset= 0;
    fft(&in, out, 4);
    for(int i=0; i<2; i++) {
        double complex val = out[i];
        printf("%f %+f\n", creal(val), cimag(val));
    }
}

void fft(step_vec *in, double complex *out, int len)
{
    if(len == 1) {
        out[0] = SINDEX(*in,0);
        return;
    }

    double complex *fh, *sh;
    fh = out;
    sh = out+len/2;

    step_vec fh_s, sh_s;
    fh_s.data = in->data;
    fh_s.offset = in->offset;
    fh_s.step = in->step*2;

    sh_s.data = in->data;
    sh_s.offset = in->offset + in->step;
    sh_s.step = in->step*2;

   //TODO: Insert actual FFT

}
