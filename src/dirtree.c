#include "dirtree.h"
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
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
  bool child = false;
  chdir(path);

  if ((dir = opendir(".")) == NULL) {
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }
    if (entry->d_type == DT_DIR) {
      char tmp[PATH_MAX];
      getcwd(tmp, PATH_MAX);
      strcat(tmp, "/");
      strcat(tmp, entry->d_name);
      pid_t pid = fork();
      if (pid == 0) {
        child = true;
        // f(tmp);
        chdir(tmp);
        closedir(dir);
        dir = opendir(".");
        // printf("%d\n", entry->d_type == DT_DIR);
      } else {
        wait(NULL);
      }
    } else {
      char tmp[PATH_MAX];
      getcwd(tmp, PATH_MAX);
      strcat(tmp, "/");
      strcat(tmp, entry->d_name);
      f(tmp);
    }
  }
  if (child) {
    exit(0);
  }
  while(wait(NULL) > 0){}
  closedir(dir);
}
