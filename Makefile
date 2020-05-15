C_TARGETS =$(shell find . -name "*.c")
all:
	mkdir -p build
	nasm -felf src/scald-loader/loader.asm -o build/ldr.o
	~/i686-elf/bin/i386-unknown-elf-gcc $(C_TARGETS) build/ldr.o -o build/image.bin -nostdlib -ffreestanding -mno-red-zone -nostdlib -Wall -Wextra -T src/linker.ld -Isrc/scald-kernel/include
run: all
	qemu-system-i386 -fda build/image.bin -serial stdio