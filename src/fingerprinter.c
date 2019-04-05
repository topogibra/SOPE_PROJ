//
// Created by Carlos Eduardo on 2019-03-11.
//

#include "fingerprinter.h"

/**
 * Checks if the provided hash_program is a valid program that hashes a file.
 * Valid program names are: md5sum, sha1sum, sha256sum
 * @param hash_program The name of the hash program
 * @return The hex size of the hash calculated by the program with name
 * hash_program, or 0 if the program doesn't exist
 */
int is_valid_hash_function(char* hash_program) {
  if (strcmp(hash_program, "md5sum") == 0) {
    return MD5SUM_SIZE;
  } else if (strcmp(hash_program, "sha1sum") == 0) {
    return SHA1SUM_SIZE;
  } else if (strcmp(hash_program, "sha256sum") == 0) {
    return SHA256SUM_SIZE;
  } else {
    return 0;
  }
}

/**
 * Closes the read end of the pipe and redirects STDOUT to the write end of the
 * pipe.
 * @param fd The file descriptors of a pipe
 */
void redirect_stdout_to_pipe(int fd[]) {
  if (close(fd[0]) != 0) {
    perror("Error closing file");
    exit(6);
  }
  if (dup2(fd[1], STDOUT_FILENO) == -1) {
    perror("Error redirecting file descriptor");
    exit(7);
  }
  if (close(fd[1]) != 0) {
    perror("Error closing file");
    exit(6);
  }
}

/**
 * Creates a pipe and checks if it opened successfully
 * @param fd
 */
void open_pipe(int fd[]) {
  if (pipe(fd) == -1) {
    perror("Failed to open pipe");
    exit(3);
  }
}

/**
 * Verifies if a file called file_name exists
 * @param file_name The name of the file
 */
void check_file_exists(char* file_name) {
  if (access(file_name, F_OK) == -1) {
    perror("File not found");
    exit(2);
  }
}

/**
 * Gets the hex size of the checksum array returned by the hash function
 * hash_program
 * @param hash_program The name of the hash program
 * @return The number of bytes of the hex hash returned by program called
 * hash_program
 */
int get_checksum_size(char* hash_program) {
  int checksum_size;
  if ((checksum_size = is_valid_hash_function(hash_program)) == 0) {
    printf("Invalid hash function\n");
    exit(1);
  }
  return checksum_size;
}

/**
 * Runs the hash program and gets it's output
 * @param fd The pipe's file descriptors
 * @param hash_program The hash program
 * @param file_name The name of the file to be hashed
 */
void exec_hash_command(int fd[], char* hash_program, char* file_name) {
  redirect_stdout_to_pipe(fd);
  execlp(hash_program, hash_program, file_name, NULL);
  perror("Failed to execute hash function");
  exit(5);
}

/**
 * Retrieves the hash from the pipe, removes the newline character and closes
 * the pipe's read and write ends
 * @param fd The pipe's file descriptors
 * @param hash_program The hash program
 * @return The file's hash without newline character
 */
void get_hash_from_pipe(int fd[], int checksum_size, char* checksum) {
  int stat;
  // char checksum[checksum_size + 1];

  if (close(fd[1]) != 0) {
    perror("Error closing file");
    exit(6);
  }
  ssize_t bytes_read = read(fd[0], checksum, sizeof(char) * checksum_size);
  if (bytes_read == -1) {
    perror("Error reading from pipe to buffer");
    exit(8);
  }
  checksum[checksum_size] = '\0';
  wait(&stat);
  // Truncate array so that string finishes where it's supposed to
  // checksum[checksum_size + 1] = '\0';
  if (close(fd[0]) != 0) {
    perror("Error closing file");
    exit(6);
  }
  // char *ptr = (char *)malloc(strlen(checksum) + 1);
  // strcpy(ptr, checksum);
  // return ptr;
}

/**
 * Generates a checksum for a program using a specific hash program
 * @param file_name Name of the file to be hashed
 * @param hash_program Name of the hash program
 * @return The hash of the file
 */
void gen_checksum(char* file_name, char* hash_program, char* checksum) {
  pid_t pid;
  int fd_size = 2;
  int fd[fd_size];
  int checksum_size = get_checksum_size(hash_program);

  check_file_exists(file_name);
  open_pipe(fd);

  pid = fork();
  if (pid == -1) {
    perror("Error creating fork");
    exit(4);
  } else if (pid == 0) {
    setpgid(0,getpid());
    exec_hash_command(fd, hash_program, file_name);
  } else {
    get_hash_from_pipe(fd, checksum_size, checksum);
  }
}
