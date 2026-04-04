#vibecoded




SRC_DIR = src
BUILD_DIR = build
IMG_DIR = image
EFI_DIR = $(IMG_DIR)/EFI/BOOT

# ============================================
# Файлы
# ============================================
EFI_FILE = BOOTX64.EFI
KERNEL_BIN = kernel.bin

SRC = $(SRC_DIR)/starter.c
KERNEL_ASM = $(SRC_DIR)/start.asm
KERNEL_C = $(SRC_DIR)/kernel.c $(SRC_DIR)/screen.c
KERNEL_LD = $(SRC_DIR)/kernel.ld
HEADERS = $(SRC_DIR)/structs.h $(SRC_DIR)/types.h $(SRC_DIR)/screen.h $(SRC_DIR)/font.h $(SRC_DIR)/abi.h

KERNEL_OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(KERNEL_C:.c=.o)))
START_OBJ = $(BUILD_DIR)/start.o

# ============================================
# Компиляторы
# ============================================
CC = clang
LD = ld.lld
ASM = nasm

# ============================================
# Флаги для EFI загрузчика (Windows ABI)
# ============================================
CFLAGS_EFI = \
    -target x86_64-unknown-windows \
    -ffreestanding \
    -fno-stack-protector \
    -fshort-wchar \
    -mno-red-zone \
    -nostdlib \
    -fuse-ld=lld \
    -I$(SRC_DIR) \
    -Wl,/subsystem:efi_application \
    -Wl,/entry:efi_main

# ============================================
# Флаги для ядра (System V ABI + атрибуты ms_abi)
# ============================================
CFLAGS_KERNEL = \
    -ffreestanding \
    -fno-stack-protector \
    -mno-red-zone \
    -nostdlib \
    -O2 \
    -I$(SRC_DIR)

LDFLAGS_KERNEL = \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T $(KERNEL_LD)

# ============================================
# Цели
# ============================================
.PHONY: all copy run debug clean clean-img

all: $(BUILD_DIR) $(EFI_FILE) $(KERNEL_BIN) copy

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(EFI_FILE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS_EFI) $(SRC) -o $(EFI_FILE)
	@echo "✅ EFI loader compiled: $(EFI_FILE)"

$(START_OBJ): $(KERNEL_ASM)
	$(ASM) -f elf64 $(KERNEL_ASM) -o $(START_OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

$(KERNEL_BIN): $(START_OBJ) $(KERNEL_OBJS) $(KERNEL_LD)
	$(LD) $(LDFLAGS_KERNEL) --oformat binary $(START_OBJ) $(KERNEL_OBJS) -o $(KERNEL_BIN)
	@echo "✅ Kernel compiled: $(KERNEL_BIN)"
	@echo "📦 Kernel size: $$(stat -f %z $(KERNEL_BIN) 2>/dev/null || stat -c %s $(KERNEL_BIN)) bytes"

copy:
	@mkdir -p $(EFI_DIR)
	@cp $(EFI_FILE) $(EFI_DIR)/
	@cp $(KERNEL_BIN) $(IMG_DIR)/
	@echo "📁 Files copied to $(IMG_DIR)"

run: all
	@echo "🚀 Starting QEMU..."
	qemu-system-x86_64 \
		-drive format=raw,file=fat:rw:$(IMG_DIR) \
		-bios /usr/share/edk2/ovmf/OVMF_CODE.fd \
		-monitor stdio \
		-serial file:serial.log \
		-m 2G

debug: all
	@echo "🐛 Starting QEMU with GDB..."
	qemu-system-x86_64 \
		-drive format=raw,file=fat:rw:$(IMG_DIR) \
		-bios /usr/share/edk2/ovmf/OVMF_CODE.fd \
		-monitor stdio \
		-serial file:serial.log \
		-s -S \
		-m 2G

clean:
	@rm -rf $(EFI_FILE) $(KERNEL_BIN) $(BUILD_DIR) $(IMG_DIR) serial.log
	@echo "🧹 Cleaned all files"

clean-img:
	@rm -rf $(IMG_DIR)
	@echo "🧹 Cleaned image directory"