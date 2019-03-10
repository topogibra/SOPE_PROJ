//
// Created by Carlos Eduardo on 2019-03-10.
//

#ifndef FORENSIC_PARSER_H
#define FORENSIC_PARSER_H
#include <stdint.h>


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

int8_t parseArguments(int argc, char *argv[], char *flagArguments[]);


#endif //FORENSIC_PARSER_H
