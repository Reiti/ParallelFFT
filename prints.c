#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "prints.h"
typedef char* str;
void print_comp(double complex* cpar1, double complex* cpar2, int len){
	for(int i = 0; i<len; i++) {
		print_cmpl_cell(cpar1, 10, i);	
		(void)printf("- ");
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
		str vz;
		if(b <0){
			vz = "-";
			b*=-1;
		}else{
			vz = "+";
		} 
		if(ceilf(b)==b){
			 (void)sprintf(bs, "%s%*ii] ",vz ,fmtl, (int)b);
		}else{
			 (void)sprintf(bs, "%s%*fi] ",vz ,fmtl,  b);
		}
		str res = (str)malloc(50);
		(void)strcpy(res,as);
		(void)strcat(res, bs);
		(void)printf("%s",res);
		(void)free(res);
		(void)free(as);(void)free(bs);
}
void print_cmplx_ar(double complex* cpar, int fmtl, int horz, int len){
	for(int i = 0; i<len; i++) {
		print_cmpl_cell(cpar, fmtl,i);
		if(horz){
			(void)printf("\n");
		}
    }
	if(!horz)
		(void)printf("\n");
}
