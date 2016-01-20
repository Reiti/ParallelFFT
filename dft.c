/**Basic DFT implementation
  *@author Michael Reitgruber
*/

#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prints.h"
#include "numgenparser.h"
#include <time.h>
#define PI 3.14159265358979323846


/*Calculates the DFT of the input vector using the naive O(N^2) algorithm*/
void dft(double complex *in, double complex *out, int len);

int main(int argc, char *argv[])
{
    char c;
	int p=0;
  int b=0;
  int times = 1;
	while((c =getopt(argc, argv, "b:p"))!=-1){
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
    dft(in, out, len);
    (void) clock_gettime(CLOCK_REALTIME, &time);
    tdmicros = (((int)time.tv_sec*1000000) + time.tv_nsec/1000)-tdmicros;
    if(b)(void) printf("%d\n", tdmicros);
  }
  if(!b)(void)printf("dft done! Took %d microseconds\n", tdmicros);
	if(p){
		(void)printf("Result:\n");
	//print_cmplx_ar(out,10, 1,len);
    	print_comp(in, out,len);
	}


	free(in);free(out);

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
