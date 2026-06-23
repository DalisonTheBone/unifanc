
#ifndef MODULES_H
#define MODULES_H

#include <stdbool.h>

// Types
typedef struct {
    int size;
    char *bytes;
} cmd_output;

// Functions
cmd_output run_command(char args[]);

int get_temps(char module_path[]);
bool set_fan_speed(char module_path[], int percent);
void init_module(char module_path[]);

#endif
