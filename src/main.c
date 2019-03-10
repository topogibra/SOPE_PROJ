#include "parser.h"
#include <stdio.h>

#define NO_FLAGS 3

int main(int argc, char *argv[]) {
    char *flagArguments[NO_FLAGS];
    for(size_t i = 0; i < NO_FLAGS; i++)
    {
        flagArguments[i] = "";
    }

    uint8_t flags = parseArguments(argc, argv, flagArguments, NO_FLAGS);
    uint8_t hash_flags = 0;
    if(flags & CLCHASH){
      //escolher uma das duas
      /*
      hash_flags = parseHashArguments(flagArguments[0]);
      printf("%d\n",hash_flags );
      */
      char * hash[3];
      hash_flags = getHashArguments(flagArguments[0],hash);
      for(int i = 0; i<hash_flags;i++){
        printf("%s\n",hash[i]);
      }


    }

    return 0;
}
