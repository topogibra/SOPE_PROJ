#ifndef DIRTREE_H
#define DIRTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/**
 * @brief Applies the function f to all files it finds while traversing through the directory tree on the path. By default it only searches for files in the directory given by the argument. For the function to search in sub-directories you have to set the bitflag with R_LIST.  To apply f to directories use A_DIR macro to set the bitflag. It will exit with the code 139 if the path given is not a directory.
 * 
 * @param path Path of the directory 
 * @param f Function to be applied to all files. Must receive char * that is the absolute path of the file.
 * @return int 0 if no error; 139 if the path is not a directory
 */
int listdtree(char* path, void (*f)(char *));

/**
 * @brief Set the flag in list tree mode. 0x0 is the default.
 * 
 * @param fla Bit flag.
 */
void setLDTflag(u_int8_t fla);

/**
 * @brief Recursive Listing.
 * 
 */
#define R_LIST 0x01

/**
 * @brief Apply function f to directories.
 * 
 */
#define A_DIR 0x02



#endif
