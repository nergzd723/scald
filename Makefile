C_TARGETS =$(shell find . -name "*.c")
all:
	mkdir -p build
	nasm -felf src/scald-loader/loader.asm -o build/ldr.o
	nasm -felf src/scald-kernel/interrupt.asm -o build/interrupt.o
	~/i686-elf/bin/i386-unknown-elf-gcc $(C_TARGETS) build/ldr.o build/interrupt.o -o build/image.bin -nostdlib -ffreestanding -mno-red-zone -nostdlib -Wall -Wextra -T src/linker.ld -Isrc/scald-kernel/include -Os
run: all
	qemu-system-i386 -fda build/image.bin -serial stdio