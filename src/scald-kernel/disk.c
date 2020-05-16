#include <stdint.h>
#include <stdbool.h>
#include <biosInt.h>

void* ReadSector(uint8_t track, bool head, uint8_t sector)
{
    regs16_t regs;
    static char buffer[512]; // one sector buffer
    uint8_t ah = 2; // READ
    uint8_t al = 1; // one sector
    regs.ax = (ah << 8) | (al & 0xFF); // AX: AL AH
    uint8_t ch = track; // TRACK ID
    uint8_t cl = sector; // SECTOR ID
    regs.cx = (ch << 8) | (cl & 0xFF);
    uint8_t dh = head;
    uint8_t dl = 0; // DISK ID: FLOPPY 1
    regs.dx = (dh << 8) | (dl & 0xFF);
    regs.bx = (unsigned short)(&buffer); // Is it right??
    int32(0x13, &regs);
    return &buffer;
}