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
void handle_29(){
    logf("SYSCALL\n");
}
void Kernel(){
    init_serial();
    init_pic();
    t_init();
    init_gdt();
    stdwr(SCREEN, "LOOP WITHOUT INTERRUPTS\n");

    char* buffer = ReadSector(0,0,1);
    logf("\n start sector");
    for (int i = 0; i < 512; i++){
        logf("%x ", buffer[i]);
    }
    logf("\n end sector");
    init_idt();
    buffer = ReadSectorB(0,0,1);
    stdwr(SCREEN, "\n start sector");
    stdwr(SCREEN, "\n end sector");
    register_interrupt_handler(0x29, &handle_29);
    memcpy((void*) 0x00100000, buffer, 512);
    stdwr(SCREEN, "\n put payload to 0x0010000, jumping\n");
    void (*foo)(void) = (void (*)())0x00100000;
    foo();
    stdwr(SCREEN, "\n returned? or not?\n");
    init_keyboard();
    FILE floppyb;
    floppyb.fileno = 1;
    buffer = read(&floppyb, 2048);
    logf("\n start sector");
    for (int i = 0; i < 2048; i++){
        logf("%x ", buffer[i]);
    }
    logf("\n end sector");
    for (;;);
}