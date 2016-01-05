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
#define PI 3.14159265358979323846
typedef char* str;
void fft(double complex *in, double complex *out, int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);
void print_cmplx_ar(double complex* cpar, int fmtl, int horz);
void print_comp(double complex* cpar1, double complex* cpar2);
void print_cmpl_cell(double complex* cpar,int fmtl, int ind);
int main(int argc, char *argv[])
{
    //TODO Handle generator input and different size arrays

    double complex in[8] = {1,2,3,4,5,6,7,8};
    double complex out[8] = {0};
	(void)printf("Processing FFT of Input:\n");print_cmplx_ar(in,10, 0);
    fft(in, out, 8);
	(void)printf("Result:\n");
	print_cmplx_ar(out,10, 1);
    print_comp(in, out);
}

void print_comp(double complex* cpar1, double complex* cpar2){
	for(int i = 0; i<8; i++) {
		print_cmpl_cell(cpar1, 10, i);	
		(void)printf("-> ");
		print_cmpl_cell(cpar2, 10, i);	
		(void)printf("\n");
    }
	(void)printf("\n");
}
void print_cmpl_cell(double complex* cpar,int fmtl, int ind){
	float a = creal(cpar[ind]);
		str as = (str)malloc(25);  
		if(ceilf(a)==a){//print number without retarded floating points
			 (void)sprintf(as, "[%*i ",fmtl, (int)a);
			//(void)sprintf(as,"%8s",as);
		}else{
			 (void)sprintf(as, "[%*f ",fmtl, a);
			//(void)sprintf(as,"%8s",as);
		}
		float b = cimag(cpar[ind]);
		str bs = (str)malloc(25);
		if(ceilf(b)==b){
			 (void)sprintf(bs, "%+*ii] ",fmtl, (int)b);
		}else{
			 (void)sprintf(bs, "%+*fi] ",fmtl,  b);
		}
		str res = (str)malloc(50);
		res = strcat(as,bs);
		(void)printf("%s",res);
		/*free(res);*//*gibt error... :(*/
		free(&bs[0]);free(&res[0]);
}
void print_cmplx_ar(double complex* cpar, int fmtl, int horz){
	for(int i = 0; i<8; i++) {
		print_cmpl_cell(cpar, fmtl,i);
		if(horz){
			(void)printf("\n");
		}
    }
	if(!horz)
		(void)printf("\n");
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
		if(len %2 ==1)
			return 0;
		len>>=1;
	}
	return 1;
}
void fft(double complex *in, double complex *out, int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/
	if((sizeof(in)/sizeof(in[0]) != len && !ispow2(len) )){
		(void)fprintf(stderr,"ERROR: wrong length... must be power of 2");
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
