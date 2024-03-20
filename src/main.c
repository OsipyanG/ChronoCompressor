#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "archive.h"
#include "unarchive.h"

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    char *input_path = NULL;
    enum { MODE_NONE, ARCHIVE, UNARCHIVE } mode = MODE_NONE;

    static struct option long_options[] = {{"archive", required_argument, 0, 'a'},
                                           {"unarchive", required_argument, 0, 'u'},
                                           {"path", required_argument, 0, 'p'},
                                           {"help", no_argument, 0, 'h'},
                                           {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "ha:u:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'a':
                mode = ARCHIVE;
                input_path = optarg;
                break;
            case 'u':
                mode = UNARCHIVE;
                input_path = optarg;
                break;
            case 'h':
                printf("Usage: %s --archive <path_to_archive> | --unarchive <path_to_unarchive> | --help\n",
                       argv[0]);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr,
                        "Usage: %s --archive <path_to_archive> | --unarchive <path_to_unarchive> | --help\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Проверка, что путь был указан
    if (input_path == NULL) {
        fprintf(stderr, "You must specify a path. Use --help for usage.\n");
        exit(EXIT_FAILURE);
    }

    // Вызов функций архивации или разархивации в зависимости от режима
    if (mode == ARCHIVE) {
        archive(input_path);
    } else if (mode == UNARCHIVE) {
        unarchive(input_path);
    } else {
        fprintf(stderr, "Invalid mode. Use --help for usage.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
