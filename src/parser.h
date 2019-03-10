//
// Created by Carlos Eduardo on 2019-03-10.
//

#ifndef FORENSIC_PARSER_H
#define FORENSIC_PARSER_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define NUMBER_HASH 3
#define MD5 1
#define SHA1 2
#define SHA256 4

/**
 * NOFLAGS = no flags used
 * SRECURS = searches recursively
 * CLCHASH = calculates a hash
 * GENLOGF = generates a log file
 */
enum options {
    NOFLAGS = 0, // 00000 == 0
    SRECURS = 1, // 00001 == 1
    CLCHASH = 2, // 00010 == 2
    SAVECSV = 4, // 00100 == 4
    GENLOGF = 8, // 01000 == 8
};

int8_t parseArguments(int argc, char *argv[], char *flagArguments[], size_t flags);

//return mask with flags
int8_t parseHashArguments(char * str);

//return integer with the number of flags and the second parameter has the string representing the hashes
int8_t getHashArguments(char * str,char * hash[]);


#endif //FORENSIC_PARSER_H
