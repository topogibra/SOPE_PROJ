//
// Created by Carlos Eduardo on 2019-03-11.
//

#include "fingerprinter.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int is_valid_hash_function(char *hash_command_name)
{

    if (strcmp(hash_command_name, "md5sum") == 0)
    {
        return MD5SUM_SIZE;
    }
    else if (strcmp(hash_command_name, "sha1sum") == 0)
    {
        return SHA1SUM_SIZE;
    }
    else if (strcmp(hash_command_name, "sha256sum") == 0)
    {
        return SHA256SUM_SIZE;
    }
    else
    {
        return 0;
    }
}


void redirect_stdout_to_pipe(int fd[])
{
    if (close(fd[0]) != 0)
    {
        perror("Error closing file");
        exit(6);
    }
    if (dup2(fd[1], STDOUT_FILENO) == -1)
    {
        perror("Error redirecting file descriptor");
        exit(7);
    }
    if (close(fd[1]) != 0)
    {
        perror("Error closing file");
        exit(6);
    }
}

void open_pipe(int fd[])
{
    if (pipe(fd) == -1)
    {
        perror("Failed to build pipe");
        exit(3);
    }
}

void check_file_exists(char* file_name)
{
    if (access(file_name, F_OK) == -1)
    {
        perror("File not found");
        exit(2);
    }
}

int get_checksum_size(char* hash_command_name)
{
    int checksum_size;
    if ((checksum_size = is_valid_hash_function(hash_command_name)) == 0)
    {
        perror("Invalid hash function");
        exit(1);
    }
    return checksum_size;
}

void send_hash_to_pipe(int fd[], int fd_size, char* hash_command_name, char* file_name)
{
    redirect_stdout_to_pipe(fd);
    execlp(hash_command_name, hash_command_name, file_name, NULL);
    perror("Failed to execute hash function");
    exit(5);
}

char* get_hash_from_pipe(int fd[], char* hash_command_name)
{
    int stat;
    int checksum_size = get_checksum_size(hash_command_name);
    char checksum[checksum_size + 1];

    if (close(fd[1]) != 0)
    {
        perror("Error closing file");
        exit(6);
    }
    ssize_t bytes_read = read(fd[0], checksum, sizeof(checksum));
    if (bytes_read == -1)
    {
        perror("Error reading from pipe to buffer");
        exit(8);
    }
    wait(&stat);
    // Truncate array so that string finishes where it's supposed to
    checksum[checksum_size + 1] = '\0';
    if (close(fd[0]) != 0)
    {
        perror("Error closing file");
        exit(6);
    }
    char *ptr = (char *)malloc(strlen(checksum) + 1);
    strcpy(ptr, checksum);
    return ptr;
}

char *gen_checksum(char* file_name, char* hash_command_name)
{
    pid_t pid;
    int fd_size = 2;
    int fd[fd_size];
    char* checksum;


    check_file_exists(file_name);
    open_pipe(fd);

    pid = fork();
    if (pid == -1)
    {
        perror("Error creating fork");
        exit(4);
    }
    else if (pid == 0)
    {
        send_hash_to_pipe(fd, fd_size, hash_command_name, file_name);
    }
    else
    {
        checksum = get_hash_from_pipe(fd, hash_command_name);
    }

    return checksum;
}
