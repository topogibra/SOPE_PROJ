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

u_int8_t flag = 0x0;

int listdtree(char* path, void (*f)(char*)) {
  // Safeguard to maintain the current working directory
  char current_dir[PATH_MAX];
  DIR* dir;
  struct dirent* entry;
  bool is_child = false;

  getcwd(current_dir, PATH_MAX);

  chdir(path);

  if ((dir = opendir(".")) == NULL) {
    return 139;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }
    if (entry->d_type == DT_DIR) {
      char newpath[PATH_MAX];
      getcwd(newpath, PATH_MAX);
      strcat(newpath, "/");
      strcat(newpath, entry->d_name);
      if (flag & R_LIST) {
        pid_t pid = fork();
        if (pid == 0) {
          is_child = true;
          if (flag & A_DIR) {
            f(newpath);
          }
          chdir(newpath);
          closedir(dir);
          dir = opendir(".");
        }
      }
    } else {
      char newpath[PATH_MAX];
      getcwd(newpath, PATH_MAX);
      strcat(newpath, "/");
      strcat(newpath, entry->d_name);
      f(newpath);
    }
  }

  while (wait(NULL) > 0) {
  }

  if (is_child) {
    // Just to make sure no child processes become orphan
    while (wait(NULL) > 0) {
    }
    exit(0);
  }
  closedir(dir);
  chdir(current_dir);
  return 0;
}

void setLDTflag(u_int8_t fla) {
  flag = fla;
}
