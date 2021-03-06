/**Parallel recursive FFT implementation of the Cooley-Tukey algorithm using Cilk
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include "prints.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "numgenparser.h"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>


#define PI 3.14159265358979323846

double complex* rou;

void fft(double complex *in, double complex *out, int len);


int main(int argc, char *argv[])
{
//	__cilkrts_set_param("nworkers", "24");


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
				times=atoi(optarg);
        break;
			default:
				break;
		}
	}

	if(!b)printf("Total number of workers: %d\n",__cilkrts_get_nworkers());

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

	rou = (double complex*)malloc(len*sizeof(double complex));
	for(int i=0;i<len;i++)
		rou[i] = cexp(-2*PI*I*i/len);




	if(p){
		(void)printf("processing input of: \n");print_cmplx_ar(in, 10, 1, len);
	}

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
		print_cmplx_ar(out,10, 1,len);
    	//print_comp(in, out,len);/*useless with this rec mehtod*/
	}

	free(in);free(out);free(rou);

}

/*roots of unity*/
double complex *rou;

void sequential(double complex *dc1, double complex *dc2, int len, int step)
{
    if(step >= len) {
      return;
    }
	/*basicly this calcs FFT for the odd and even part and stores that in one array,
	recursive calls later use these previous calculations, to calculate further...
	that's why the two arrays get swapped
	it's easily to demonstrate if you draw youself the tree of recursive calls. Each node
	with two children gets their needed FFT information, calculated by the children,
	stored in dc2. Each node self stores the calculated information in dc1, which
	is dc2 in all parents and the 'out' array in the original call*/
  sequential(dc2, dc1, len, step*2);
  sequential(dc2+step, dc1+step, len, step*2);


    for(int k=0; k<len/2; k+=step) {
      double complex twiddle = rou[k]*dc2[2*k + step];

      dc1[k] = dc2[2*k] + twiddle;
      dc1[k + len/2] = dc2[2*k] - twiddle;
    }
}

void fft_help(double complex *dc1, double complex *dc2, int len, int step)
{
	if((len/2)/step < 64) { //cutoff
		sequential(dc1, dc2, len, step);
		return;
	}
    if(step >= len) {
      return;
    }
	/*basicly this calcs FFT for the odd and even part and stores that in one array,
	recursive calls later use these previous calculations, to calculate further...
	that's why the two arrays get swapped
	it's easily to demonstrate if you draw youself the tree of recursive calls. Each node
	with two children gets their needed FFT information, calculated by the children,
	stored in dc2. Each node self stores the calculated information in dc1, which
	is dc2 in all parents and the 'out' array in the original call*/
  cilk_spawn fft_help(dc2, dc1, len, step*2);
  cilk_spawn fft_help(dc2+step, dc1+step, len, step*2);
	cilk_sync;


    for(int k=0; k<len/2; k+=step) {
      double complex twiddle = rou[k]*dc2[2*k + step];

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
