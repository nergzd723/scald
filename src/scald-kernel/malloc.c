#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <serial.h>

#define MEMORY_START 0x00200000 // start at 2MiB

uint32_t current_position = MEMORY_START;

char* malloc(size_t size){
    logf("[MALLOC] allocating static memory buffer of %x bytes of size at %x\n", size, current_position);
    memset((char*)current_position, 0x0, size);
    current_position += size;
    return (char*)current_position;
}