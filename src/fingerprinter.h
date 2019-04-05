//
// Created by Carlos Eduardo on 2019-03-11.
//

#ifndef FORENSIC_FINGERPRINTER_H
#define FORENSIC_FINGERPRINTER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum HashArraySizes {
  MD5SUM_SIZE = 32,
  SHA256SUM_SIZE = 64,
  SHA1SUM_SIZE = 41
};

int is_valid_hash_function(char* hash_command_name);
void gen_checksum(char* file_name, char* hash_command_name, char* checksum);

#endif  // FORENSIC_FINGERPRINTER_H
