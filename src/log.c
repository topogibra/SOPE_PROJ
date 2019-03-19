#include <stdio.h>

FILE* open_file(const char *pathname, const char *mode) {
    FILE* file;
    if((file=fopen(pathname, mode)) == NULL) {
        perror("Failed to open file");
    }
    return file;
}
