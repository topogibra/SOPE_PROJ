//
// Created by Carlos Eduardo on 2019-03-11.
//

#include "fingerprinter.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int is_valid_hash_function(char* hash_command_name)
{
    if(strcmp(hash_command_name, "md5sum") != 0 &&
    strcmp(hash_command_name, "sha1sum") != 0 &&
    strcmp(hash_command_name, "sha256sum")!= 0)
    {
        return 0;
    }
    return 1;
}

char* gen_checksum(char* file_name, char* hash_command_name)
{
    pid_t pid;
    char checksum[1024];
    int fd[2];

    if (!is_valid_hash_function(hash_command_name))
    {
        perror("Invalid hash function");
        exit(1);
    }

    if (access(file_name, F_OK) == -1)
    {
        perror("File not found");
        exit(2);
    }

    if (pipe(fd) == -1)
    {
        perror("Failed to build pipe");
        exit(3);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Error creating fork");
        exit(4);
    }
    else if (pid == 0)
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
        if(close(fd[1]) != 0)
        {
            perror("Error closing file");
            exit(6);
        }
        execlp(hash_command_name, hash_command_name, file_name, NULL);
        perror("Failed to execute hash function");
        exit(5);
    }
    else
    {
        int stat;
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
        checksum[16] = '\0';
        if(close(fd[0]) != 0){
            {
                perror("Error closing file");
                exit(6);
            }
        }
    }
    char* ptr = (char*) malloc(strlen(checksum)+1);
    strcpy(ptr, checksum);
    printf("%s", ptr);
    return ptr;
}
