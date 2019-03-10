//
// Created by Carlos Eduardo on 2019-03-10.
//

#include "parser.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Gets the flags and respective arguments from argv[]
 * @param argc The size of argv[]
 * @param argv The arguments passed to the program
 * @param flagArguments The arguments that correspond to each flag, with the following position relation:
 *                      0 = 'h';
 *                      1 = 'o';
 *                      2 = 'v';
 * @return
 */
int8_t parseArguments(int argc, char *argv[], char *flagArguments[])
{
    int opt;
    int flags = NOFLAGS;
    char *cvalue = NULL;

    if(argc == 1){
      printf("Usage: %s [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "rh:o:v:")) != -1) {
        switch (opt) {
            case 'r':
                flags |= SRECURS;
                break;
            case 'h':
                flags |= CLCHASH;
                cvalue = optarg;
                flagArguments[0] = cvalue;
                //printf("%s\n", flagArguments[0] );
                break;
            case 'o':
                flags |= SAVECSV;
                cvalue = optarg;
                flagArguments[1] = cvalue;
                break;
            case 'v':
                flags |= GENLOGF;
                cvalue = optarg;
                //flagArguments[2] = cvalue;
                break;
            case '?':
                if (optopt == 'h' || optopt == 'o' || optopt == 'v')
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else
                {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
            default:
                printf("Usage: %s [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    flagArguments[2] = argv[argc-1];
    printf("%s\n", flagArguments[0] );
    printf("%s\n", flagArguments[1] );
    printf("%s\n", flagArguments[2] );
    return flags;
}
