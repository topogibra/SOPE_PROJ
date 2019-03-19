#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int open_file(FILE** file, const char *pathname, const char *mode);
int close_file(FILE* file);
void log_activity(FILE* file, char* event_description);

#endif
