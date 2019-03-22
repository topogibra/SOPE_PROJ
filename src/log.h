#ifndef LOG_H
#define LOG_H

#include <stdio.h>




int open_file(FILE** file, const char *pathname, const char *mode);
int close_file(FILE* file);
double get_time();
void log_activity(FILE* file, double initialTime, char* eventDescription);

#endif
