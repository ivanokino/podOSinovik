#include "../bootloader/structs.h"
 #include "../efi/efi.h"
#include "../efi/efilib.h"
#include "../utils/string.h"
#include "../drivers/screen.h"
void print_largest_block(struct MemMapInfo* mmi){
    EFI_MEMORY_DESCRIPTOR* map = mmi->MemoryMap;
    u64 size = mmi->MapSize;
    u64 desc_size = mmi->DescriptorSize;
    u64 max_size = 0;
    u64 max_addr = 0;
    u8 string[20];
    print("\n");
    print("=== All Conventional Memory Blocks ===\n");
    
    for(u64 i = 0; i < size / desc_size; i++){
        if(map->Type == EfiConventionalMemory){
            u64 block_size = map->NumberOfPages * 4096ULL; //cuz 1 page = 4KB or 4096 bytes
            u64 block_addr = map->PhysicalStart;
            
        
            print("Block: addr = ");
            u64_to_ascii(block_addr / (1024ULL*1024ULL), string); // /1024*1024-from byte to megabytes
            print(string);
            print(" MB, size = ");
            u64_to_ascii(block_size / (1024ULL*1024ULL), string);
            print(string);
            print(" MB\n");
            
            if(block_size > max_size){
                max_size = block_size;
                max_addr = block_addr;
            }
        }
        map = (EFI_MEMORY_DESCRIPTOR*)((u8*)map + desc_size);
    }
    
    print("\n=== Largest Block ===\n");
    print("addr: ");
    u64_to_ascii(max_addr / (1024ULL*1024ULL), string);
    print(string);
    print(" MB, size: ");
    u64_to_ascii(max_size / (1024ULL*1024ULL), string);
    print(string);
    print(" MB\n");
}