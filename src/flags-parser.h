#ifndef LAB_1_FLAGS_PARSER_H
#define LAB_1_FLAGS_PARSER_H

#include <stdbool.h>

enum ERRORS {
    PRINT_HELP = 1,
    PARSING_ERROR,
};

typedef struct {
    bool archive;
    bool extract;
    char *dir_path;
    char *archive_path;
} cli_flags;

int parse_parameters(int argc, char **argv, cli_flags *flags);

#endif
