/**Parallel iterative FFT implementation of the Cooley-Tukey algorithm using cilk
  *@author Michael Reitgruber
*/


#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include "prints.h"
#include "numgenparser.h"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);



int main(int argc, char *argv[])
{
    char c;
	int p=0;
  int b=0;
  int times=1;
	while((c =getopt(argc, argv, "pb:"))!=-1){
		switch(c){
			case 'p':
				p=1;
				break;
      case 'b':
        b=1;
        times=atoi(optarg);
        break;
			default:
				break;
		}
	}

  if(p == 1 && b == 1) {
    fprintf(stderr, "Print and Benchmark not allowed at the same time");
    exit(0);
  }


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

	if(p){
		(void)printf("Processing FFT of Input:\n");print_cmplx_ar(in,10,1 , len);
	}
		//printf("----%d %d-----\n",sizeof(in),sizeof(in[0]) );

  struct timespec time;
  int tdmicros = 0;
	if(!b)(void)printf("fft starts: \n");
  for(int i=0; i<times; i++) {
    (void) clock_gettime(CLOCK_REALTIME, &time);
    tdmicros = ((int)time.tv_sec*1000000) + time.tv_nsec/1000;
    fft(in, out, len);
    (void) clock_gettime(CLOCK_REALTIME, &time);
    tdmicros = (((int)time.tv_sec*1000000) + time.tv_nsec/1000)-tdmicros;
    if(b)(void) printf("%d\n", tdmicros);
  }
  if(!b)(void)printf("fft done! Took %d microseconds\n", tdmicros);
	if(p){
		(void)printf("Result:\n");
	//print_cmplx_ar(out,10, 1,len);
    	print_comp(in, out,len);
	}


	free(in);free(out);
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


void loop_helper(double complex *in, double complex *out, int len, int k, int i)
{
  double complex omega = cexp(-2*k*PI*I/i);
  for(int j = 0; j < len/i; j++) {
      double complex twiddle = omega * out[j*i + k + i/2];
      out[j*i + k + i/2] = out[j*i + k] - twiddle;
      out[j*i + k] = out[j*i + k] + twiddle;
    }
}


void fft(double complex *in, double complex *out, int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/

    for(int i=0; i<len; i++) {
        out[reverse(i,lg(len))] = in[i];
    }

   for(int i = 2; i <= len; i *= 2)  {
     if(len/i <= 512) { //cutoff
       for(int k = 0; k < i/2; k++) {
         double complex omega = cexp(-2*k*PI*I/i);
         for(int j = 0; j < len/i; j++) {
             double complex twiddle = omega * out[j*i + k + i/2];
             out[j*i + k + i/2] = out[j*i + k] - twiddle;
             out[j*i + k] = out[j*i + k] + twiddle;
           }
       }
     }
     else {
        for(int k = 0; k < i/2; k++) {
          cilk_spawn loop_helper(in, out, len, k, i);
        }
        cilk_sync;
    }
  }

}
