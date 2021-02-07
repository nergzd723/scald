#include <stddef.h>
#include <io.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
extern int print_serial_hex(int);
#define PORT 0x3f8   /* COM1 */

char* inttostr( int zahl ) ;

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

void logf(const char* format, ...) {
    va_list parameters;
    va_start(parameters, format);
    while (*format != '\0') {
        if (format[0] != '%') {
            write_serial_byte(format[0]);
            format++;
            continue;
        }

        format++;  // skip past '%'

        if (*format == 's') { // string
            format++;
            const char* str = va_arg(parameters, const char*);
            print_serial(str);
        } else if (*format == 'c') { // character
            format++;
            char c = (char) va_arg(parameters, int); // char is promoted to int
            write_serial_byte(c);
        } else if (*format == 'd') { // character
            format++;
            uint32_t num = va_arg(parameters, uint32_t); // char is promoted to int
            print_serial_hex(num);
        } else if (*format == 'x') { // hex number
            format++;
            uint32_t num = va_arg(parameters, uint32_t);
            write_serial_byte('0');
            write_serial_byte('x');
            print_serial_hex(num);
        }
    }

    va_end(parameters);
}