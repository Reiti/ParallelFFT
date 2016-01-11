#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

static char * pgm_name = "<not set>";

static void usage(){
	(void)fprintf(stderr, "USAGE: %s: [-n <int32>] [-s <file>]\n", pgm_name);	
	exit(1);
}

static int parse_number(char* numstring) {
    char* endptr;
    int thenumber = strtol(numstring, &endptr, 10);
    if(*endptr != '\0' || thenumber < 0) {
        usage();
    }
    return thenumber;
}

int main(int argc, char  **argv){
	if(argc >=1){
		pgm_name = argv[0];
	}
	char c;
	int exp = 24;
	FILE * stream = stdout;
	while ((c = getopt (argc, argv, "n:s:")) != -1)
    {
        switch (c)
        {
        case 'n':
			exp = parse_number(optarg);
			break;
		case 's':
			if((stream = fopen(optarg, "w")) == NULL){
				(void)fprintf(stderr, "bad file\n");
				usage();
			}
			break;
        case '?':
            if (isprint (optopt))
                (void) fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                (void) fprintf (stderr, "Unknown option character `\\x%x'.\n",
                                optopt);
            usage ();
            break;
        default:
            assert (0);
        }
    }

	srand(time(NULL));

	for(int i =0;i< pow(2, exp);i++){
		(void)fprintf(stream, "%d\n",rand() % 1024);
	}
	return 0;
}
