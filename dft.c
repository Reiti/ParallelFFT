/**Basic DFT implementation
  *@author Michael Reitgruber
*/

#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include "prints.h"
#include "numgenparser.h"
#define PI 3.14159265358979323846


/*Calculates the DFT of the input vector using the naive O(N^2) algorithm*/
void dft(double complex *in, double complex *out, int len);

int main(int argc, char *argv[])
{
    char c;
	int p=0;
	while((c =getopt(argc, argv, "p"))!=-1){
		switch(c){
			case 'p':
				p=1;
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
    
   if(p){
		(void)printf("Processing FFT of Input:\n");print_cmplx_ar(in,10,1 , len);
	}
		//printf("----%d %d-----\n",sizeof(in),sizeof(in[0]) );
	(void)printf("dft starts: \n");
    dft(in, out, len);
	(void)printf("dft done! \n");
	
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

