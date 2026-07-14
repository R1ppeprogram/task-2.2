#include <stdio.h>
#include <stdlib.h>
#include "getopt.h"
#include <string.h>
#include "hex_dumper.h"

// Функция для вывода справки
void print_usage(const char *program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -i file_name    Input file name\n");
    printf("  -o offset       Offset from start (default: 0)\n");
    printf("  -l size         Number of bytes to read (default: all)\n");
    printf("  -g size         Group size in bytes (default: 1)\n");
    printf("  -n count        Number of groups per line (default: 16)\n");
    printf("  -d dir          Directory to process\n");
    printf("  -h              Show this help\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -i file.txt -o 10 -l 32 -g 4 -n 4\n", program_name);
    printf("  %s -i file.txt -g 1 -n 16\n", program_name);
}

int main(int argc, char *argv[]) {
    DumpOptions options;
    int opt;
    
    // Инициализация структуры с параметрами по умолчанию
    memset(&options, 0, sizeof(DumpOptions));
    options.offset = 0;
    options.length = 0;
    options.group_size = 1;
    options.count = 16;
    options.input_file = NULL;
    options.directory = NULL;
    
    // Разбор аргументов командной строки с помощью getopt
    while ((opt = getopt(argc, argv, "i:o:l:g:n:d:h")) != -1) {
        switch (opt) {
            case 'i':
                options.input_file = optarg;
                break;
            case 'o':
                options.offset = strtoul(optarg, NULL, 0);
                break;
            case 'l':
                options.length = strtoul(optarg, NULL, 0);
                break;
            case 'g':
                options.group_size = strtoul(optarg, NULL, 0);
                if (options.group_size == 0) {
                    fprintf(stderr, "Error: Group size must be > 0\n");
                    return 1;
                }
                break;
            case 'n':
                options.count = strtoul(optarg, NULL, 0);
                if (options.count == 0) {
                    fprintf(stderr, "Error: Count must be > 0\n");
                    return 1;
                }
                break;
            case 'd':
                options.directory = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                fprintf(stderr, "Unknown option: %c\n", optopt);
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Проверка: должен быть задан файл или директория
    if (!options.input_file && !options.directory) {
        fprintf(stderr, "Error: Input file or directory must be specified\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Вызов основной функции
    int result = hex_dump(&options);
    
    return result;
}