
#ifndef FILE_H
#define FILE_H
#include <stddef.h>
#include <stdbool.h>

// Types
typedef struct {
    bool is_valid;
    int size;
    char *bytes;
} file;

// Checks if file and path to file exist
bool is_valid_path(char file_path[]);

// Reads file at path and returns file
file read_file(char file_path[]);

// Writes bytes to file path if file and file path exist
bool write_file(char file_path[], char *bytes);

#endif
