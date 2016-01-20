/**Parallel recursive FFT implementation of the Cooley-Tukey algorithm using OpenMP
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include "prints.h"
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include "numgenparser.h"

#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);

double complex* rou;

int main(int argc, char *argv[])
{
	omp_set_num_threads(omp_get_max_threads());
	omp_set_nested(1);

	char c;
	int p=0;
	int b=0;
	int times=1;
	while((c =getopt(argc, argv, "pb:")) != -1){
		switch(c){
			case 'p':
				p=1;
				break;
			case 'b':
        b=1;
				times = atoi(optarg);
        break;
			default:
				break;
		}
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

	rou = (double complex*)malloc(len*sizeof(double complex));
	for(int i=0;i<len;i++)
		rou[i] = cexp(-2*PI*I*i/len);

	if(p){
		(void)printf("processing input of: \n");print_cmplx_ar(in, 10, 1, len);
	}

	double timeUsed = 0.0;
	int tdmicros=0;
  if(!b)(void)printf("fft starts: \n");
	#pragma omp parallel shared(timeUsed, in, out, len)
	{
		#pragma omp single
		{
			for(int i=0; i<times; i++) {
				timeUsed=omp_get_wtime();
				fft(in, out, len);
				timeUsed=omp_get_wtime() - timeUsed;
				tdmicros = (int)(timeUsed*1000000);
				if(b) printf("%d\n", tdmicros);
			}
		}
	}
  if(!b)(void)printf("fft done! Took %d microseconds\n", tdmicros);
	if(p){
		(void)printf("Result:\n");
		print_cmplx_ar(out,10, 1,len);
    	//print_comp(in, out,len);/*useless with this rec mehtod*/
	}

	free(in);free(out);
}



void fft_help(double complex *dc1, double complex *dc2, int len, int step)
{
	  if(step >= len) {
	    return;
	  }
		/*execute sequentially after a certain recursion depth
			cutoff point determined experimentially*/
		#pragma omp task if(step < len/8)
	  fft_help(dc2, dc1, len, step*2);
		#pragma omp task if(step < len/8)
	  fft_help(dc2+step, dc1+step, len, step*2);
		#pragma omp taskwait

	  for(int k=0; k<len/2; k+=step) {
	    double complex twiddle =rou[k]*dc2[2*k + step];
	    dc1[k] = dc2[2*k] + twiddle;
	    dc1[k + len/2] = dc2[2*k] - twiddle;
	  }
}
void fft(double complex *in, double complex *out, int len)
{
    for(int i=0; i<len; i++) {
      out[i] = in[i];
    }
	//out is right, but in wont be the same later
    fft_help(out, in, len, 1);
}
