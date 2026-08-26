    #include "../bootloader/structs.h"
    #include "../efi/efi.h"
    #include "../efi/efilib.h"
    #include "../utils/string.h"
    #include "../drivers/screen.h"


    

    EFI_MEMORY_DESCRIPTOR* find_largest_block(struct MemMapInfo* mmi){

    EFI_MEMORY_DESCRIPTOR* desc = mmi->MemoryMap;
    u64 size = mmi->MapSize;
    u64 desc_size = mmi->DescriptorSize;
    u64 max_size = 0;
    u64 max_addr = 0;
    static EFI_MEMORY_DESCRIPTOR max_desc_s = {0};
    EFI_MEMORY_DESCRIPTOR* max_desc = &max_desc_s;
    
    for (u32 i =0; i < size / desc_size; i++){

        if(desc->Type == EfiConventionalMemory){
            u64 block_size = desc->NumberOfPages;
            u64 block_addr = desc->PhysicalStart;

            if(max_desc->NumberOfPages < block_size){
                max_desc->NumberOfPages = block_size;
                max_desc->PhysicalStart = block_addr;
                max_desc->Attribute = desc->Attribute;
                max_desc->Pad = desc->Pad;

                max_desc->Type = desc->Type;
                max_desc->VirtualStart = desc->VirtualStart;


            }

        }

        desc = (EFI_MEMORY_DESCRIPTOR*)((u8*)desc + desc_size); 

    }
    return max_desc;
    }





    u64 init_heap(struct MemMapInfo* mmi){

    EFI_MEMORY_DESCRIPTOR* heap = find_largest_block(mmi);
    u64 heap_addr = heap->PhysicalStart;


    u64 heap_size = heap->NumberOfPages * 4;
    heap_size /= 1024; 

    u8 string_buff [20]= {0};
    u64_to_ascii(heap_size, string_buff);
    print(string_buff);
    print("MB \n");


    return heap_addr;


    }




        // void print_largest_block(struct MemMapInfo* mmi){
    //     EFI_MEMORY_DESCRIPTOR* map = mmi->MemoryMap;
    //     print("\n=== Largest Block ===\n");
    //     print("addr: ");
    //     u64_to_ascii(max_addr / (1024ULL*1024ULL), string);
    //     print(string);
    //     print(" MB, size: ");
    //     u64_to_ascii(max_size / (1024ULL*1024ULL), string);
    //     print(string);
    //     print(" MB\n");
    // }
