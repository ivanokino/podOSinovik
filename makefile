#vibecoded




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
KERNEL_DIR = $(SRC_DIR)/kernel
DRIVERS_DIR = $(SRC_DIR)/drivers
UTILS_DIR = $(SRC_DIR)/utils
EFI_DIR_SRC = $(SRC_DIR)/efi

# ============================================
# Файлы
# ============================================
EFI_FILE = BOOTX64.EFI
KERNEL_BIN = kernel.bin

# Загрузчик
SRC = $(SRC_DIR)/starter.c

# Ядро (ассемблер + C файлы)
KERNEL_ASM = $(SRC_DIR)/start.asm
KERNEL_C = \
    $(KERNEL_DIR)/kernel.c \
    $(DRIVERS_DIR)/screen.c

KERNEL_LD = $(KERNEL_DIR)/kernel.ld

# Заголовочные файлы
HEADERS = \
    $(UTILS_DIR)/structs.h \
    $(UTILS_DIR)/types.h \
    $(UTILS_DIR)/abi.h \
    $(DRIVERS_DIR)/screen.h \
    $(DRIVERS_DIR)/font.h

# Объектные файлы
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
    -I$(UTILS_DIR) \
    -I$(DRIVERS_DIR) \
    -I$(EFI_DIR_SRC) \
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
    -I$(UTILS_DIR) \
    -I$(DRIVERS_DIR)

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
$(EFI_FILE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS_EFI) $(SRC) -o $(EFI_FILE)
	@echo "✅ EFI loader compiled: $(EFI_FILE)"

# ============================================
# Сборка ядра
# ============================================
$(START_OBJ): $(KERNEL_ASM)
	$(ASM) -f elf64 $(KERNEL_ASM) -o $(START_OBJ)

# Правило для компиляции .c файлов из разных директорий
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

$(BUILD_DIR)/screen.o: $(DRIVERS_DIR)/screen.c $(HEADERS)
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

# Линковка ядра
$(KERNEL_BIN): $(START_OBJ) $(KERNEL_OBJS) $(KERNEL_LD)
	$(LD) $(LDFLAGS_KERNEL) --oformat binary $(START_OBJ) $(KERNEL_OBJS) -o $(KERNEL_BIN)
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