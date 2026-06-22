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

typedef struct {
    int length;
    op *list;
} op_list;

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

static op_list get_op_list(file config) {

    op_list return_list = {0, NULL};
    op *buffer = malloc(sizeof(op)*50);

    int index = 0;
    int buffer_index = 0;
    while (true) {

        if (buffer_index >= 50) {
            
            op *temp = realloc(return_list.list, return_list.length*sizeof(op));
            memcpy(temp + ((return_list.length - buffer_index)*sizeof(op)), buffer, buffer_index*sizeof(op));
            //free(return_list.list);
            return_list.list = temp;

        }

        if (index >= config.size) {
            
            op *temp = realloc(return_list.list, return_list.length*sizeof(op));
            memcpy(temp + ((return_list.length - buffer_index)*sizeof(op)), buffer, buffer_index*sizeof(op));
            //free(return_list.list);
            return_list.list = temp;
            break;
        }

        op current_op = parse_char(config, index);
        index += current_op.length;
        if (current_op.token_type == white_space) {free(current_op.token); continue;}
        if (current_op.token_type == new_line) {free(current_op.token); continue;}
        if (current_op.token_type == line_comment) {free(current_op.token); continue;}
        
        return_list.length++;
        buffer[buffer_index] = current_op;
        buffer_index++;

    }
    free(buffer);
    return return_list;

}

config_file get_config(char file_path[]) {

    config_file return_config = {0, NULL};
    file config = read_file(file_path);

    if (!config.is_valid) {return return_config;}

    op_list list = get_op_list(config);

    int token_index = 0;

    size_t section_name_len = 0;
    char *section_name = NULL;

    size_t arg_cnt = 0;
    config_argument *arguments = NULL;

    while (true) {

        if (token_index >= list.length) {
            config_section temp_section = {section_name_len, arg_cnt, section_name, arguments};
            return_config.section_cnt++;
            config_section *temp_section_list = realloc(return_config.sections, return_config.section_cnt*sizeof(config_section));
            temp_section_list[return_config.section_cnt-1] = temp_section;

            //free(return_config.sections);
            return_config.sections = temp_section_list;

            arg_cnt = 0;
            arguments = NULL;
            break;
        }

        op current_token = list.list[token_index];

        if (current_token.token_type == brac_open && !(token_index + 3 > list.length)) {

            op next_token = list.list[token_index+1];
            op after_next_token = list.list[token_index+2];

            if (next_token.token_type == unknown_op && after_next_token.token_type == brac_close) {
                
                if (section_name != NULL) {

                    config_section temp_section = {section_name_len, arg_cnt, section_name, arguments};
                    return_config.section_cnt++;
                    config_section *temp_section_list = realloc(return_config.sections, return_config.section_cnt*sizeof(config_section));
                    temp_section_list[return_config.section_cnt-1] = temp_section;

                    //free(return_config.sections);
                    return_config.sections = temp_section_list;

                    arg_cnt = 0;
                    arguments = NULL;

                }
                
                token_index += 3;
                section_name = next_token.token;
                section_name_len = next_token.length;
                continue;
            }

        }

        if (current_token.token_type == arg_end && !(token_index - 3 < 0)) {

            op value_token = list.list[token_index-1];
            op sep_token = list.list[token_index-2];
            op key_token = list.list[token_index-3];

            if (value_token.token_type == unknown_op && sep_token.token_type == arg_sep && key_token.token_type == unknown_op) {

                config_argument temp_arg = {key_token.length, value_token.length, key_token.token, value_token.token};
                arg_cnt++;
                config_argument *temp_arguments = realloc(arguments, arg_cnt*sizeof(config_argument));
                temp_arguments[arg_cnt-1] = temp_arg;

                //free(arguments);
                arguments = temp_arguments;

            }

        }

        token_index++;

    }

    return return_config;
}

config_section get_section(config_file config, char section_name[]) {

    for (int i=0;i<config.section_cnt;i++) {
        config_section section = config.sections[i];
        if (section.section_name == section_name) {
            return section;
        }

    }

    config_section default_section = {strlen(section_name),0,section_name,NULL};

    return default_section;

}

bool free_config(config_file config) {

    for (int sec=0;sec<config.section_cnt;sec++) {

        config_section section = config.sections[sec];

        for (int arg=0;arg<section.argument_cnt;arg++) {

            config_argument argument = section.arguments[arg];

            free(argument.key);
            free(argument.value);

        }

        free(section.arguments);

    }

    free(config.sections);

    return true;
}
