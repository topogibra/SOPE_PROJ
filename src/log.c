#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static FILE * file = NULL;
static double initialTime = 0;

/**
 * Sets the global variable file
 * @param file_temp The file to which log_activity will write
 */

void set_file(FILE* file_temp){
  file = file_temp;
}
/**
 * Get the global variable file
 * @return file
 */
FILE* get_file(){
  return file;
}

/**
 * Sets the global variable initialTime
 * @param time The initialTime
 */
void set_initialTime(double time){
  initialTime = time;
}
/**
 * Get the initialTime
 * @return Initial time
 */
double get_initialTime(){
  return initialTime;
}

/**
 * @brief Opens a file and checks for errors
 *
 * @param file The file being opened
 * @param pathname The file location
 * @param mode The mode which fopen will run on
 *
 * @return 0 if the program executed correctly
 */
int open_file(FILE** file, const char *pathname, const char *mode) {
    if((*file=fopen(pathname, mode)) == NULL) {
        perror("Failed to open file");
        exit(1);
    }
    return 0;
}


/**
 * @brief Closes a file and checks for errors
 *
 * @param file The file being closed
 *
 * @return 0 if the program executed correctly

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
 * @brief Gets the current time of the system
 *
 * @return The time of the system
 */
double get_time() {
    clock_t time;
    if((time=clock()) == -1) {
        perror("Could not get time");
        exit(1);
    }
    return (double) time/CLOCKS_PER_SEC;
}

/**
 * @brief Writes to a file information about a certain program action
 * @param eventDescription Text describing the information that will be written to the file
 */
void log_activity( char* eventDescription) {
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
