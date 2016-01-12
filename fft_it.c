/**Non-Parallel iterative FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/


#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "prints.h"
#include "numgenparser.h"


#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);



int main(int argc, char *argv[])
{
    //TODO Handle generator input and different size arrays
	


	
	int len = getNumAmount();
	
	//double complex in[len]; 
    //double complex out[len];	

	double complex * in = (double complex*)malloc(len * sizeof(double complex));
	double complex * out = (double complex*)malloc(len * sizeof(double complex));
	
	int counter = getNumbers(in);

	if(counter != len){
		(void)fprintf(stderr, "wrong number amount in stream\n");
		return 1;
	}
	



	(void)printf("Processing FFT of Input:\n");//print_cmplx_ar(in,10, 0, len);
	
		//printf("----%d %d-----\n",sizeof(in),sizeof(in[0]) );
	(void)printf("fft starts: \n");
    fft(in, out, len);
	(void)printf("fft done! \n");
	free(in);free(out);
	//(void)printf("Result:\n");
	//print_cmplx_ar(out,10, 1,len);
    //print_comp(in, out,len);
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
int ispow2(int len){
	while(len !=0){
		if(len %2 ==1 && len /2 != 0)
			return 0;
		len>>=1;
	}
	return 1;
}
void fft(double complex *in, double complex *out, int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/
	if(!ispow2(len) ){
		(void)fprintf(stderr,"ERROR: wrong length... must be power of 2\n");
		exit(1);
	}
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
