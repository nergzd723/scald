#include <stdint.h>
#include <stdbool.h>
void* ReadSector(uint8_t track, bool head, uint8_t sector);
int WriteSector(uint8_t track, bool head, uint8_t sector, char* data);
void* ReadTrack(uint8_t track, bool head);
void ReadTrackAt(uint8_t track, bool head, void* at);