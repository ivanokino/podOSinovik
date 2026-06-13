#===========================================
EFI_FLAGS = -target x86_64-unknown-windows -ffreestanding -fno-stack-protector -fshort-wchar -mno-red-zone -nostdlib -Isrc -Wl,/subsystem:efi_application -Wl,/entry:efi_main -fuse-ld=lld

KERNEL_FLAGS = -ffreestanding -fno-stack-protector -mno-red-zone -nostdlib -O2 -mgeneral-regs-only -Isrc

LD_FLAGS = -nostdlib -static -T src/kernel/kernel.ld --oformat binary

#===========================================
all:
	mkdir -p build
	clang $(EFI_FLAGS) src/bootloader/starter.c -o BOOTX64.EFI
	nasm -f elf64 src/bootloader/start.asm -o build/start.o
	nasm -f elf64 src/cpu/interrupt.asm -o build/interrupt.o
	clang $(KERNEL_FLAGS) -c src/kernel/kernel.c -o build/kernel.o
	clang $(KERNEL_FLAGS) -c src/drivers/screen.c -o build/screen.o
	clang $(KERNEL_FLAGS) -c src/drivers/ports.c -o build/ports.o
	clang $(KERNEL_FLAGS) -c src/drivers/keyboard.c -o build/keyboard.o
	clang $(KERNEL_FLAGS) -c src/drivers/APIC.c -o build/APIC.o
	clang $(KERNEL_FLAGS) -c src/drivers/shell.c -o build/shell.o
	clang $(KERNEL_FLAGS) -c src/cpu/idt.c -o build/idt.o
	clang $(KERNEL_FLAGS) -c src/cpu/isr_irq.c -o build/isr_irq.o
	clang $(KERNEL_FLAGS) -c src/utils/string.c -o build/string.o
	clang $(KERNEL_FLAGS) -c src/drivers/memory.c -o build/memory.o
	ld.lld $(LD_FLAGS) build/start.o build/interrupt.o  build/kernel.o build/screen.o build/memory.o build/ports.o build/keyboard.o build/APIC.o build/shell.o build/idt.o build/isr_irq.o build/string.o -o kernel.bin

	mkdir -p image/EFI/BOOT
	cp BOOTX64.EFI image/EFI/BOOT/
	cp kernel.bin image/

run:
	mkdir -p build
	clang $(EFI_FLAGS) src/bootloader/starter.c -o BOOTX64.EFI
	nasm -f elf64 src/bootloader/start.asm -o build/start.o
	nasm -f elf64 src/cpu/interrupt.asm -o build/interrupt.o
	clang $(KERNEL_FLAGS) -c src/kernel/kernel.c -o build/kernel.o
	clang $(KERNEL_FLAGS) -c src/drivers/screen.c -o build/screen.o
	clang $(KERNEL_FLAGS) -c src/drivers/ports.c -o build/ports.o
	clang $(KERNEL_FLAGS) -c src/drivers/keyboard.c -o build/keyboard.o
	clang $(KERNEL_FLAGS) -c src/drivers/APIC.c -o build/APIC.o
	clang $(KERNEL_FLAGS) -c src/drivers/shell.c -o build/shell.o
	clang $(KERNEL_FLAGS) -c src/cpu/idt.c -o build/idt.o
	clang $(KERNEL_FLAGS) -c src/cpu/isr_irq.c -o build/isr_irq.o
	clang $(KERNEL_FLAGS) -c src/utils/string.c -o build/string.o
	clang $(KERNEL_FLAGS) -c src/drivers/memory.c -o build/memory.o
	ld.lld $(LD_FLAGS) build/start.o build/interrupt.o  build/kernel.o build/screen.o build/memory.o build/ports.o build/keyboard.o build/APIC.o build/shell.o build/idt.o build/isr_irq.o build/string.o -o kernel.bin
	mkdir -p image/EFI/BOOT
	cp BOOTX64.EFI image/EFI/BOOT/
	cp kernel.bin image/
	qemu-system-x86_64 -drive format=raw,file=fat:rw:image -bios /usr/share/edk2/ovmf/OVMF_CODE.fd -monitor stdio -m 2G 

debug:
	qemu-system-x86_64 -drive format=raw,file=fat:rw:image -bios /usr/share/edk2/ovmf/OVMF_CODE.fd -s -S -monitor stdio -m 2G

clean:
	rm -rf build BOOTX64.EFI kernel.bin image serial.log