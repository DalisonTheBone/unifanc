#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "modules.h"

// Types

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

int get_temps(char module_path[]) {

    int len = strlen(module_path);

    char *cmd = malloc(len + 3);
    memmove(cmd, module_path, len);
    memmove(cmd + len, " g", 3);
    //printf("%s\n", cmd);
    cmd_output cmd_responce = run_command(cmd);
    free(cmd);

    return atoi(cmd_responce.bytes);

} 

void init_module(char module_path[]) {

    int len = strlen(module_path);

    char *cmd = malloc(len + 3);
    memmove(cmd, module_path, len);
    memmove(cmd + len, " g", 3);
    
    run_command(cmd);
    free(cmd);

} 

bool set_fan_speed(char module_path[], int percent) {

    char *cmd = malloc(strlen(module_path) + 7);
    sprintf(cmd, "%s s %d", module_path, percent);
    //printf("%s\n",cmd);
    run_command(cmd);
    free(cmd);

    return true;
}

