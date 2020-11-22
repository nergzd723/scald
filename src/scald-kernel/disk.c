#include <stdint.h>
#include <stdbool.h>
#include <biosInt.h>
#include <malloc.h>
#include <string.h>
#include <serial.h>
#include <file.h>

char* ReadSector(uint8_t track, bool head, uint8_t sector)
{
    regs16_t regs;
    static char buffer[512];
    //char* buffer = malloc();
    memset(buffer, 0, sizeof(buffer)); // zero it out!
    uint8_t ah = 2; // READ
    uint8_t al = 1; // one sector
    regs.ax = (ah << 8) | (al & 0xFF); // AX: AL AH
    uint8_t ch = track; // TRACK ID
    uint8_t cl = sector; // SECTOR ID
    regs.cx = (ch << 8) | (cl & 0xFF);
    uint8_t dh = head;
    uint8_t dl = 0; // DISK ID: FLOPPYNO
    regs.dx = (dh << 8) | (dl & 0xFF);
    regs.bx = (unsigned short)(&buffer); // Is it right??
    int32(0x13, &regs);
    return buffer;
}

char* ReadSectorB(uint8_t track, bool head, uint8_t sector)
{
    regs16_t regs;
    static char buffer[512];
    //char* buffer = malloc();
    memset(buffer, 0, sizeof(buffer)); // zero it out!
    uint8_t ah = 2; // READ
    uint8_t al = 1; // one sector
    regs.ax = (ah << 8) | (al & 0xFF); // AX: AL AH
    uint8_t ch = track; // TRACK ID
    uint8_t cl = sector; // SECTOR ID
    regs.cx = (ch << 8) | (cl & 0xFF);
    uint8_t dh = head;
    uint8_t dl = 1; // DISK ID: FLOPPY 2
    regs.dx = (dh << 8) | (dl & 0xFF);
    regs.bx = (unsigned short)(&buffer); // Is it right??
    int32(0x13, &regs);
    return buffer;
}

int WriteSector(uint8_t track, bool head, uint8_t sector, char* data)
{
    regs16_t regs;
    uint8_t ah = 3; // WRITE
    uint8_t al = 1; // one sector
    regs.ax = (ah << 8) | (al & 0xFF); // AX: AL AH
    uint8_t ch = track; // TRACK ID
    uint8_t cl = sector; // SECTOR ID
    regs.cx = (ch << 8) | (cl & 0xFF);
    uint8_t dh = head;
    uint8_t dl = 0; // DISK ID: FLOPPY 1
    regs.dx = (dh << 8) | (dl & 0xFF);
    regs.bx = (unsigned short)(data); // Is it right??
    int32(0x13, &regs);
}

char* read(FILE* file, size_t size){
    if (file->fileno == 1 || file->fileno == 0){
        uint16_t size_sectors = (uint16_t)(size/512);
        char* output = malloc(size);
        memset(output, 0, size);
        uint16_t output_pos = 0;
        for (uint16_t i = 0; i<size_sectors; i++){
            logf("reading sector %x", i);
            char* buffer = ReadSector((uint8_t)(i/18), (uint8_t)((i/18) / 80), (i % 18)+1);
            file->position += 512;
            memcpy(output+output_pos, buffer, 512);
            output_pos += 512;
        }
        return output;
    }
}