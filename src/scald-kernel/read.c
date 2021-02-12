#include <stdint.h>
#include <file.h>
#include <stddef.h>
#include <serial.h>
#include <disk.h>

char* read(FILE* file, size_t size){
    logf("[READ] reading %x bytes from fileno %d\n", size, file->fileno);
    switch (file->fileno)
    {
    case FLOPPYA_FILENO:
    case FLOPPYB_FILENO:
        return read_disk(file, size);
    case SERIAL_FILENO:
        return read_serial(file, size);
    default:
        return NULL;

    }

}

void seek(FILE* file, size_t pos){
    file->position += pos;
}