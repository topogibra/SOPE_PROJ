//
// Created by Carlos Eduardo on 2019-03-10.
//

#include "parser.h"
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Gets the flags and respective arguments from argv[]
 * @param argc The size of argv[]
 * @param argv The arguments passed to the program
 * @param flagArguments The arguments that correspond to each flag, with the
 * following position relation: 0 = 'h'; 1 = 'o'; 2 = 'v';
 * @returnd1
 */
int8_t parseArguments(int argc,
                      char* argv[],
                      char* flagArguments[],
                      size_t flags) {
  int opt;
  char* cvalue = NULL;

  if (argc == 1) {
    printf(
        "Usage: %s [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] "
        "<file|dir>\n",
        argv[0]);
    exit(EXIT_FAILURE);
  }

  while ((opt = getopt(argc, argv, "rh:o:v:")) != -1) {
    switch (opt) {
      case 'r':
        flags |= SRECURS;
        break;
      case 'h':
        flags |= CLCHASH;
        cvalue = optarg;
        flagArguments[0] = cvalue;
        break;
      case 'o':
        flags |= SAVECSV;
        cvalue = optarg;
        flagArguments[1] = cvalue;
        break;
      case 'v':
        flags |= GENLOGF;
        cvalue = optarg;
        // flagArguments[2] = cvalue;
        break;
      case '?':
        if (optopt == 'h' || optopt == 'o' || optopt == 'v') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        } else {
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        }
        __attribute__ ((fallthrough));
      default:
        printf(
            "Usage: %s [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] "
            "<file|dir>\n",
            argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  //   flagArguments[2] = argv[argc - 1];
  for (size_t i = 0; i < flags; i++) {
      if (strcmp (flagArguments[i], ""))
        printf("%s\n", flagArguments[i]);
  }

  return flags;
}

int8_t parseHashArguments(char * str){
  uint8_t flags = 0;
  char *token = strtok(str, ",");
  while(token) {
      if(!strcmp(token,"md5"))
        flags |= MD5;
      else if(!strcmp(token,"sha1"))
        flags |= SHA1;
      else if(!strcmp(token,"sha256"))
        flags |= SHA256;
      token = strtok(NULL, ",");
  }
  return flags;
}
int8_t getHashArguments(char * str,char *hash[]){
  uint8_t flags = 0;
  char *token = strtok(str, ",");
  uint8_t i = 0;
  while(token) {
      if(!strcmp(token,"md5")){
        flags |= MD5;
        hash[i++] = token;
      }
      else if(!strcmp(token,"sha1")){
        flags |= SHA1;
        hash[i++] = token;
      }
      else if(!strcmp(token,"sha256")){
        flags |= SHA256;
        hash[i++] = token;
      }
      token = strtok(NULL, ",");

  }
  return --i;
}
