#include "dirtree.h"
#include <dirent.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.h"

u_int8_t flag = 0x0;
int nofiles = 0;
int nodir = 0;

void dirlog(char* nlog);

void usr1_file_handler(int signo) {
  // printf("hello\n");
  nofiles++;
  // aqui seria nofiles++
}

void usr2_dir_handler(int signo) {
  nodir++;
  // aqui seria nodir ++
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

int listdtree(char* path, void (*f)(char*)) {
  struct sigaction sig1, sig2;

  // Safeguard to maintain the current working directory
  char current_dir[PATH_MAX];
  DIR* dir;

  struct dirent* entry;
  bool is_child = false;

  getcwd(current_dir, PATH_MAX);

  if (chdir(path) != 0) {
    exit(4);
  }

  set_handlers(&sig1, &sig2);

  if (setpgid(getpid(), 0) != 0) {
    printf("Failed to set pgid");
    exit(-1);
  }

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
      killpg(0, SIGUSR2);
      dirlog(NULL);
      if (flag & R_LIST) {
        pid_t pid = fork();

        if (pid == 0) {  // child
          is_child = true;
          if (flag & A_DIR) {
            f(newpath);
          }
          chdir(newpath);
          closedir(dir);
          dir = opendir(".");
        }
      }
      continue;
    }
    if (entry->d_type == DT_REG) {
      killpg(0, SIGUSR1);
      char newpath[PATH_MAX];
      getcwd(newpath, PATH_MAX);
      strcat(newpath, "/");
      strcat(newpath, entry->d_name);
      f(newpath);
      continue;
    } else {
      char tmp[500];
      strcat(tmp, "IDK WTF IS THIS\n");
      dirlog(tmp);
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

  // Reset usr1 and usr2 handlers

  sigaction(SIGUSR1, &sig1, NULL);
  sigaction(SIGUSR2, &sig2, NULL);

  return 0;
}

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

void setLDTflag(u_int8_t fla) {
  flag = fla;
}
