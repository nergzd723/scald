#ifndef FILE_H
#define FILE_H
#include <stddef.h>
#include <stdint.h>
#define FLOPPYA_FILENO 0
#define FLOPPYB_FILENO 1
#define SERIAL_FILENO 2
typedef struct
{
    uint8_t fileno;
    uint32_t position;
    uint32_t size; 
} FILE;
char* read(FILE* file, size_t size);
void seek(FILE* file, size_t pos);
#endif