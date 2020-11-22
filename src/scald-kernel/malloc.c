#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MEMORY_START 0x00200000 // start at 2MiB

uint32_t current_position = MEMORY_START;

char* malloc(size_t size){
    memset((char*)current_position, 0x0, size);
    current_position += size;
    return (char*)current_position;
}