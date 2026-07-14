#ifndef HEX_DUMPER_H
#define HEX_DUMPER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// Структура для хранения параметров вывода
typedef struct {
    char *input_file;    // имя входного файла (-i)
    size_t offset;       // смещение от начала файла (-o)
    size_t length;       // количество выводимых байт (-l)
    size_t group_size;   // размер кусочка в байтах (-g)
    size_t count;        // количество кусочков в строке (-n)
    char *directory;     // директория для вывода (-d)
} DumpOptions;

// Основная функция для создания hex-дампа
int hex_dump(const DumpOptions *options);

// Функция для преобразования байта в hex-строку
void byte_to_hex(unsigned char byte, char *hex_str, int uppercase);

// Функция для вывода отдельной строки дампа
void print_dump_line(FILE *file, size_t offset, const unsigned char *data, 
                     size_t data_size, const DumpOptions *options);

#endif // HEX_DUMPER_H