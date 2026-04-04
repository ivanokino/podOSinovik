#ifndef ABI_H
#define ABI_H

#ifdef __x86_64__
    #define MS_ABI __attribute__((ms_abi))
#else
    #define MS_ABI
#endif

#endif  