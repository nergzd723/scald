#include <io.h>
#include <biosInt.h>
#include <tty.h>
#include <serial.h>
#include <string.h>
#include <disk.h>
#include <idt.h>
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
// int32 test
void int32_test()
{
    int y;
    regs16_t regs;
     
    // switch to 320x200x256 graphics mode
    regs.ax = 0x0013;
    int32(0x10, &regs);
     
    // full screen with blue color (1)
    memset((char *)0xA0000, 1, (320*200));
     
    // draw horizontal line from 100,80 to 100,240 in multiple colors
    for(y = 0; y < 200; y++)
        memset((char *)0xA0000 + (y*320+80), y, 160);
     
    // wait for key
    regs.ax = 0x0000;
    int32(0x16, &regs);
     
    // switch to 80x25x16 text mode
    regs.ax = 0x0003;
    int32(0x10, &regs);
}
void Kernel(){
    init_serial();
    init_pic();
    t_init();
    init_idt();
    asm volatile ("sti");
    stdwr(CRITICAL, "LOOP WITHOUT INTERRUPTS\n");
    for (;;);
}