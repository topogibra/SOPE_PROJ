#include "dirtree.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void listdtree(char* path, int indent) {
  DIR* dir;
  struct dirent* entry;
  chdir(path);
  if (!(dir = opendir("."))) {
    return;
  }

  while ((entry = readdir(dir)) !=
         NULL) {  // Cycles through the next entry in the dirtree
    struct stat ftype;
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }
    printf("%s/%s    %d", path, entry->d_name, getpid());
    stat(entry->d_name, &ftype);
    // printf("%d", S_ISDIR(ftype.st_mode));
    // if (S_ISDIR(ftype.st_mode)) {
    if (S_ISDIR(ftype.st_mode)) {
      printf(" <- dire\n");
      pid_t pid = fork();
      if (pid == 0) {    // child
        char tmp[1024];  // Path of the directory
        strcpy(tmp, ".");
        strcat(tmp, "/");
        strcat(tmp, entry->d_name);
        printf("tmp = %s    %d\n", tmp, getpid());
        chdir(tmp);
        listdtree(tmp, 1);  // Runs through the new path
        exit(0);
      } else {
        int waitst;
        wait(&waitst);
      }

    } else {
      printf("\n");
    }
  }

  closedir(dir);
}
