#include "dirtree.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void listdtree(char* path, int indent) {
  DIR* dir;
  struct dirent* entry;
  int a = indent;
  struct stat *ftype;

  if (!(dir = opendir(path))) {
    return;
  }

  while ((entry = readdir(dir)) != NULL) {

    printf("%s\n", entry->d_name);
    stat(entry->d_name,ftype);
    if (S_ISDIR(ftype->st_mode)){

    }
  }
}
