# ============================================
# Директории
# ============================================
SRC_DIR = src
BUILD_DIR = build
IMG_DIR = image
EFI_DIR = $(IMG_DIR)/EFI/BOOT

# ============================================
# Поддиректории исходников
# ============================================
CPU_DIR = $(SRC_DIR)/cpu
DRIVERS_DIR = $(SRC_DIR)/drivers
UTILS_DIR = $(SRC_DIR)/utils
EFI_DIR_SRC = $(SRC_DIR)/efi
KERNEL_DIR = $(SRC_DIR)/kernel
BOOTLOADER_DIR = $(SRC_DIR)/bootloader

# ============================================
# Файлы
# ============================================
EFI_FILE = BOOTX64.EFI
KERNEL_BIN = kernel.bin

# Загрузчик
SRC = $(BOOTLOADER_DIR)/starter.c
START_ASM = $(BOOTLOADER_DIR)/start.asm

# Ядро (ассемблер + C файлы)
INTERRUPT_ASM = $(CPU_DIR)/interrupt.asm

KERNEL_C = \
    $(KERNEL_DIR)/kernel.c \
    $(DRIVERS_DIR)/screen.c \
    $(DRIVERS_DIR)/ports.c \
    $(DRIVERS_DIR)/keyboard.c \
    $(DRIVERS_DIR)/APIC.c \
    $(CPU_DIR)/idt.c \
    $(CPU_DIR)/isr_irq.c

KERNEL_LD = $(KERNEL_DIR)/kernel.ld

# Заголовочные файлы (все .h файлы)
HEADERS = \
    $(shell find $(SRC_DIR) -name '*.h' 2>/dev/null)

# Объектные файлы
KERNEL_OBJS = \
    $(BUILD_DIR)/kernel.o \
    $(BUILD_DIR)/screen.o \
    $(BUILD_DIR)/ports.o \
    $(BUILD_DIR)/keyboard.o \
    $(BUILD_DIR)/APIC.o \
    $(BUILD_DIR)/idt.o \
    $(BUILD_DIR)/isr_irq.o

START_OBJ = $(BUILD_DIR)/start.o
INTERRUPT_OBJ = $(BUILD_DIR)/interrupt.o

# ============================================
# Компиляторы
# ============================================
CC = clang
LD = ld.lld
ASM = nasm

# ============================================
# Флаги для EFI загрузчика
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
    -I$(UTILS_DIR) \
    -I$(DRIVERS_DIR) \
    -I$(CPU_DIR) \
    -I$(BOOTLOADER_DIR) \
    -I$(EFI_DIR_SRC) \
    -I$(EFI_DIR_SRC)/protocol \
    -Wl,/subsystem:efi_application \
    -Wl,/entry:efi_main

# ============================================
# Флаги для ядра
# ============================================
CFLAGS_KERNEL = \
    -ffreestanding \
    -fno-stack-protector \
    -mno-red-zone \
    -fno-stack-check \
    -mgeneral-regs-only \
    -nostdlib \
    -O2 \
    -I$(SRC_DIR) \
    -I$(UTILS_DIR) \
    -I$(DRIVERS_DIR) \
    -I$(CPU_DIR) \
    -I$(BOOTLOADER_DIR)

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

# ============================================
# Сборка EFI загрузчика
# ============================================
$(EFI_FILE): $(SRC) $(START_ASM) $(HEADERS)
	$(CC) $(CFLAGS_EFI) $(SRC) -o $(EFI_FILE)
	@echo "✅ EFI loader compiled: $(EFI_FILE)"

# ============================================
# Сборка ядра
# ============================================
$(START_OBJ): $(START_ASM)
	$(ASM) -f elf64 $(START_ASM) -o $(START_OBJ)

$(INTERRUPT_OBJ): $(INTERRUPT_ASM)
	$(ASM) -f elf64 $(INTERRUPT_ASM) -o $(INTERRUPT_OBJ)

# Правила для компиляции .c файлов (автоматическое правило)
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

$(BUILD_DIR)/%.o: $(DRIVERS_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

# Линковка ядра
$(KERNEL_BIN): $(START_OBJ) $(INTERRUPT_OBJ) $(KERNEL_OBJS) $(KERNEL_LD)
	$(LD) $(LDFLAGS_KERNEL) --oformat binary $(START_OBJ) $(INTERRUPT_OBJ) $(KERNEL_OBJS) -o $(KERNEL_BIN)
	@echo "✅ Kernel compiled: $(KERNEL_BIN)"
	@echo "📦 Kernel size: $$(stat -f %z $(KERNEL_BIN) 2>/dev/null || stat -c %s $(KERNEL_BIN)) bytes"

# ============================================
# Копирование в образ
# ============================================
copy:
	@mkdir -p $(EFI_DIR)
	@cp $(EFI_FILE) $(EFI_DIR)/
	@cp $(KERNEL_BIN) $(IMG_DIR)/
	@echo "📁 Files copied to $(IMG_DIR)"
	@echo "  ├── $(EFI_DIR)/$(EFI_FILE)"
	@echo "  └── $(IMG_DIR)/$(KERNEL_BIN)"

# ============================================
# Запуск
# ============================================
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

# ============================================
# Очистка
# ============================================
clean:
	@rm -rf $(EFI_FILE) $(KERNEL_BIN) $(BUILD_DIR) $(IMG_DIR) serial.log
	@echo "🧹 Cleaned all files"

clean-img:
	@rm -rf $(IMG_DIR)
	@echo "🧹 Cleaned image directory"