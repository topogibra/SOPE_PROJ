#include "parser.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    char *flagArguments[3];
    parseArguments(argc, argv, flagArguments);
    return 0;
}
