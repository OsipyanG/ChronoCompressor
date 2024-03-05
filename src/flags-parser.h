#ifndef LAB_1_FLAGS_PARSER_H
#define LAB_1_FLAGS_PARSER_H

#include <stdbool.h>

enum ERRORS {
    PRINT_HELP = 1,
    PARSING_ERROR,
    MALLOC_ERROR,
};

typedef struct {
    bool archive;
    bool extract;
    char *root_dir;
} cli_flags;

int parse_parameters(int argc, char **argv, cli_flags *flags);

#endif
