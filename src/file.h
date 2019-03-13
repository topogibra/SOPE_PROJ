//
// Created by Gustavo Torres on 12-03-2019
//

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <time.h>
#include <sys/sysmacros.h>

int8_t hash_flags;
char * hash[3];


void info(char * file_path,char * hash[], int8_t hash_flags);




#endif
