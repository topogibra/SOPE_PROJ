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
#include "log.h"

bool recurs = false;
char currentdir[PATH_MAX];
bool all_father = true;
int nofiles = 0;
int nodir = 0;

void dirlog(char* nlog) {
  char log[PATH_MAX];
  if (nlog == NULL) {
    sprintf(log, "New directory: %d/%d directories/files at this time.", nodir,
            nofiles);
  } else {
    log_activity(nlog);
    return;
  }
  log_activity(log);
}

void usr1_file_handler(int signo) {
  nofiles++;
}

void usr2_dir_handler(int signo) {
  nodir++;
}

void set_handlers(struct sigaction* sig1, struct sigaction* sig2) {
  struct sigaction sig;

  // usr1
  sig.sa_handler = usr1_file_handler;
  sig.sa_flags = SA_RESTART;

  sigaction(SIGUSR1, &sig, sig1);

  // usr2
  sig.sa_handler = usr2_dir_handler;
  sig.sa_flags = SA_RESTART;

  sigaction(SIGUSR2, &sig, sig2);
}

void set_recur(bool b) {
  recurs = b;
}

void printcwd() {
  char tmp[PATH_MAX];
  getcwd(tmp, PATH_MAX);
  printf("CWD -> %s\n", tmp);
}

void analyzer(char* path, void (*f)(char*)) {
  // Restore sig1 and sig2 handlers
  struct sigaction sig1, sig2;

  set_handlers(&sig1, &sig2);

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
      // killpg(0, SIGUSR2);
      // dirlog(NULL);
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
      // killpg(0, SIGUSR1);
      if (!all_father) {
        printf("%s", currentdir);
      }
      f(entry->d_name);
    }
  }

  for (; num_childs != 0; num_childs--) {
    wait(NULL);
  }

  // if (all_father) {
  //   printf("Files/Dir %d/%d\n", nofiles, nodir);
  // }

  sigaction(SIGUSR1, &sig1, NULL);
  sigaction(SIGUSR2, &sig2, NULL);
  closedir(dir);
}
