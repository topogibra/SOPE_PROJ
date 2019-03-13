#include "dirtree.h"
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void listdtree(char* path, void (*f)(char*)) {
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
    // printf("%s/%s    %d\n", path, entry->d_name, getpid());
    stat(entry->d_name, &ftype);
    if (S_ISDIR(ftype.st_mode)) {
      pid_t pid = fork();
      if (pid == 0) {        // child
        char tmp[PATH_MAX];  // Path of the directory
        strcpy(tmp, ".");
        strcat(tmp, "/");
        strcat(tmp, entry->d_name);
        // printf("tmp = %s    %d\n", tmp, getpid());
        chdir(tmp);
        listdtree(tmp, f);  // Runs through the new path
        exit(0);
      } else {
        int waitst;

        wait(&waitst);
      }

    } else {
      char entry_path[PATH_MAX];
      getcwd(entry_path,PATH_MAX);
      strcat(entry_path, "/");
      strcat(entry_path, entry->d_name);
      f(entry_path);
    }
  }

  closedir(dir);
}
