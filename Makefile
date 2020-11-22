C_TARGETS =$(shell find . -name "*.c")
all:
	mkdir -p build
	nasm -felf src/scald-loader/loader.asm -o build/ldr.o
	nasm -felf src/scald-kernel/interrupt.asm -o build/interrupt.o
	nasm -felf src/scald-kernel/isr.asm -o build/isr.o
	nasm -fbin src/scald-application/application.asm -o build/application.bin
	~/i686-elf/bin/i386-unknown-elf-gcc $(C_TARGETS) -T src/linker.ld -ffreestanding -g build/interrupt.o build/isr.o build/ldr.o -o build/image.bin -nostdlib -mno-red-zone -nostdlib -Wall -Wextra -Isrc/scald-kernel/include -Os
run: all
	qemu-system-i386 -fda build/image.bin -fdb build/application.bin -serial stdio
