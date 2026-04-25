#include "efi/efi.h"
#include "efi/efilib.h"
#include "efi/efiprot.h"



#include "structs.h"
static BOOLEAN MyCompareGuid(EFI_GUID *Guid1, EFI_GUID *Guid2) {
    UINT32 *g1 = (UINT32*)Guid1;
    UINT32 *g2 = (UINT32*)Guid2;
    
    for (int i = 0; i < 4; i++) {  // 4 x 32-bit = 128-bit GUID
        if (g1[i] != g2[i]) return FALSE;
    }
    return TRUE;
}

typedef void (*KernelEntry)(struct framebuffer_info* fb_inf, u32 lapic_base, u32 ioapic_base );
 
EFI_GUID gEfiFileInfoGuid = EFI_FILE_INFO_ID;
EFI_STATUS
EFIAPI      
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{       

    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"hello\n");
    
    ///////////FYLE SHI

    //get io and local APIC addrs
    struct XSDP* xsdp = NULL; //p - pointer
    u32 lapic_base =0;
    u32 ioapic_base =0;
    EFI_GUID acpi20_guid = ACPI_20_TABLE_GUID;
        for(int i =0; i<SystemTable->NumberOfTableEntries; i++){
            if(MyCompareGuid(&SystemTable->ConfigurationTable[i].VendorGuid, &acpi20_guid)){
                xsdp = (struct XSDP*)SystemTable->ConfigurationTable[i].VendorTable;
                break;
            }
        }
    if(!xsdp) SystemTable->ConOut->OutputString(SystemTable->ConOut, L"cant find xsdp");

  
  
    struct ACPI_HEADER *xsdt = (struct ACPI_HEADER*)(u64)xsdp->XsdtAddress;

    u64* entries = (u64*)(xsdt+1);
    u32 entries_count = (xsdt->Length - sizeof(struct ACPI_HEADER))/8;
    struct MADT* madt; 
        for(int i =0; i<entries_count;i++){
        struct ACPI_HEADER* table = (struct ACPI_HEADER*)(u64)entries[i];
            if(table->Signature == 0x43495041){ //APIC
            madt = (struct MADT*)table;
            break;

            }
        }
        if(!madt) SystemTable->ConOut->OutputString(SystemTable->ConOut, L"cant find MADT" );

    lapic_base = madt->LocalApicAddress;
    
    u8 *ptr = (u8*)madt + sizeof(struct ACPI_HEADER)+8;
    u8 *end = (u8*)madt + madt->Header.Length;
    while(ptr<end){
        u8 type = ptr[0];
        u8 length = ptr[1];
        
        if(type==1){
            ioapic_base = *(u32*)(ptr+4);
            SystemTable->ConOut->OutputString(SystemTable->ConOut, L"FOUND IOAPIC\n" );

            break;
        }
        ptr += length;
    }
    /////////////////////////

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSys;
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    EFI_STATUS status = SystemTable->BootServices->LocateProtocol(
        &fsGuid,
        NULL,
        (void**)&FileSys
    );


    /////////////GRAPHIC SHI
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics;
    EFI_GUID graph_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    
    status = SystemTable->BootServices->LocateProtocol(
        &graph_guid,
        NULL,
        (void**)&graphics
    );
    


    ////////////
    if(EFI_ERROR(status)){
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"LocateProtocol error");
    return status;}

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"LocateProtocol succes\n");

    EFI_FILE_PROTOCOL *Root;
    status = FileSys->OpenVolume(FileSys, &Root);

    ///eror check later
    
    EFI_FILE_PROTOCOL *File;
    status = Root->Open(
        Root,
        &File,
        L"kernel.bin",
        EFI_FILE_MODE_READ,
        0
    );
    //error file cheak
SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Open file succes\n");

    EFI_FILE_INFO *FileInfo;
    UINTN InfoSize = 0;
    status = File->GetInfo(
        File,
        &gEfiFileInfoGuid,
        &InfoSize,
        NULL
    );  
    //would be error chekc there

    status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        InfoSize,
        (void**)&FileInfo
    );
        //eror chec
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"allocate for fileinfo succes\n");
    status = File->GetInfo(File, &gEfiFileInfoGuid, &InfoSize, FileInfo);
    //eror chekdc

    UINT64 FileSize = FileInfo->FileSize;

    EFI_PHYSICAL_ADDRESS KernelAddr =0x200000;//bad but okay i guesss
    UINTN Pages = EFI_SIZE_TO_PAGES(FileSize);
    
    status = SystemTable->BootServices->AllocatePages(
        AllocateAddress, 
        EfiLoaderData,
        Pages,
        &KernelAddr 
    );
    //eror checl later
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"allocate for kernel succes\n");
    UINTN ReadSize = (UINTN)FileSize;
    status = File->Read(File, &ReadSize, (void*)KernelAddr);
    //ckeck

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"kernel is reed\n");
    File->Close(File);
    Root->Close(Root);
    SystemTable->BootServices->FreePool(FileInfo);

    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    UINTN Mapkey, DescriptorSize;
    UINT32 DescriptorVersion;

    status = SystemTable->BootServices->GetMemoryMap(
        &MemoryMapSize,
        NULL,
        &Mapkey,
        &DescriptorSize,
        &DescriptorVersion
    );
    //eror check late
    MemoryMapSize = MemoryMapSize+DescriptorSize*10;
    status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        MemoryMapSize,
        (void **)&MemoryMap
    );
    //eror checj

    status = SystemTable->BootServices->GetMemoryMap(
        &MemoryMapSize,
        MemoryMap,
        &Mapkey,
        &DescriptorSize,
        &DescriptorVersion
    );
    //eror check

    EFI_GRAPHICS_OUTPUT_PROTOCOL* Gop;





    status = SystemTable->BootServices->ExitBootServices(ImageHandle, Mapkey);
    if(EFI_ERROR(status)){
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"ExitBootServices error");
        return status;
    }

    

    //////////////////////////////////////////////


    //ENTRY
    struct framebuffer_info info;//// ALLOCATE POOLL LATER
    info.base = graphics->Mode->FrameBufferBase;
    info.size = graphics->Mode->FrameBufferSize;
    info.width = graphics->Mode->Info->HorizontalResolution ;
    info.height = graphics->Mode->Info->VerticalResolution;
    info.pitch = graphics->Mode->Info->PixelsPerScanLine;
    info.pixel_format = graphics->Mode->Info->PixelFormat;//?
    info.bpp = 4;//?

    


    KernelEntry entry = (KernelEntry)KernelAddr;
    entry(&info, lapic_base, ioapic_base);
  
    
    return EFI_SUCCESS;
}