#include <stdint.h>
#include <serial.h>
#include <disk.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    char jmp_over[3]; // 3 bytes of JMP SHORT 3C
    char oem_identifier[8]; // 8 bytes of OEM identifier
    uint16_t bytes_per_sector; // 2 bytes of bytes per sector
    uint8_t sectors_per_cluster; // a byte of sectors per cluster
    uint16_t reserved_sectors; // 2 bytes of reserved sectors count
    uint8_t fat_count; // count of FAT's on drive
    uint16_t directory_entries_count; // count of directory entries
    uint16_t total_sector_count; // total sector count, if 0, actual number is in large sector count
    uint8_t media_descriptor; // media descriptor type
    uint16_t sectors_per_fat; // specific for fat12
    uint16_t sectors_per_track; // sectors per track
    uint16_t head_count; // number of heads
    uint32_t hidden_sector_number; // number of hidden sectors
    uint32_t large_sector_count; // large sector count if doesn't fit into total sector count
}__attribute__((packed)) fat_bpb;

typedef struct
{
    uint32_t sectors_per_fat; // size of FAT in sectors
    uint16_t flags; // flags?
    uint16_t fat_version; // FAT version number
    uint32_t rootdir_cluster; // cluster number of rootdir
    uint16_t fsinfo_sector; // sector number of fsinfo
    uint16_t backup_boot_sector; // sector number of backup boot sector
    char reserved_bytes[12]; // 12 reserved bytes
    uint8_t drive_number; // drive number
    uint8_t winnt_flags; // Windows NT flags
    uint8_t signature; // signature, must be 0x28 or 0x29
    uint32_t serial_number; // serial number of volume
    char volume_label[11]; // volume label, padded with spaces
    char system_identifier[8]; // system identifies, always FAT32  
    char boot_code[420];
}__attribute__((packed)) fat32_ebr;

typedef struct
{
    uint8_t drive_number;
    uint8_t winnt_flags;
    uint8_t signature;
    uint32_t serial_number;
    char volume_label[11];
    char system_identifier[8];
    char boot_code[448];
}__attribute__((packed)) fat16_ebr;


typedef struct{
    fat_bpb bpb;
    fat16_ebr ebr;
    uint16_t bootable_signature; // should be 0xAA55
}__attribute__((packed)) fat32_bootsector;

typedef struct
{
    char file_name[11];
    uint8_t attributes;
    uint8_t NT_reserved;
    uint8_t creation_time_precise;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_accessed_date;
    uint16_t cluster_high;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t cluster_low;
    uint32_t size;
}__attribute__((packed)) fat_entry;

typedef struct
{
    uint8_t order;
    char first_5_chars[10];
    uint8_t isLong;
    uint8_t checksum;
    char next_6_chars[12];
    uint16_t zero;
    char last_2_chars[5];
    fat_entry entry;
}__attribute__((packed)) longfat_entry;

char* getfile(uint16_t sectors_per_fat, uint16_t cluster, uint16_t first_sector_cluster, uint32_t size){
    logf("[FAT32] sectors per fat: %x, first cluster: %x, cluster: %x, destination: %x\n", sectors_per_fat, first_sector_cluster, cluster, first_sector_cluster+(4*cluster)+sectors_per_fat + 0x22);
    char* buffer = ata_pio_read(first_sector_cluster+(4*cluster)+sectors_per_fat + 0x12, 1);
    return buffer;
}

void process_entry(longfat_entry* entry, uint16_t sectors_per_fat){
    if (entry->isLong == 0x0F){
        logf("[FAT] detected a valid long file\n");
    }
    logf("the name is %s\n", entry->entry.file_name);
    logf("the cluster is %x\n", entry->entry.cluster_low);
    char* contents = getfile(sectors_per_fat, entry->entry.cluster_low, 0xD, entry->entry.size);
    logf("the contents of the file are: %s", contents);
}

void check_bootsector(){
    char* bootsector_raw = ata_pio_read(0,1);
    bootsector_raw = ata_pio_read(0, 1);
    fat32_bootsector* bootsector = (fat32_bootsector*)bootsector_raw;
    logf("[FAT32] searching for fat32 file system on the ATA drive 1! \n");
    if (bootsector->bootable_signature == 0xAA55){
        logf("[FAT32] sectors per cluster: %d\n", bootsector->bpb.sectors_per_cluster);
        logf("[FAT32] reserved sectors: %d\n", bootsector->bpb.reserved_sectors);
        logf("[FAT32] sectors per fat: %d\n", bootsector->bpb.sectors_per_fat);
        logf("[FAT32] found a boot sector!\n");
    }
    else
    {
        logf("[FAT32] there is no fat32 filesystem on the drive, exiting... \n");
        return;
    }
    uint32_t sectors_per_fat = bootsector->bpb.sectors_per_fat;
    uint32_t root_dir_sectors = ((bootsector->bpb.directory_entries_count * 32) + (bootsector->bpb.bytes_per_sector - 1)) / bootsector->bpb.bytes_per_sector;
    uint32_t first_data_sector = bootsector->bpb.reserved_sectors + (bootsector->bpb.fat_count * bootsector->bpb.sectors_per_fat) + root_dir_sectors;
    uint32_t first_sector_cluster = 0xD;
    char* rootdir_sector = ata_pio_read(first_sector_cluster, 1);
    for (int i = 0; i<512; i+=32){
        char* b = (char*)rootdir_sector+i;
        if (!b[0]){
            logf("[FAT] end of directory\n");
            break;
        }
        if (b[0] == 0xE5){
            logf("b is unused\n");
            continue;
        }
        if(b[11] == 0x0F){
            logf("[FAT] long file detected\n");
        }
        process_entry((longfat_entry*)b, sectors_per_fat);
        i+=32;
    }
exit:
    return bootsector;
}