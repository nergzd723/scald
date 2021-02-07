#include <io.h>
#include <malloc.h>
#include <serial.h>
#include <string.h>

#define ATA_IO_BASE 0x1F0

#define ATA_DATA_REGISTER_OFFSET 0

#define ATA_ERROR_REGISTER_OFFSET 1

#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_RDY 0x40
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_ERR 0x01

#define ATA_FEATURES_REGISTER_OFFSET 1
#define ATA_SECTOR_COUNT_REGISTER_OFFSET 2
#define ATA_SECTOR_LBALO_REGISTER_OFFSET 3
#define ATA_CYLINDER_LOW_REGISTER_OFFSET 4
#define ATA_CYLINDER_HIGH_REGISTER_OFFSET 5
#define ATA_HEAD_REGISTER_OFFSET 6
#define ATA_STATUS_REGISTER_OFFSET 7
#define ATA_COMMAND_REGISTER_OFFSET 7

#define ATA_CONTROL_BASE 0x3F6

#define ATA_ALTERNATE_STATUS_REGISTER_OFFSET 0
#define ATA_DEVICE_CONTROL_REGISTER_OFFSET 0
#define ATA_DRIVE_ADDRESS_REGISTER_OFFSET 1

// Produce 400ns latency, usually 4 inbs should work
void ata_400ns(){
    inb(ATA_CONTROL_BASE+ATA_DEVICE_CONTROL_REGISTER_OFFSET);
    inb(ATA_CONTROL_BASE+ATA_DEVICE_CONTROL_REGISTER_OFFSET);
    inb(ATA_CONTROL_BASE+ATA_DEVICE_CONTROL_REGISTER_OFFSET);
    inb(ATA_CONTROL_BASE+ATA_DEVICE_CONTROL_REGISTER_OFFSET);
    return;
}
void ata_wait_busy(){
    while(inb(ATA_IO_BASE+ATA_COMMAND_REGISTER_OFFSET) & ATA_STATUS_BSY); //wait for it to become 0
}

void ata_wait_ready(){
    while(!(inb(ATA_IO_BASE+ATA_COMMAND_REGISTER_OFFSET) & ATA_STATUS_RDY));
}

char* ata_pio_read_sector(uint32_t LBA){
    uint16_t temp = 0;
    char* buffer = malloc(512); // allocate some space
    ata_wait_busy();
    outb(ATA_IO_BASE+ATA_HEAD_REGISTER_OFFSET, 0xE0 | (LBA >> 24) & 0xF);
    outb(ATA_IO_BASE+ATA_SECTOR_COUNT_REGISTER_OFFSET, 1);
    outb(ATA_IO_BASE+ATA_SECTOR_LBALO_REGISTER_OFFSET, (uint8_t)LBA); //lower
    outb(ATA_IO_BASE+ATA_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(LBA >> 8)); //lower(real one lmfao)
    outb(ATA_IO_BASE+ATA_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(LBA >> 16)); //higher
    outb(ATA_IO_BASE+ATA_COMMAND_REGISTER_OFFSET, 0x20); // ATA read
    ata_wait_busy();
    ata_wait_ready();
    for (uint16_t j = 0; j<256; j++){
        temp = inw(ATA_IO_BASE);
        buffer[j*2] = temp & 0xFF;
        buffer[j*2+1] = (temp >> 8) & 0xFF;
    }

    logf("[ATA] PIO read, LBA %x\n", LBA);
    return buffer;
}

void ata_pio_read_sector_direct(uint32_t LBA, char* buffer){
    uint16_t temp = 0;
    ata_wait_busy();
    outb(ATA_IO_BASE+ATA_HEAD_REGISTER_OFFSET, 0xE0 | (LBA >> 24) & 0xF);
    outb(ATA_IO_BASE+ATA_SECTOR_COUNT_REGISTER_OFFSET, 1);
    outb(ATA_IO_BASE+ATA_SECTOR_LBALO_REGISTER_OFFSET, (uint8_t)LBA); //lower
    outb(ATA_IO_BASE+ATA_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(LBA >> 8)); //lower(real one lmfao)
    outb(ATA_IO_BASE+ATA_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(LBA >> 16)); //higher
    outb(ATA_IO_BASE+ATA_COMMAND_REGISTER_OFFSET, 0x20); // ATA read
    ata_wait_busy();
    ata_wait_ready();
    for (uint16_t j = 0; j<256; j++){
        temp = inw(ATA_IO_BASE);
        buffer[j*2] = temp & 0xFF;
        buffer[j*2+1] = (temp >> 8) & 0xFF;
    }

    logf("[ATA] PIO read, LBA %x\n", LBA);
}

char* ata_pio_read(uint32_t LBA, uint32_t sector_count){
    char* buffer = malloc(sector_count* 512);
    for(int i = 0; i < sector_count; i++){
        ata_pio_read_sector_direct(LBA+i, buffer+(i*512));
    }
    return buffer;
}