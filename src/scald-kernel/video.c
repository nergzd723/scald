#include <biosInt.h>
#include <stdint.h>
#define FB_ADDR 0xA0000

enum color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void switch_video_mode(){
    regs16_t regs;
    regs.ax = 0x13;
    int32(0x10, &regs);
}
void fill(){
    uint16_t* buffer = (uint16_t*) FB_ADDR;
    memset((char *)0xA0000, VGA_COLOR_LIGHT_GREEN, (320*200));
}

void draw_triangle(uint16_t base){
    uint8_t* buffer = (uint8_t*) FB_ADDR;
    uint32_t pointer = 0;
    for (int i = 0; i < 160; i++)
    {
        for(int b = 0; b <= i; b++){
            buffer[pointer+b] = VGA_COLOR_LIGHT_BLUE;
        }
        pointer += 320;
    }
}