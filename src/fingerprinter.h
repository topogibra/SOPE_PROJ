//
// Created by Carlos Eduardo on 2019-03-11.
//

#ifndef FORENSIC_FINGERPRINTER_H
#define FORENSIC_FINGERPRINTER_H
#include <stdio.h>

enum HashArraySizes
{
    MD5SUM_SIZE = 32,
    SHA256SUM_SIZE = 64,
    SHA1SUM_SIZE = 40
};

int is_valid_hash_function(char* hash_command_name);
char* gen_checksum(char *file_name, char *hash_command_name);

#endif //FORENSIC_FINGERPRINTER_H
