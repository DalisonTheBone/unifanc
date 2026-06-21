#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Types
typedef struct {
    int size;
    char *bytes;
} cmd_output;

// Functions

cmd_output run_command(char args[]) {

    cmd_output cmd_responce = {
        0, NULL
    };

    FILE *process_ptr = popen(args, "r");

    if (process_ptr == NULL) {return cmd_responce;}

    char *bytes = malloc(256);
    char byte;
    int byte_index = 0;

    while (true) {
        
        int current_byte = fgetc(process_ptr);
        byte = current_byte;

        if (byte_index >= 256) {

            char *return_bytes = malloc(cmd_responce.size);
            memmove(return_bytes, cmd_responce.bytes, cmd_responce.size - byte_index);
            memmove(return_bytes + (cmd_responce.size - byte_index), bytes, byte_index);

            free(cmd_responce.bytes);
            cmd_responce.bytes = return_bytes;

            byte_index = 0;

            if (current_byte == -1) {break;}

        }

        if (current_byte == -1) {

            char *return_bytes = malloc(cmd_responce.size);
            memmove(return_bytes, cmd_responce.bytes, cmd_responce.size - byte_index);
            memmove(return_bytes + (cmd_responce.size - byte_index), bytes, byte_index);

            free(cmd_responce.bytes);
            cmd_responce.bytes = return_bytes;

            break;
        }

        cmd_responce.size++;
        bytes[byte_index] = byte;
        byte_index++;

    }

    pclose(process_ptr);
    free(bytes);

    return cmd_responce;

}


