#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <getopt.h>
static char * pgm_name = "<not set>";

static void usage() {
    (void)fprintf(stderr, "USAGE: %s [-e|--exp <int32>]\n\
		 [-f|--file <file>]\n\
		 [-r|--randmax <randmax>]\n\
		 [--rectangle]\n\
		 [--triangle]]\n", pgm_name);
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

int main(int argc, char  **argv) {
    if(argc >=1) {
        pgm_name = argv[0];
    }
    int exp = 24;
    int randmax = 10;
    int rectangle = 0;
    int triangle = 0;
    FILE *stream = stdout;
    while(1) {
        static struct option l_opts[] = {
            {"exp", required_argument, 0, 'e'},
            {"file", required_argument,0, 'f'},
            {"randmax", required_argument, 0, 'r'},
            {"rectangle", no_argument, 0 ,0},
            {"triangle", no_argument, 0, 1}
        };
        int opt_index =0;
        char c = getopt_long(argc, argv, "e:f:r:",  l_opts, &opt_index);
        if(c==-1) {
            break;
        }
        switch(c) {
        case 0:
            rectangle =1;
            break;
        case 1:
            triangle =1;
            break;
        case 'e':
            if(optarg ==NULL) {
                usage();
            }
            exp = parse_number(optarg);
            break;
        case 'f':
            if(optarg ==NULL) {
                usage();
            }
            if((stream = fopen(optarg, "w")) == NULL) {
                (void)fprintf(stderr, "bad file\n");
                usage();
            }
            break;
        case 'r':
            if(optarg ==NULL) {
                usage();
            }
            randmax = parse_number(optarg);
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

    if(optind != argc) {
        usage();
    }

    (void)fprintf(stream, "%d\n", (int)pow(2,exp));

    if(triangle) {
        int print =0;
        for(int i =0; i< pow(2, exp); i++) {
            (void)fprintf(stream, "%d\n",print);
            print = (print == 0 ? 1 : (print == 1? 2 :0));
        }
    } else if(rectangle) {
        for(int i =0; i< pow(2, exp);) {
            for(int j=0; j<2; j++,i++)
                (void)fprintf(stream, "%d\n",1);
            for(int j=0; j<2; j++,i++)
                (void)fprintf(stream, "%d\n",-1);
        }
    } else {
        srand(time(NULL));
        for(int i =0; i< pow(2, exp); i++) {
            (void)fprintf(stream, "%d\n",rand() % randmax);
        }
    }
    return 0;
}
