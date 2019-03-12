#include "fingerprinter.h"
#include <stdio.h>

int main() {
    FILE* file;
    md5("../src/main.c", file);
    return 0;
}
