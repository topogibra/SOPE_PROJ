#ifndef ANALYZER_H
#define ANALYZER_H
#include <stdbool.h>

void set_recur(bool b);
void analyzer(char* path, void (*f)(char*));

#endif
