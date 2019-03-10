#include "parser.h"
#include <stdio.h>

#define NO_FLAGS 3

int main(int argc, char *argv[]) {
    char *flagArguments[NO_FLAGS];
    for(size_t i = 0; i < NO_FLAGS; i++)
    {
        flagArguments[i] = "";
    }
    
    parseArguments(argc, argv, flagArguments, NO_FLAGS);
    return 0;
}
