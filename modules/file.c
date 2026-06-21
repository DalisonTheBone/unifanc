#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Types
typedef struct {
    bool is_valid;
    int size;
    char *bytes;
} file;

// Functions
bool is_valid_path(char file_path[]) {return access(file_path, F_OK) == 0;}

file read_file(char file_path[]) {
    file return_file = {
        true, 0, malloc(0)
    };

    if (!is_valid_path(file_path)) {return_file.is_valid = false; return return_file;}

    FILE *file_ptr = fopen(file_path, "rb");

    if (file_ptr == NULL) {
        return_file.is_valid = false;
        fclose(file_ptr);
        return return_file;
    }

    char *bytes = malloc(256);
    char byte;
    int byte_index = 0;

    while (true) {

        int current_byte = fgetc(file_ptr);
        byte = current_byte;
        
        if (byte_index >= 256) {

            char *new_byte_buffer = malloc(return_file.size);
            
            memcpy(new_byte_buffer, return_file.bytes, return_file.size - 256);
            memmove(new_byte_buffer + (return_file.size - 256), bytes, byte_index);

            free(return_file.bytes);
            return_file.bytes = new_byte_buffer;

            byte_index = 0;

            if (current_byte == -1) {break;}
            
        }

        if (current_byte == -1) {
            
            char *new_byte_buffer = malloc(return_file.size);

            memcpy(new_byte_buffer, return_file.bytes, return_file.size - byte_index + 1);
            memmove(new_byte_buffer + (return_file.size - byte_index), bytes, byte_index);

            free(return_file.bytes);
            return_file.bytes = new_byte_buffer;

            break;
        }

        return_file.size++;
        bytes[byte_index] = byte;
        byte_index++;

    }
    
    fclose(file_ptr);

    return return_file;
}

bool write_file(char file_path[], char *to_write) {

    if (!is_valid_path(file_path)) {return false;}

    FILE *file_ptr = fopen(file_path, "wb");

    if (file_ptr == NULL) {return false;}

    fputs(to_write, file_ptr);
    fclose(file_ptr);

    return true;
}
