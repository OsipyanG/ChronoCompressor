#ifndef LAB_1_ARCHIVE_H
#define LAB_1_ARCHIVE_H

#include <sys/types.h>

void archive(const char *path);

typedef struct {
    unsigned int path_length;  // Длина пути к файлу
    off_t file_size;           // Размер файла
    mode_t mode;               // Права доступа к файлу
    unsigned char type;        // Тип элемента: 0 - файл, 1 - директория
    time_t mod_time;           // Время последнего изменения файла
    // Далее следует путь к файлу
} file_metadata;

#endif  // LAB_1_ARCHIVE_H
