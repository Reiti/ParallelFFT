/**Non-Parallel recursive FFT implementation using the Cooley-Tukey algorithm
  *@author Michael Reitgruber
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include "prints.h"
#include <stdlib.h>
#include <time.h>
#include "numgenparser.h"
#include <mpi/mpi.h>

#define PI 3.14159265358979323846

void fft(double complex *in, double complex *out, int len);

double complex* rou;

int main(int argc, char *argv[])
{
	char c;
	int p=0;
	while((c =getopt(argc, argv, "p")) != -1){
		switch(c){
			case 'p':
				p=1;
				break;
			default:
				break;
		}
	}


	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int len = -1;

	if(rank == 0){
		len = getNumAmount();
		for(int i=1;i<size;i++)
			MPI_Send(&len, 1, MPI_INT,i, 1,MPI_COMM_WORLD);
	}else{
		MPI_Status status;
		MPI_Recv(&len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(len ==-1){
		(void)fprintf(stderr,"something went wrong!... Rank: %d\n", rank);
		return 1;
	}



	//double complex in[len];
    //double complex out[len];

	double complex * in = (double complex*)malloc(len * sizeof(double complex));
	double complex * out = (double complex*)malloc(len * sizeof(double complex));
	int counter=-2;
	if(rank == 0){
		counter = getNumbers(in);
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
		(void)fprintf(stderr, "wrong number amount in stream... %d %d\n", counter,len);
		return 1;
	}

	rou = (double complex*)malloc(len*sizeof(double complex));
	for(int i=0;i<len;i++)
		rou[i] = cexp(-2*PI*I*i/len);

	if(p){
		(void)printf("processing input of: \n");print_cmplx_ar(in, 10, 1, len);
	}

	struct timespec time;
	unsigned long tdnano;
	time_t tdsec;
  (void)printf("fft starts: \n");
	(void) clock_gettime(CLOCK_REALTIME, &time);
  tdnano = time.tv_nsec;
  tdsec = time.tv_sec;
  fft(in, out, len);
	(void) clock_gettime(CLOCK_REALTIME, &time);
  tdnano = time.tv_nsec - tdnano;
  tdsec = time.tv_sec - tdsec;
  if(tdsec == 0)
	  (void)printf("fft done! Took %ld nanoseconds\n", tdnano);
  else
    (void)printf("fft done! Took %d seconds\n", (int)tdsec);
	if(p){
		(void)printf("Result:\n");
		print_cmplx_ar(out,10, 1,len);
    	//print_comp(in, out,len);/*useless with this rec mehtod*/
	}

	free(in);free(out);
	MPI_Finalize();
}



void fft_help(double complex *dc1, double complex *dc2, int len, int step)
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
    fft_help(dc2, dc1, len, step*2);
    fft_help(dc2+step, dc1+step, len, step*2);

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
