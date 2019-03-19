#include "log.h"

/**
 * Opens a file and checks for errors. 
 */
int open_file(FILE** file, const char *pathname, const char *mode) {
    if((*file=fopen(pathname, mode)) == NULL) {
        perror("Failed to open file");
        exit(1);
    }
}

/**
 * Closes a file and checks for errors
 */
int close_file(FILE* file) {
    int return_value;
    if((return_value=fclose(file)) == EOF) {
        perror("Failed to close file"); 
        exit(2);
    }
    return 0;
}

/**
 * Writes the log info of the program (provided by event_description) 
 * to a file
 */
void log_activity(FILE* file, char* event_description) {
    char buffer[1024];
    pid_t pid = getpid();
    sprintf(buffer, "%d - %s", pid, event_description);
    printf("%s", buffer);
}
