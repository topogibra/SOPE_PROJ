#ifndef LOG_H
#define LOG_H

#include <stdio.h>

int open_file(FILE** file, const char *pathname, const char *mode);
int close_file(FILE* file);
double get_time();
void log_activity(char* eventDescription);

void set_file(FILE* file);
FILE* get_file();

void set_initialTime(double time);
double get_initialTime();

#endif
