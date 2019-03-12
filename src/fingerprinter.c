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
     * TODO: Handle possible errors with the code
     */

    pid_t pid;
    char checksum[1024];
    int fd[2];

    if (pipe(fd) == -1)
    {
        exit(5);
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
        execlp("md5sum", "md5sum", name, NULL);
        perror("Failed to execute md5sum: ");
        exit(3);
    }
    else
    {
        int stat;

        close(fd[1]);
        int nbytes = 0;
        int naux = 0;
        while ((naux = read(fd[0], checksum + nbytes, sizeof(checksum)-nbytes)) > 0)
        {
            nbytes += naux;
        }
        wait(&stat);
        checksum[16] = '\0';
        close(fd[0]);
    }

    printf("%s", checksum);

    return 0;
}
