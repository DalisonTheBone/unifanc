#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "config.h"

// Op
typedef enum {
    white_space = 0, //
    new_line = 1,
    arg_key = 2,
    arg_val = 3,
    arg_sep = 4,
    arg_end = 5,
    brac_open = 6,
    brac_close = 7,
    sect_name = 8,
    line_comment = 9,
    unknown_op = 10
} ops;

typedef struct {
    int length;
    ops token_type;
    char *token;
} op;

// Functions

static ops get_char_token(file config, int index) {
    if (config.bytes[index] == ' ') {
        return white_space; 
    }

    if (config.bytes[index] == '=') {
        return arg_sep; 
    }

    if (config.bytes[index] == ';') {
        return arg_end; 
    }

    if (config.bytes[index] == '{') {
        return brac_open; 
    }

    if (config.bytes[index] == '}') {
        return brac_close; 
    }

    if (config.bytes[index] == '#') {
        return line_comment; 
    }

    if (config.bytes[index] == '\n') {
        return new_line;
    }

    return unknown_op;
}

static op parse_char(file config, int index) {

    op return_op = {1, unknown_op, NULL};
    
    return_op.token_type = get_char_token(config, index);

    if (return_op.token_type == unknown_op) {
        
        int i = 1;
        while (true) {

            if (get_char_token(config, index + i) != unknown_op) {break;}
            if ((index + i) >= config.size) {break;}

            return_op.length++;
            i++;

        }

    }

    if (return_op.token_type == line_comment) {

        int i = 1;
        while (true) {

            if (get_char_token(config, index + i) == new_line) {break;}
            if ((index + i) >= config.size) {break;}
            
            return_op.length++;
            i++;

        }

    }

    return_op.token = malloc(return_op.length + 1);
    memcpy(return_op.token, config.bytes + index, return_op.length);
    return_op.token[return_op.length] = '\0';

    return return_op;

}

config_file get_config(char file_path[]) {

    config_file return_config = {0, NULL};
    file config = read_file(file_path);

    if (!config.is_valid) {return return_config;}

    int index = 0;

    while (true) {

        if (index >= config.size) {break;}

        op current_op = parse_char(config, index);
        index += current_op.length;
        if (current_op.token_type == white_space) {continue;}
        if (current_op.token_type == new_line) {continue;}
        if (current_op.token_type == line_comment) {continue;}
        printf("%s\n", current_op.token);

    }
    printf("%d\n", config.size);
    return return_config;

}

