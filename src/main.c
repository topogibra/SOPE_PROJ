#include "parser.h"
#include "dirtree.h"
#include <stdio.h>

#define NO_FLAGS 3

int main(int argc, char *argv[]) {
    // char *flagArguments[NO_FLAGS];
    // for(size_t i = 0; i < NO_FLAGS; i++)
    // {
    //     flagArguments[i] = "";
    // }
    
    // parseArguments(argc, argv, flagArguments, NO_FLAGS);
    // printf("%s\n",argv[1]); 
    listdtree(argv[1],1);
    return 0;
}
