#include "string.h"


u8 strcmp(u8 *str1, u8* str2){

while(*str1==*str2 && *str1!='\0' ){
    str1++;str2++;
}
return *str1-*str2;
    
}