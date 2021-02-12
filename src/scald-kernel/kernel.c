#include <io.h>
#include <biosInt.h>
#include <tty.h>
#include <serial.h>
#include <string.h>
#include <disk.h>
#include <idt.h>
#include <isr.h>
#include <file.h>
#include <pic.h>
#include <malloc.h>

#define PORT 0x3f8   /* COM1 */

typedef enum {
    SERIAL,
    SCREEN,
    CRITICAL
} stdio_channel;

typedef enum{
    ZERO,
    ERR,
    WRN
} exit_status;

void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void stdwr(stdio_channel c, char* msg){
    switch (c)
    {
    case SERIAL:
        print_serial(msg);
        break;
    case SCREEN:
        write(msg);
        break;
    case CRITICAL:
        print_serial("-----ERR-----\n");
        print_serial(msg);
        write("-----ERR-----\n");
        write(msg);
        break;
    default:
        break;
    }
}
void handle_29(context_t* context){
    logf("[SYSCALL] got a syscall\n");
    static uint32_t fileno;
    fileno = context->ecx;
    logf("ecx: %x\n", fileno);
    if (fileno == 0x5){
        logf("READ SERIAL");
    } 
}

void exec(void* address, uint32_t size){
    logf("[EXEC] executing binary file at address %x, size %d\n", address, size);
    void* exec_addr = (void*)0x00100000;
    memset(exec_addr, 0x0, size);
    memcpy(exec_addr, address, size);
    void (*call)() = (void (*)())exec_addr;
    logf("[EXEC] jumping!\n");
    call();
}

void Kernel(){
    init_serial();
    logf("booting scald!\n");
    FILE serial;
    char* buffer = NULL;
    init_pic();
    t_init();
    init_gdt();
    init_idt();
    FILE floppyb;
    floppyb.fileno = FLOPPYB_FILENO;
    buffer = read(&floppyb, 512);
    register_interrupt_handler(0x29, &handle_29);
    exec(buffer, 512);
    stdwr(SCREEN, "\n returned? or not?\n");
    char* ata = ata_pio_read(0, 1);
    stdwr(SCREEN, "ATA1 done\n");
    ata = ata_pio_read(1, 1);
    stdwr(SCREEN, "ATA2 done\n");
    check_bootsector();
    listfiles();
    char* file = readfile("SPLASH");
    listfiles();
    buffer = read(&floppyb, 512);
    exec(buffer, 512);
    stdwr(SCREEN, "ATA1 done\n");
    stdwr(SCREEN, "ATA2 done\n");
    for(;;){
        char b = read_serial_byte();
        terminal_putchar(b);
        logf("%x", b);
    }
    for (;;);
}