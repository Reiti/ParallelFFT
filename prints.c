#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "prints.h"
typedef char* str;
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
