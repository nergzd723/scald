C_TARGETS =$(shell find src/lib src/scald-kernel -name "*.c")
C_TARGETS_APP =$(shell find src/lib src/scald-application -name "*.c")

all:
	mkdir -p build
	nasm -felf src/scald-loader/loader.asm -o build/ldr.o
	nasm -felf src/scald-kernel/interrupt.asm -o build/interrupt.o
	nasm -felf src/scald-kernel/isr.asm -o build/isr.o
	nasm -felf src/scald-application/application.asm -o build/application-b.bin
	~/i686-elf/bin/i386-unknown-elf-gcc $(C_TARGETS_APP) -T src/scald-application/linker.ld -ffreestanding build/application-b.bin -o build/application.bin -nostdlib -mno-red-zone -nostdlib -Wall -Wextra -O3
	~/i686-elf/bin/i386-unknown-elf-gcc $(C_TARGETS) -T src/linker.ld -ffreestanding build/interrupt.o build/isr.o build/ldr.o -o build/image.elf -nostdlib -mno-red-zone -nostdlib -Wall -Wextra -g -Isrc/scald-kernel/include  -O2
	~/i686-elf/bin/i386-unknown-elf-objcopy --only-keep-debug build/image.elf build/image.sym
	~/i686-elf/bin/i386-unknown-elf-objcopy --strip-debug build/image.elf
	~/i686-elf/bin/i386-unknown-elf-objcopy -O binary build/image.elf build/image.bin
run: all
	qemu-system-i386 -boot d -fda build/image.bin -fdb build/application.bin -serial stdio -hda harddisk.img
debug: all
	qemu-system-i386 -s -S -boot d -fda build/image.bin -fdb build/application.bin -serial stdio -hda harddisk.img &
	gdb --command=gdb.conf 