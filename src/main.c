#include <limits.h>
#include <stdio.h>
#include "dirtree.h"
#include "parser.h"

#define NO_FLAGS 3

void f(char* path) {
  printf("%s\n", path);
}

int main(int argc, char* argv[]) {
  // char *flagArguments[NO_FLAGS];
  // for(size_t i = 0; i < NO_FLAGS; i++)
  // {
  //     flagArguments[i] = "";
  // }

  // parseArguments(argc, argv, flagArguments, NO_FLAGS);
  // printf("%s\n",argv[1]);
  listdtree(argv[1], f);
  return 0;
}
