#ifndef FILE_H
#define FILE_H
#include <stdint.h>
#define FLOPPYA_FILENO 0
#define FLOPPYB_FILENO 1
typedef struct
{
    uint8_t fileno;
    uint8_t position;
    uint8_t size; 
} FILE;
#endif