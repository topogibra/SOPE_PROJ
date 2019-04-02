#include "analyzer.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

bool recurs = false;
char currentdir[PATH_MAX];
bool all_father = true;

void set_recur(bool b) {
  recurs = b;
}

void printcwd() {
  char tmp[PATH_MAX];
  getcwd(tmp, PATH_MAX);
  printf("CWD -> %s\n", tmp);
}

void analyzer(char* path, void (*f)(char*)) {
  int num_childs = 0;
  DIR* dir;
  struct dirent* entry;

  if (chdir(path) != 0) {
    exit(4);
  }

  if ((dir = opendir(".")) == NULL) {
    exit(3);
  }

  while ((entry = readdir(dir)) != NULL) {
    char entrypath[PATH_MAX];
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }
    sprintf(entrypath, "%s%s", (all_father) ? "" : currentdir, entry->d_name);
    if (entry->d_type == DT_DIR) {
      if (recurs) {
        pid_t pid = fork();
        if (pid == 0) {
          all_father = false;
          sprintf(currentdir, "%s%s", (all_father) ? "" : currentdir,
                  strcat(entry->d_name, "/"));
          analyzer(entry->d_name, f);
          exit(0);
        } else {
          num_childs++;
        }
      }
    }
    if (entry->d_type == DT_REG) {
      if (!all_father) {
        printf("%s", currentdir);
      }
      f(entry->d_name);
    }
  }

  for (; num_childs != 0; num_childs--) {
    wait(NULL);
  }

  closedir(dir);
}
