
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#include "file.h"


#define NO_FLAGS 3



int main(int argc, char *argv[]) {
    /**
    char* checksum = gen_checksum("../src/main.c", "sha256sum");
    printf("%s\n", checksum);
    free(checksum);
     */


    char *flagArguments[NO_FLAGS];
    for(size_t i = 0; i < NO_FLAGS; i++)
    {
        flagArguments[i] = "";
    }

    uint8_t flags = parseArguments(argc, argv, flagArguments, NO_FLAGS);

    uint8_t hash_flags = 0;
    char * hash[3];

    if(flags & CLCHASH){
      hash_flags = getHashArguments(flagArguments[0],hash);
    }

    int output = STDOUT_FILENO;

    if(flags & SAVECSV){
      if((output = open(flagArguments[1], O_WRONLY | O_CREAT | O_TRUNC , 0777 ))==-1){
        perror("open");
        exit(1);
      }
      if(dup2(output,STDOUT_FILENO)==-1){
        perror("dup2");
        exit(1);
      }
    }
    setFlags(hash,hash_flags);
    info(flagArguments[NO_FLAGS-1]);

    if(flags & SAVECSV){
      close(output);
    }

    return 0;
}
