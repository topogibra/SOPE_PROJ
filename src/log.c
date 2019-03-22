#include "log.h"
#include <string.h>

/**
 * Opens a file and checks for errors. 
 */
int open_file(FILE** file, const char *pathname, const char *mode) {
    if((*file=fopen(pathname, mode)) == NULL) {
        perror("Failed to open file");
        exit(1);
    }
    return 0;
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
 * Sets constant time_value with the specified value
 */
double get_time() {
    clock_t time;
    if((time=clock()) == -1) {
        perror("Couldn not get time");
        exit(1);
    }
    return (double) time/CLOCKS_PER_SEC;
}

/**
 * Writes the log info of the program (provided by event_description) 
 * to a file
 */
void log_activity(FILE* file, double initialTime, char* eventDescription) {
    char buffer[1024];
    double currentTime = get_time();
    double timeStamp = (currentTime - initialTime);
    pid_t pid = getpid();

    if(strcmp(eventDescription, "") == 0) {
        printf("Event description cannot be an empty string");
        exit(1);
    }

    sprintf(buffer, "%f - %d - %s", timeStamp, pid, eventDescription);
    fprintf(file, "%s", buffer);
}
