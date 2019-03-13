#include "fingerprinter.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* checksum = gen_checksum("../src/main.c", "sha256sum");
    printf("%s\n", checksum);
    free(checksum);
#define NO_FLAGS 3

int main(int argc, char *argv[]) {
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
      /*printf("Hashes : \n");
      for(int i = 0; i<hash_flags;i++){
        printf("%s\n",hash[i]);
      }*/
    }
    return 0;
}