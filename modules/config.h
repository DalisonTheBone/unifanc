
#ifndef CONFIG_H
#define CONFIG_H
#include <stddef.h>
#include "file.h"

// Types
typedef struct {
    size_t len;
    char *ptr;
} string;

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

//
config_file get_config(char file_path[]);

#endif
