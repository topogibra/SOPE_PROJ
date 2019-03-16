#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "dirtree.h"
#include "parser.h"

#define NO_FLAGS 3

void f(char* path) {
  printf("%s    PID:%d PARENT:%d\n", path, getpid(), getppid());
}

int main(int argc, char* argv[]) {
  // char *flagArguments[NO_FLAGS];
  // for(size_t i = 0; i < NO_FLAGS; i++)
  // {
  //     flagArguments[i] = "";
  // }

  // parseArguments(argc, argv, flagArguments, NO_FLAGS);
  // printf("%s\n",argv[1]);
  // printf("PARENT PID: %d\n", getpid());
  listdtree(argv[1], f);
  return 0;
}
