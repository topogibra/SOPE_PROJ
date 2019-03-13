#include "fingerprinter.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* checksum = gen_checksum("../src/main.c", "md5sum");
    printf("%s\n", checksum);
    free(checksum);
    return 0;
}