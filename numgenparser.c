#include "numgenparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
int getNumAmount(void){
	char line[100];
	(void)fgets(line, 100, stdin);
	char* endptr;
	line[strlen(line)-1] = '\0';
	int len  = strtol(line, &endptr, 10);
	//(void)printf("%s : %d\n", line, len);
	if(*endptr != '\0' || len < 0) {
   	 	(void)fprintf(stderr, "error while reading numberamount\n");
		return -1;
	}
	return len;
}

static int parse_number(char* numstring) {
    char* endptr;
    int thenumber = strtol(numstring, &endptr, 10);
    if(*endptr != '\0' || thenumber < 0) {
        (void)fprintf(stderr, "error while reading number\n");
		exit(1); 
    }
    return thenumber;
}

int getNumbers(complex double *in){
	int counter=0;
	char line[100];
	while(fgets(line, 6, stdin)){
		line[strlen(line)-1] = '\0';
		in[counter] = parse_number(line);
		counter++;
	}	
	return counter;
}
