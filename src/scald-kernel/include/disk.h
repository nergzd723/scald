#include <stdint.h>
#include <stdbool.h>
#include <file.h>
#include <stddef.h>
void* ReadSector(uint8_t track, bool head, uint8_t sector, uint8_t floppy_id);
void* ReadSectorB(uint8_t track, bool head, uint8_t sector);
int WriteSector(uint8_t track, bool head, uint8_t sector, char* data);
void* ReadTrack(uint8_t track, bool head);
void ReadTrackAt(uint8_t track, bool head, void* at);
char* read(FILE* file, size_t size);