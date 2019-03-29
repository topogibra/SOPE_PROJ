#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "parser.h"
#include "file.h"
#include "log.h"

#define NO_FLAGS 3

void sigint_handler(int signal) {
    printf("Finishing tasks before closing...\n");
    wait(-1);
    printf("%d\n", getpid());
    exit(1);
}

int main() {
    FILE* file = NULL;
    open_file(&file, "logfile.txt", "a");

    struct sigaction action;
    action.sa_handler = sigint_handler;
    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);

    fork();

    while(1);

    double initialTimeStamp = get_time();
    set_file(file);
    set_initialTime(initialTimeStamp);
    //open_file(&file, "log_entries.txt", "a");
    ////close_file(file);
/*
    char* checksum = gen_checksum("src/main.c", "sha256sum");
    printf("%s\n", checksum);
    free(checksum);
    */
    log_activity("Computed log\n");

    /*
    char *flagArguments[NO_FLAGS];
    for(size_t i = 0; i < NO_FLAGS; i++)
    {
        flagArguments[i] = "";
    }

    uint8_t flags = parseArguments(argc, argv, flagArguments, NO_FLAGS);

    uint8_t num_hash = 0; // number of hashes passed as argument
    char * hash[3]; // array with the hashes strings

    if(flags & CLCHASH){
      num_hash = getHashArguments(flagArguments[0],hash);
    }

    int output = STDOUT_FILENO;

    if(flags & SAVECSV){
      if((output = open(flagArguments[1], O_WRONLY | O_CREAT | O_TRUNC , 0777 ))==-1){
        perror("open");
        exit(1);
      }
      if(dup2(output,STDOUT_FILENO)==-1){
        perror("dup2");
        exit(1);
      }
    }

    setFlags(hash,num_hash);

    info(flagArguments[NO_FLAGS-1]);

    if(flags & SAVECSV){
      close(output);
    }
    */

    return 0;
}
