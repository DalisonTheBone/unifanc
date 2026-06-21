
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
    string key;
    string value;
} config_argument;

typedef struct {
    string section_name;

    size_t argument_cnt;
    config_argument *arguments;
} config_section;

typedef struct {
    size_t section_cnt;
    config_section *sections;
} config_file;

//
config_file get_config(char file_path[]);

#endif
