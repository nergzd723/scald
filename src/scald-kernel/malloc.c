#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <serial.h>

#define MEMORY_START 0x00200000 // start at 2MiB

uint32_t remainder = 0;

char* malloc(size_t size){
    logf("[MALLOC] allocating static memory buffer of %x bytes of size at %x\n", size, MEMORY_START+remainder);
    remainder = remainder + size;
    memset((char*)remainder+MEMORY_START, 0x0, size);
    return (char*)remainder+MEMORY_START;
}