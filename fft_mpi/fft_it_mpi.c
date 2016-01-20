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
#include <time.h>
#include "prints.h"
#include "numgenparser.h"
#include <assert.h>
//#include "/opt/NECmpi/gcc/1.3.1/include/mpi.h"
#include <mpi/mpi.h>
#define PI 3.14159265358979323846

double complex* rou;
int rank, size;
void fft(int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);
void merge(int len);
int ispow2(int len);
	double complex * in;
	double complex * out;
int h=0;
int h2=0;

MPI_Comm mycomm= MPI_COMM_WORLD;

void frees(void){
	if(in !=NULL)
	free(in);
	if(out !=NULL)
	free(out);
	if(rou !=NULL)
	free(rou);
}
int main(int argc, char *argv[])
{
	
	atexit(frees);
    char c;
	int p=0;
	FILE* fin =NULL;
	int m=0;
	int b=0;
	
	MPI_Init(&argc, &argv);
	MPI_Group group;
	MPI_Comm_group(MPI_COMM_WORLD, &group);
	MPI_Comm_create(MPI_COMM_WORLD, group, &mycomm);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	assert(ispow2(size));
	while((c =getopt(argc, argv, "pf:mhab"))!=-1){
		switch(c){
			case 'p':
				p=1;
				break;
			case 'f':
				if((fin = fopen(optarg, "r+"))==NULL ){
					(void)fprintf(stderr,"openeing optarg failed\n");
					return -1;
				}
				break;
			case 'm':
				m=1;
				break;
			case 'h':
				h=1;
				break;
			case 'a':
				h2=1;
				break;
			case 'b':
				b=1;
				break;
			default:
				break;
		}
	}
	if(fin == NULL){
		(void)fprintf(stderr, "need file ... -f <inputfile>\n");
		return 1;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	
	int len = -1;
	if(p)
		(void)printf("Size = %d... I am Processor rank: %d\n", size, rank);
	

		len = getNumAmount(fin);

	MPI_Barrier(MPI_COMM_WORLD);
	if(len <=0){
		if(rank==0)
		(void)fprintf(stderr,"something went wrong!...len: %d - Rank: %d\n", len, rank);
		
	return 1;
	}

	in = (double complex*)malloc(len * sizeof(double complex));
	out = (double complex*)malloc(len * sizeof(double complex));
	

	if(in ==NULL || out ==NULL){
		fprintf(stderr, "ERROR MALLOCING!!rank = %d\n", rank);
		return 1;
	}
	
	int counter=-2;
		counter = getNumbers(in, fin);

	MPI_Barrier(MPI_COMM_WORLD);

	if(counter != len){
		if(rank==0)
		(void)fprintf(stderr, "wrong number amount in stream\n");
		return 1;
	}
	assert(len >= size);
	rou = (double complex*)malloc(len*sizeof(double complex));
	for(int i=0;i<len;i++)
		rou[i] = cexp(-2*PI*I*i/len);

	if(p && rank ==0){
		(void)printf("Processing FFT of Input:\n");print_cmplx_ar(in,10,1 , len);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double tpast= MPI_Wtime();
	  fft(len);

	double tmeasure = 0.0;
	tpast = MPI_Wtime()-tpast;
	MPI_Reduce(&tpast, &tmeasure, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	int msecs = (int)(tmeasure * 1000000);

	if(rank == 0 && !b){
		(void)printf("According to MPI time fft took %d microseconds\n", msecs);
	}
	if(rank ==0 &&b)
		(void)printf("%d", msecs);


	if(m)
		merge(len);	

	if(m  && rank ==0){
		(void)printf("Result:\n");

    	print_comp(in, out,len);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	fclose(fin);
	MPI_Comm_free(&mycomm);		
	MPI_Finalize();
	return 0;
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
int ispow2(int x){
	while(x %2 ==0 && x >1)
		x/=2;
	return x==1;	
}

void merge(int len){
	MPI_Status status;
	if(size <2) return;	
	if(rank ==0){
		for(int r =1;r< size;r++){
			double complex *rec = \
						(double complex*)malloc((len) * sizeof(double complex));
			MPI_Recv(rec, len, MPI_DOUBLE_COMPLEX,
						 r, 0, MPI_COMM_WORLD, &status);
			for(int rn =r;rn < len;rn+=size){
				for(int i=rn;i<len ;i+=size){
					out[i]=rec[i];
				}
			}
			free(rec);
		}
	}else{
		MPI_Send(out, len, MPI_DOUBLE_COMPLEX,
					0, 0,MPI_COMM_WORLD);
	}
}

void help(int len, int i){
	MPI_Status status;
	if(size > i/2 && len/i >= size){
		int k = rank % (i/2);
		int tbd=(len/i)/(size/(i/2));
		int t = rank / (i/2);

		if(rank < i/2){
			for(int r=rank+i/2;r < size;r+=i/2)
				MPI_Send(out, len, MPI_DOUBLE_COMPLEX,
						r, 0,MPI_COMM_WORLD);
		}else{
			MPI_Recv(out, len, MPI_DOUBLE_COMPLEX,
			 			k, 0, MPI_COMM_WORLD, &status);
		}
		
		double complex omega = cexp(-2*k*PI*I/i);
		for(int j = t*tbd;j < (t+1) * tbd; j++){
			double complex twiddle = omega * out[j*i + k + i/2];
            out[j*i + k + i/2] = out[j*i + k] - twiddle;
            out[j*i + k] = out[j*i + k] + twiddle;
		}	
		if(rank < i/2){
			for(int r=rank+i/2;r < size;r+=i/2){
				MPI_Recv(out+i*(r/(i/2))*tbd+k, len-i*(r/(i/2))*tbd-k, MPI_DOUBLE_COMPLEX,
			 			r, rank, MPI_COMM_WORLD, &status);
			}
		}else{
			MPI_Send(out+i*t*tbd+k, len-i*t*tbd-k, MPI_DOUBLE_COMPLEX,
					k, k,MPI_COMM_WORLD);
		}
	}else{
		for(int k =rank; k < i/2; k+=size) {
			double complex omega = cexp(-2*k*PI*I/i);
			for(int j = 0;j < len/i; j++){
				double complex twiddle = omega * out[j*i + k + i/2];
            	out[j*i + k + i/2] = out[j*i + k] - twiddle;
    	        out[j*i + k] = out[j*i + k] + twiddle;
			}
		}
	}
}

void help2(int len, int i){
	if(size > i/2 && len/i >= size){
		int k = rank % (i/2);

		MPI_Comm_split(mycomm, k, rank, &mycomm);
		int newrank;
		int newsize;
		MPI_Comm_rank(mycomm, &newrank);	
		MPI_Comm_size(mycomm, &newsize);

		double complex * tmp = \
			(double complex*)malloc(len/newsize* sizeof(double complex));

		MPI_Scatter(out+newrank*len/newsize, len/newsize, MPI_DOUBLE_COMPLEX,
					tmp, len/newsize, MPI_DOUBLE_COMPLEX,  0, mycomm );
		int tbd=(len/i)/newsize;
		double complex omega = cexp(-2*k*PI*I/i);
		for(int j = 0;j < tbd; j++){
			double complex twiddle = omega * tmp[j*i + k + i/2];
            tmp[j*i + k + i/2] = tmp[j*i + k] - twiddle;
            tmp[j*i + k] = tmp[j*i + k] + twiddle;
		}
		MPI_Gather(tmp, len/newsize, MPI_DOUBLE_COMPLEX,
					out, len/newsize, MPI_DOUBLE_COMPLEX,0,  mycomm);	
		free(tmp);

	}else{
		for(int k =rank; k < i/2; k+=size) {
			double complex omega = cexp(-2*k*PI*I/i);
			for(int j = 0;j < len/i; j++){
				double complex twiddle = omega * out[j*i + k + i/2];
            	out[j*i + k + i/2] = out[j*i + k] - twiddle;
    	        out[j*i + k] = out[j*i + k] + twiddle;
			}
		}
	}

}

void sprayData(int i, int len){
	MPI_Status status;
	if(rank +i/2 < i){
		MPI_Send(out, len, MPI_DOUBLE_COMPLEX,
				rank + i/2, rank,MPI_COMM_WORLD);
	}else{
		MPI_Recv(out, len, MPI_DOUBLE_COMPLEX,
			 	rank - i/2, rank-i/2, MPI_COMM_WORLD, &status);
	}	
}

void fft(int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/
    for(int i=0; i<len; i++) {
        out[reverse(i,lg(len))] = in[i];
    }
	
  	for(int i = 2; i <= len; i *= 2)  {
		//(void)printf("Ich %d berechne index: ",rank);
		for(int k=rank;k < i/2;k+=size){
	       	double complex omega = cexp(-2*k*PI*I/i);
            for(int j = 0;j < len/i ;j++) {
                double complex twiddle = omega * out[j*i + k + i/2];
                out[j*i + k + i/2] = out[j*i + k] - twiddle;
                out[j*i + k] = out[j*i + k] + twiddle;
            }
		}
		if(i <= size && rank < i){
			sprayData(i, len);
		}
    }	
}

