//
// Created by Carlos Eduardo on 2019-03-11.
//

#include "fingerprinter.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int md5(char *name, FILE* file)
{
    /**
     *  TODO: Separate md5()'s fork code into two functions: one for the path finding, and another for executing md5sum
     *  The first function should give a char* with the path to md5sum. Need to find a way to make this work
     *  (without bad access errors).
     */

    pid_t pid;
    char checksum[1024];
    char md5sumPath[1024];
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Error creating pipe: ");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("Error creating fork: ");
        exit(2);
    }
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execl("/usr/bin/which", "which", "md5sum", NULL);
        perror("Failed to execute which: ");
        exit(3);
    }
    else
    {
        int stat;
        wait(&stat);
        close(fd[1]);
        while (read(fd[0], md5sumPath, sizeof(md5sumPath)));
        close(fd[0]);
    }

    md5sumPath[strlen(md5sumPath)-1] = '\0';

    pid = fork();
    if (pid == -1)
    {
        perror("Error creating fork: ");
        exit(2);
    }
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execl(md5sumPath, "md5sum", name, NULL);
        perror("Failed to execute md5sum: ");
        exit(3);
    }
    else
    {
        int stat;
        wait(&stat);
        close(fd[1]);
        while (read(fd[0], checksum, sizeof(checksum)));
        close(fd[0]);
    }

    md5sumPath[strlen(md5sumPath)-1] = '\0';

    return 0;
}
