#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <sys/types.h>

int open_file(FILE** file, const char *pathname, const char *mode);
int close_file(FILE* file);
void log_activity(char* eventDescription);

void set_file(FILE* file);
FILE* get_file();

void set_initialTime();
double get_initialTime();
double difftimespecms(struct timespec ts1, struct timespec ts2);

#endif
