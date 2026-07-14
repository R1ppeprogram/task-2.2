#include "hex_dumper.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Преобразование байта в hex-строку
void byte_to_hex(unsigned char byte, char *hex_str, int uppercase) {
    const char *hex_chars = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    hex_str[0] = hex_chars[byte >> 4];
    hex_str[1] = hex_chars[byte & 0x0F];
    hex_str[2] = '\0';
}

// Проверка, является ли байт печатаемым символом
int is_printable(unsigned char byte) {
    return byte >= 32 && byte <= 126;
}

// Вывод одной строки дампа
void print_dump_line(FILE *file, size_t offset, const unsigned char *data, 
                     size_t data_size, const DumpOptions *options) {
    // Вывод смещения
    printf("%08lx ", (unsigned long)offset);
    
    // Вывод hex-представления
    for (size_t i = 0; i < data_size; i++) {
        char hex_str[3];
        byte_to_hex(data[i], hex_str, 1);
        printf("%s", hex_str);
        
        // Добавляем пробел между байтами
        if (i < data_size - 1) {
            printf(" ");
        }
    }
    
    // Дополняем пробелами до фиксированной ширины
    size_t total_hex = data_size * 3 - 1; // 3 символа на байт (2 hex + пробел)
    size_t max_hex = options->count * 3 - 1;
    for (size_t i = total_hex; i < max_hex; i++) {
        printf(" ");
    }
    
    // Если группа = 1 байт, выводим символьное представление
    if (options->group_size == 1) {
        printf("  |");
        for (size_t i = 0; i < data_size; i++) {
            printf("%c", is_printable(data[i]) ? data[i] : '.');
        }
        printf("|");
    }
    
    printf("\n");
}

// Основная функция для создания hex-дампа
int hex_dump(const DumpOptions *options) {
    FILE *file = NULL;
    unsigned char *buffer = NULL;
    size_t bytes_read;
    size_t total_read = 0;
    size_t bytes_to_read;
    
    // Открываем файл
    if (options->input_file) {
        file = fopen(options->input_file, "rb");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", options->input_file);
            return -1;
        }
        
        // Пропускаем offset байт
        if (fseek(file, options->offset, SEEK_SET) != 0) {
            fprintf(stderr, "Error: Cannot seek to offset %zu\n", options->offset);
            fclose(file);
            return -1;
        }
    } else if (options->directory) {
        // TODO: обработать директорию
        fprintf(stderr, "Directory mode not implemented yet\n");
        return -1;
    } else {
        fprintf(stderr, "Error: No input file or directory specified\n");
        return -1;
    }
    
    // Определяем, сколько байт читать
    bytes_to_read = options->length;
    if (bytes_to_read == 0) {
        // Если длина не задана, читаем до конца файла
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, options->offset, SEEK_SET);
        bytes_to_read = file_size - options->offset;
    }
    
    // Выделяем буфер для чтения
    buffer = (unsigned char*)malloc(options->count * options->group_size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }
    
    // Читаем и выводим данные
    size_t line_count = 0;
    while (total_read < bytes_to_read) {
        size_t remaining = bytes_to_read - total_read;
        size_t chunk_size = (remaining < options->count * options->group_size) ? 
                            remaining : options->count * options->group_size;
        
        bytes_read = fread(buffer, 1, chunk_size, file);
        if (bytes_read == 0) break;
        
        // Выводим каждую строку
        for (size_t i = 0; i < bytes_read; i += options->group_size) {
            size_t line_size = (bytes_read - i < options->group_size) ? 
                               bytes_read - i : options->group_size;
            size_t current_offset = options->offset + total_read + i;
            
            print_dump_line(file, current_offset, buffer + i, line_size, options);
            line_count++;
        }
        
        total_read += bytes_read;
    }
    
    free(buffer);
    fclose(file);
    return 0;
}