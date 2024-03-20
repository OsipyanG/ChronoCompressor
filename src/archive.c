//
// Created by gurgeno on 3/20/24.
//

#include "archive.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void write_file_to_archive(FILE *archive, const char *file_path);
void archive_directory(const char *dir_path, const char *base_path, FILE *archive);

void archive(const char *path) {
    FILE *archive;
    char archive_name[1024];

    snprintf(archive_name, sizeof(archive_name), "archive.bin");
    archive = fopen(archive_name, "wb");

    if (!archive) {
        perror("fail open archive");
        exit(EXIT_FAILURE);
    }

    archive_directory(path, path, archive);
    fclose(archive);
}

void write_file_to_archive(FILE *archive, const char *file_path) {
    struct stat st;
    if (stat(file_path, &st) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    file_metadata metadata = {.path_length = strlen(file_path),
                              .file_size = S_ISDIR(st.st_mode) ? 0 : st.st_size,
                              .mode = st.st_mode,
                              .type = S_ISDIR(st.st_mode) ? 1 : 0,
                              .mod_time = st.st_mtime};

    // Записываем структуру метаданных
    fwrite(&metadata, sizeof(metadata), 1, archive);
    // Непосредственно за структурой записываем путь к файлу
    fwrite(file_path, sizeof(char), metadata.path_length, archive);

    // Если это файл, записываем его содержимое
    if (!S_ISDIR(st.st_mode)) {
        FILE *file = fopen(file_path, "rb");
        char buffer[1024];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, bytes, archive);
        }
        fclose(file);
    }
}
void archive_directory(const char *dir_path, const char *base_path, FILE *archive) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dir_path))) return;

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            archive_directory(path, base_path, archive);
        } else {
            write_file_to_archive(archive, path);
        }
    }
    closedir(dir);
}
