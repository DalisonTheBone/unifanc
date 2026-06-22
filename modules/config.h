
#ifndef CONFIG_H
#define CONFIG_H
#include <stddef.h>
#include "file.h"

// Types
typedef struct {
    size_t key_size;
    size_t value_size;

    char *key;
    char *value;
} config_argument;

typedef struct {
    size_t name_len;
    size_t argument_cnt;
    char *section_name;
    config_argument *arguments;
} config_section;

typedef struct {
    size_t section_cnt;
    config_section *sections;
} config_file;

typedef struct {
    bool success;
    int min;
    int max;
} range; 

//
config_file get_config(char file_path[]);

config_section get_section(config_file config, char section_name[]);

bool free_config(config_file config);

range parse_range(char range_string[], size_t string_size);

#endif
