//
// Created by gurgeno on 3/20/24.
//

#include "unarchive.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <utime.h>

#include "archive.h"

void create_directories(const char *path) {
    char *subpath, *ptr;

    subpath = strdup(path);
    if (subpath == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Пройти по пути и создать все директории
    for (ptr = subpath + 1; *ptr; ptr++) {
        if (*ptr == '/') {
            *ptr = '\0';  // Временно обрезать путь
            if (mkdir(subpath, 0755) == -1) {
                if (errno != EEXIST) {
                    perror("mkdir");
                    break;  // Выход, если не удалось создать директорию и она не существует
                }
            }
            *ptr = '/';  // Восстановить путь
        }
    }
    free(subpath);
}
void unarchive(const char *archive_path) {
    FILE *archive = fopen(archive_path, "rb");
    if (!archive) {
        perror("Unable to open archive file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        file_metadata metadata;
        if (fread(&metadata, sizeof(metadata), 1, archive) != 1) {
            if (!feof(archive)) perror("Error reading metadata");
            break;  // Выход из цикла в случае конца файла или ошибки
        }

        char path[1024];
        if (fread(path, sizeof(char), metadata.path_length, archive) != metadata.path_length) {
            perror("Error reading path");
            break;
        }
        path[metadata.path_length] = '\0';  // Null-terminate the path
        create_directories(path);           // Создать все директории для пути

        if (metadata.type == 1) {  // Если элемент является директорией
            mkdir(path, metadata.mode);
        } else {  // Если элемент является файлом
            FILE *file = fopen(path, "wb");
            if (!file) {
                perror("Unable to open file for writing: %s");
                continue;
            }
            char buffer[1024];
            off_t bytes_remaining = metadata.file_size;
            while (bytes_remaining > 0) {
                size_t bytes_to_read = sizeof(buffer) < bytes_remaining ? sizeof(buffer) : bytes_remaining;
                if (fread(buffer, 1, bytes_to_read, archive) != bytes_to_read) {
                    perror("Error reading file content");
                    break;
                }
                fwrite(buffer, 1, bytes_to_read, file);
                bytes_remaining -= (off_t)bytes_to_read;
            }
            fclose(file);
        }

        // Восстановление времени модификации файла
        struct utimbuf new_times;
        new_times.modtime = metadata.mod_time;
        new_times.actime = metadata.mod_time;  // Доступ к файлу совпадает с временем модификации
        utime(path, &new_times);

        // Восстановление прав доступа к файлу
        chmod(path, metadata.mode);
    }

    fclose(archive);
}
