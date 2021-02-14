#include <stdint.h>
#include <serial.h>
#include <io.h>
#include <isr.h>
#define PIT_FREQUENCY 1193180
// PIT: the Programmable Interrupt Timer
// callbacks to IRQ0
// frequency of PIT is 1193180 Hz
// PIT register for cahnnel 0 is 0x40

uint32_t tick_count;

void pit_callback(context_t* context){
    tick_count++;
}

uint64_t read_pit(){
    return tick_count;
}

void init_pit(uint32_t frequency){
    logf("[PIT] initializing timer at %d frequency\n", frequency);
    register_interrupt_handler(32, &pit_callback);
    outb(0x43, 0x36); // send the ID of a channel 0 to command register
    outb(0x40, (uint8_t)((PIT_FREQUENCY/frequency) & 0xFF));
    outb(0x40, (uint8_t)(((PIT_FREQUENCY/frequency) >> 8) & 0xFF));
    asm volatile ("sti");
}