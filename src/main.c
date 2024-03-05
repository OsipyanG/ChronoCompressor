#include <stdbool.h>
#include <stdio.h>

#include "flags-parser.h"

void print_help();
void print_arg_error();

int main(int argc, char *argv[]) {
    cli_flags flags = {};
    int err = 0;

    err = parse_parameters(argc, argv, &flags);
    if (err == PARSING_ERROR) {
        print_arg_error();
    } else if (err == PRINT_HELP) {
        print_help();
    }

    return err;
}

void print_arg_error() { fprintf(stderr, "Try 'ar --help' for more information.\n"); }

void print_help() {
    fprintf(stderr, "Usage: ar [OPTION] [FILE]...\n");
    fprintf(stderr, "Archive and unzip files\n");
    fprintf(stderr, "  -a, --archive  Archive files and directories\n");
    fprintf(stderr, "  -e, --extract  Extract the archive\n");
}
