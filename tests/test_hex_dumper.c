#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hex_dumper.h"

// Тест 1: Преобразование байта в hex
void test_byte_to_hex() {
    printf("Test 1: byte_to_hex... ");
    char hex_str[3];
    
    byte_to_hex(0x10, hex_str, 1);
    assert(strcmp(hex_str, "10") == 0);
    
    byte_to_hex(0x0A, hex_str, 0);
    assert(strcmp(hex_str, "0a") == 0);
    
    byte_to_hex(0xFF, hex_str, 1);
    assert(strcmp(hex_str, "FF") == 0);
    
    printf("PASSED\n");
}

// Тест 2: Проверка печатаемых символов
void test_is_printable() {
    printf("Test 2: is_printable... ");
    assert(is_printable('A') == 1);
    assert(is_printable('z') == 1);
    assert(is_printable(' ') == 1);
    assert(is_printable('\t') == 0);
    assert(is_printable(0x00) == 0);
    printf("PASSED\n");
}

// Тест 3: Создание hex-строки
void test_hex_dump_basic() {
    printf("Test 3: hex_dump basic... ");
    // Создаём тестовый файл
    FILE *f = fopen("test_hex.bin", "wb");
    assert(f);
    unsigned char data[] = {0x10, 0x11, 0x12, 0x13, 0xFA, 0xFB, 0xFC, 0xFD};
    fwrite(data, 1, sizeof(data), f);
    fclose(f);
    
    DumpOptions options = {0};
    options.input_file = "test_hex.bin";
    options.offset = 0;
    options.length = 0;
    options.group_size = 1;
    options.count = 16;
    
    int result = hex_dump(&options);
    assert(result == 0);
    
    printf("PASSED\n");
}

int main() {
    printf("Running hex_dumper tests...\n");
    printf("================================\n");
    
    test_byte_to_hex();
    test_is_printable();
    test_hex_dump_basic();
    
    printf("================================\n");
    printf("All tests PASSED!\n");
    
    // Очистка
    remove("test_hex.bin");
    return 0;
}