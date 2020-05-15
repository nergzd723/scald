#include <stddef.h>
#include <io.h>
#include <string.h>
#define PORT 0x3f8   /* COM1 */

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial_byte(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT,a);
}
void print_serial(const char *s) {
    for (size_t i = 0; i < strlen(s); i++) {
        write_serial_byte(s[i]);
    }
}