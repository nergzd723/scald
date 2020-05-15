#include <io.h>
#include <tty.h>
#include <serial.h>
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
void Kernel(){
    init_serial();
    t_init();
    stdwr(CRITICAL, "LOOP WITHOUT INTERRUPTS\n");
    for (;;);
}