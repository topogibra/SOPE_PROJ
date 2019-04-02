#include <stdio.h>
#include <stdlib.h>
#include "analyzer.h"
#include "file.h"
#include "log.h"
#include "parser.h"
#define NO_FLAGS 3

void f(char* p) {
  printf("%s\n", p);
}

int main(int argc, char* argv[]) {
  FILE* file = NULL;
  open_file(&file, "logfile.txt", "a");

  printf("%d\n", getpid());

  double initialTimeStamp = get_time();
  set_file(file);
  set_initialTime(initialTimeStamp);

  log_activity("Started.");

  char* flagArguments[NO_FLAGS];
  for (size_t i = 0; i < NO_FLAGS; i++) {
    flagArguments[i] = "";
  }

  uint8_t flags = parseArguments(argc, argv, flagArguments, NO_FLAGS);

  uint8_t num_hash = 0;  // number of hashes passed as argument
  char* hash[3];         // array with the hashes strings

  if (flags & CLCHASH) {
    num_hash = getHashArguments(flagArguments[0], hash);
  }
  if (flags & SRECURS) {
    // setLDTflag(R_LIST);
    set_recur(true);
  }

  int output = STDOUT_FILENO;

  if (flags & SAVECSV) {
    if ((output = open(flagArguments[1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) ==
        -1) {
      perror("open");
      exit(1);
    }
    if (dup2(output, STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(1);
    }
  }

  setFlags(hash, num_hash);

  // info(flagArguments[NO_FLAGS-1]);

  // listdtree(flagArguments[NO_FLAGS - 1], info);
  analyzer(flagArguments[NO_FLAGS - 1], f);

  if (flags & SAVECSV) {
    close(output);
  }

  log_activity("Finished.");

  return 0;
}
