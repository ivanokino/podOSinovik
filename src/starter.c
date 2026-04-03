#include "efi/efi.h"
#include "efi/efilib.h"




#include "structs.h"



typedef void (*KernelEntry)(struct framebuffer_info* fb_inf);
 
EFI_GUID gEfiFileInfoGuid = EFI_FILE_INFO_ID;
EFI_STATUS
EFIAPI      
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{   
    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"hello\n");
    
    ///////////FYLE SHI

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

    EFI_PHYSICAL_ADDRESS KernelAddr =0x100000;//bad but okay i guesss
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
    entry(&info);
  
    
    return EFI_SUCCESS;
}