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

//#include "/opt/NECmpi/gcc/1.3.1/include/mpi.h"
#include <mpi/mpi.h>
#define PI 3.14159265358979323846

double complex* rou;
int rank, size;
void fft(int len);
uint32_t reverse(uint32_t index, int size);
int lg(int num);

double complex * in;
	double complex * out;

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
	FILE* fin =stdin;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	while(rank ==0 && (c =getopt(argc, argv, "pf:"))!=-1){
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
			default:
				break;
		}
	}


	MPI_Barrier(MPI_COMM_WORLD);
	
	
	int len = -1;
	printf("Size = %d... I am Processor rank: %d\n", size, rank);
	if(rank == 0){
		len = getNumAmount(fin);
		for(int i=1;i<size;i++)	
			MPI_Send(&len, 1, MPI_INT,i, 1,MPI_COMM_WORLD);
	}else{
		MPI_Status status;
		MPI_Recv(&len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(len <=0){
		(void)fprintf(stderr,"something went wrong!...len: %d - Rank: %d\n", len, rank);
		return 1;
	}

    

	//double complex in[len];
    //double complex out[len];

	in = (double complex*)malloc(len * sizeof(double complex));
	out = (double complex*)malloc(len * sizeof(double complex));
	int counter=-2;
	if(rank == 0){
		counter = getNumbers(in, fin);
		
	}else{
		
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(rank ==0){
		for(int i=1;i<size;i++){	
			MPI_Send(&counter, 1, MPI_INT,i, 1,MPI_COMM_WORLD);
			MPI_Send(in, len, MPI_DOUBLE_COMPLEX,i, 1,MPI_COMM_WORLD);
		}
	}else{
		MPI_Status status;
		MPI_Recv(&counter, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(in, len, MPI_DOUBLE_COMPLEX, 0, 1, MPI_COMM_WORLD, &status);	
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(counter != len){
		(void)fprintf(stderr, "wrong number amount in stream\n");
		return 1;
	}

	rou = (double complex*)malloc(len*sizeof(double complex));
	for(int i=0;i<len;i++)
		rou[i] = cexp(-2*PI*I*i/len);

	if(p && rank ==0){
		(void)printf("Processing FFT of Input:\n");print_cmplx_ar(in,10,1 , len);
	}
		//printf("----%d %d-----\n",sizeof(in),sizeof(in[0]) );
MPI_Barrier(MPI_COMM_WORLD);

  struct timespec time;
  unsigned long tdnano;
  time_t tdsec;
	if(rank ==0)
		(void)printf("fft starts: \n");
  (void) clock_gettime(CLOCK_REALTIME, &time);
  tdnano = time.tv_nsec;
  tdsec = time.tv_sec;
  fft(lg(len));
  (void) clock_gettime(CLOCK_REALTIME, &time);
  tdnano = time.tv_nsec - tdnano;
  tdsec = time.tv_sec - tdsec;
if(rank ==0){
  if(tdsec == 0)
	  (void)printf("fft done! Took %ld nanoseconds\n", tdnano);
  else
    (void)printf("fft done! Took %d seconds\n", (int)tdsec);
}
	if(p && rank ==0){
		(void)printf("Result:\n");
	//print_cmplx_ar(out,10, 1,len);
    	print_comp(in, out,len);
	}
MPI_Barrier(MPI_COMM_WORLD);
//	free(in);free(out);free(rou);
	fclose(fin);	
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
int ispow2(int len){
	while(len !=0){
		if(len %2 ==1 && len /2 != 0)
			return 0;
		len>>=1;
	}
	return 1;
}

void synch(int i, int tbd, int length){
	MPI_Status status;
	if(rank ==0){
		for(int r=1;r<size;r++){
			if(r >=i/2)break;
			MPI_Recv(out+r*tbd, length-rank*tbd,														 MPI_DOUBLE_COMPLEX, r, 1, MPI_COMM_WORLD,
					 &status);
		}						
	}else{
		if(rank < i/2)
			MPI_Send(out+rank*tbd, length-rank*tbd,
 					MPI_DOUBLE_COMPLEX, 0, 1,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		for(int r=1;r<size;r++)
				MPI_Send(out, length, MPI_DOUBLE_COMPLEX,
					r, 1,MPI_COMM_WORLD);
	}else{
		MPI_Recv(out, length, MPI_DOUBLE_COMPLEX,
					 0, 1, MPI_COMM_WORLD, &status);
	}
}

void fft(int len)
{
    /*Fill the output array in bit reversed order, rest of fft can be done inplace*/
	int length = pow(2,len);
    for(int i=0; i<length; i++) {
        out[reverse(i,len)] = in[i];
    }
	
   for(int i = 2; i < length; i *= 2)  {
		MPI_Barrier(MPI_COMM_WORLD);
		int tbd = i/2/size;
		if(!tbd){
			if(rank >= i/2) continue;
			tbd=1;
		}
        for(int k =rank * tbd; k < (rank+1)*tbd; k++) {
            double complex omega = rou[k];
            for(int j = 0;j<length/i ;j++) {
                double complex twiddle = omega * out[j*i + k + i/2];
                out[j*i + k + i/2] = out[j*i + k] - twiddle;
                out[j*i + k] = out[j*i + k] + twiddle;
            }
		}
		MPI_Barrier(MPI_COMM_WORLD);
		synch(i, tbd, length);
		MPI_Barrier(MPI_COMM_WORLD);	
    }
}
