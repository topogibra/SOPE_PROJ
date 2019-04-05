#include "file.h"
#include <limits.h>
#include "fingerprinter.h"

static int8_t num_flags = 0;
static char* hash_passed[3];

/**
 * Sets global file variables representing the number of hashes passed to the
 * program and an array of of string with each hash.
 * @param hash array with the strings of each hash passed to the program
 * @param hash_flags number of hashes passed to the function
 */
void setFlags(char* hash[], int8_t hash_flags) {
  num_flags = hash_flags;
  memcpy(hash_passed, hash, sizeof(char*) * num_flags);
}

/**
 * Displays files system info.
 * @param sb pointer to a struct stat of a file
 */
char* display_stat_info(struct stat* sb, char* str) {
  sprintf(str, "%lld, %x, %x, ", (long long)sb->st_size, sb->st_mode,
          sb->st_mode & 0x0777);
  for (int x = 0x00400, i = 0; x != 0; x >>= 1, i++) {  // print permissions
    if (sb->st_mode & x) {
      switch (i % 3) {
        case 0:
          strcat(str, "r");
          break;
        case 1:
          strcat(str, "w");
          break;
        case 2:
          strcat(str, "x");
          break;
        default:
          break;
      }
    } else
      strcat(str, "-");
    if (i % 3 == 2)
      x >>= 1;
  }
  strcat(str, ", ");
  char date[21];
  strftime(date, 21, "%Y-%m-%dT%H:%M:%S",
           localtime(&(sb->st_mtime)));  // modification
  strcat(str, date);
  strcat(str, ", ");
  strftime(date, 21, "%Y-%m-%dT%H:%M:%S",
           localtime(&(sb->st_ctime)));  // status
  strcat(str, date);
  return str;
}

/**
 * Displays file fingerprints.
 * @param file_path file path of the file
 */
void display_fingerprints(char* file_path, char* hash) {
  // char tmp[2048];
  sprintf(hash, "%s", "");
  int comma = 0;
  for (int i = 0; i < num_flags; ++i) {
    if (hash_passed[i] != NULL) {
      comma++;
    }
  }

  if (comma > 0) {
    strcat(hash, ", ");
  }

  for (int i = 0; i < num_flags; ++i) {
    if (!strcmp(hash_passed[i], "md5")) {
      char tmp[2048];
      gen_checksum(file_path, "md5sum", tmp);
      // strncat(hash, tmp, MD5SUM_SIZE);
      sprintf(hash, "%s%s", hash, tmp);
      comma--;
    } else if (!strcmp(hash_passed[i], "sha1")) {
      char tmp[2048];
      gen_checksum(file_path, "sha1sum", tmp);
      // strncat(hash, tmp, SHA1SUM_SIZE);
      sprintf(hash, "%s%s", hash, tmp);
      comma--;
    } else if (!strcmp(hash_passed[i], "sha256")) {
      char tmp[2048];
      gen_checksum(file_path, "sha256sum", tmp);
      sprintf(hash, "%s%s", hash, tmp);
      // strncat(hash, tmp, SHA256SUM_SIZE);
      comma--;
    }
    if (comma > 0) {
      strcat(hash, ", ");
    }
  }
}

/**
 * Displays file type info.
 * @param file_path file path of the file
 */
void display_file_type(char* file_path, char* str) {
  int link[2];
  char temp[1024];

  if (pipe(link) == -1)
    exit(EXIT_FAILURE);

  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0) {  // error
    perror("fork");
    exit(1);
  } else if (pid != 0) {  // pai

    close(link[1]);
    waitpid(pid, &status, 0);

    /*int nbytes = 0;
    int n_aux = 0;
    while ((n_aux = read(link[0], temp + nbytes, sizeof(temp) - nbytes)) > 0) {
      nbytes += n_aux;
    }*/

    ssize_t res;
    while ((res = read(link[0], temp, sizeof(temp) - 1)) > 0) {
      if (temp[res - 1] == 0xA)
        temp[res - 1] = 0;
      temp[res] = 0;
      strcat(str, temp);
    }

    // sprintf(str, "%s", temp);
    // strcpy(str, temp);
    close(link[0]);

    return;

  } else {  // filho
    setpgid(0, getpid());
    if (dup2(link[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(1);
    }
    close(link[0]);
    execlp("file", "file", "-b", file_path, NULL);
    exit(1);
  }
}

/**
 * Get file system status and open child process that redirects its output to
 * the father to get the file types. Then it displays its info and the
 * fingerprints specified by the file global variables.
 * @param path of the file in which the fuction will be applied
 */
void info1(char* file_path, char* buf);

void info(char* file_path) {
  struct stat sb;
  if (lstat(file_path, &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }
  char str[PATH_MAX];
  char temp[2048];
  // char temp2[2048];
  // char temp3[2048];

  strcat(str, file_path);
  strcat(str, ",");

  display_file_type(file_path, temp);
  for (size_t i = 0; i < strlen(temp); i++) {
    if (temp[i] == ',') {
      temp[i] = ' ';
    }
  }
  sprintf(str, "%s, %s,", file_path, temp);

  display_stat_info(&sb, temp);
  strcat(str, temp);

  display_fingerprints(file_path, temp);
  strcat(str, temp);

  printf("%s\n", str);
  str[0] = '\0';
  temp[0] = '\0';
}
