#include "flags-parser.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void print_help();
void print_arg_error();
int parse_flags(int argc, char **argv, cli_flags *flags);
int check_flags(cli_flags flags);

int parse_parameters(int argc, char **argv, cli_flags *flags) {
    int error;
    error = parse_flags(argc, argv, flags);
    if (error == PARSING_ERROR) {
        return PARSING_ERROR;
    } else if (error == PRINT_HELP) {
        return PRINT_HELP;
    }
    error = check_flags(*flags);
    if (error) {
        return PARSING_ERROR;
    }
    flags->root_dir = argv[optind];
    return EXIT_SUCCESS;
}

int parse_flags(int argc, char **argv, cli_flags *flags) {
    int c;
    while (true) {
        int option_index = 0;
        static struct option long_options[] = {
            {"archive", 0, 0, 'a'}, {"extract", 0, 0, 'e'}, {"help", 0, 0, 'h'}, {0, 0, 0, 0}};
        c = getopt_long(argc, argv, "aeh", long_options, &option_index);
        if (c == -1) break;
        switch (c) {
            case 'a':
                flags->archive = true;
                break;
            case 'e':
                flags->extract = true;
                break;
            case 'h':
                return PRINT_HELP;
            case '?':
                return PARSING_ERROR;
            default:;
        }
    }
    return EXIT_SUCCESS;
}

int check_flags(cli_flags flags) {
    if (flags.extract && flags.archive) {
        return PARSING_ERROR;
    }
    return EXIT_SUCCESS;
}
