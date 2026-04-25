



#ifndef STRUCTS_H
#define STRUCTS_H
#include "types.h"

// struct RSDP{
//     char Signature[8];
//     u8 CheckSum;
//     char OEMID[6];
//     u8 Revision;
//     u32 RsdtAddress;
// }__attribute__((packed));


struct XSDP {
    char Signature[8];
    u8 Checksum;
    char OEMID[6];
    u8 Revision;
    u32 RsdtAddress;
    u32 Length;
    u64 XsdtAddress;
    u8 ExtendedChecksum;
    u8 reserved[3];
} __attribute__((packed));


struct ACPI_HEADER{
    u32 Signature;
    u32 Length;
    u8 Revision;
    u8 CheckSum;
    char OEMID[6];
    char OEMTABLEID[8];
    u32 OEMREVISION;
    u32 CreatorId;
    u32 CreatorRevision;
}__attribute__((packed));

struct MADT{
    struct ACPI_HEADER Header;
    u32 LocalApicAddress;
    u32 Flags;
}__attribute__((packed)) ;

struct framebuffer_info{
    u64 base;
    u64 size;
    u32 width;
    u32 height;
    u32 pitch;
    u8 bpp; //byte for pixel
    u8 pixel_format;

    } __attribute__((packed));

#endif