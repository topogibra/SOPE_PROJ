//
// Created by Carlos Eduardo on 2019-03-11.
//

#include "fingerprinter.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


char* cenas()
{
    char *teste = malloc(sizeof(char)*1024);
    return teste;
}

int md5(char *name, FILE* file)
{
    /**
     * TODO: Handle possible errors with the code
     */

    pid_t pid;
    char checksum[1024];
    char md5sumPath[1024];
    int fd[2];

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
        execlp("md5sum", "md5sum", name, NULL);
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
