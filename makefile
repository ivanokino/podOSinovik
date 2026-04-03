EFI_FILE = BOOTX64.EFI
SRC = starter.c
KERNEL_SRC = kernel.c
KERNEL_BIN = kernel.bin
IMG_DIR = image
EFI_DIR = $(IMG_DIR)/EFI/BOOT

CC = clang
LD = ld.lld

CFLAGS_EFI = \
    -target x86_64-unknown-windows \
    -ffreestanding \
    -fno-stack-protector \
    -fshort-wchar \
    -mno-red-zone \
    -nostdlib \
    -fuse-ld=lld \
    -I. \
    -Wl,/subsystem:efi_application \
    -Wl,/entry:efi_main

CFLAGS_KERNEL = \
    -ffreestanding \
    -fno-stack-protector \
    -mno-red-zone \
    -nostdlib \
    -O2 \
    -I.

LDFLAGS_KERNEL = \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T kernel.ld

.PHONY: all copy run debug clean clean-img

all: $(EFI_FILE) $(KERNEL_BIN) copy


$(EFI_FILE): $(SRC) structs.h types.h
	$(CC) $(CFLAGS_EFI) $(SRC) -o $(EFI_FILE)
	@echo " EFI loader compiled: $(EFI_FILE)"


$(KERNEL_BIN): $(KERNEL_SRC) kernel.ld structs.h types.h
	$(CC) $(CFLAGS_KERNEL) -c $(KERNEL_SRC) -o kernel.o
	$(LD) $(LDFLAGS_KERNEL) --oformat binary kernel.o -o $(KERNEL_BIN)
	@echo " Kernel compiled: $(KERNEL_BIN)"
	@echo " Kernel size: $$(stat -f %z $(KERNEL_BIN) 2>/dev/null || stat -c %s $(KERNEL_BIN)) bytes"


copy:
	@mkdir -p $(EFI_DIR)
	@cp $(EFI_FILE) $(EFI_DIR)/
	@cp $(KERNEL_BIN) $(IMG_DIR)/
	@echo " Files copied to $(IMG_DIR):"
	@ls -lh $(EFI_DIR)/ 2>/dev/null || echo "  EFI directory: no files"
	@ls -lh $(IMG_DIR)/ 2>/dev/null || echo "  Image directory: no files"


run: all
	@echo "Starting QEMU..."
	qemu-system-x86_64 \
		-drive format=raw,file=fat:rw:$(IMG_DIR) \
		-bios /usr/share/edk2/ovmf/OVMF_CODE.fd \
		-monitor stdio \
		-serial file:serial.log \
		-m 2G


debug: all
	@echo " Starting QEMU with GDB debugger (port 1234)..."
	qemu-system-x86_64 \
		-drive format=raw,file=fat:rw:$(IMG_DIR) \
		-bios /usr/share/edk2/ovmf/OVMF_CODE.fd \
		-monitor stdio \
		-serial file:serial.log \
		-s -S \
		-m 2G

clean:
	@rm -rf $(EFI_FILE) kernel.o $(KERNEL_BIN) $(IMG_DIR) serial.log
	@echo " Cleaned all files"


clean-img:
	@rm -rf $(IMG_DIR)
	@echo " Cleaned image directory"