#include "string.h"


u8 strcmp(u8 *str1, u8* str2){

while(*str1==*str2 && *str1!='\0' ){
    str1++;str2++;
}
return *str1-*str2;
    
}


 void int_to_ascii(int n, char str[]){
    int i, sign;
    if((sign=n)<0) n = -n;
    i=0;
    do{
        str[i++]=n%10+'0';
    }while ((n/=10)>0);
    if(sign<0)str[i++]='-';
    str[i]='\0';

    int lenght = 0;
    while(str[lenght]!='\0')lenght++;

    for(int i=0;i<lenght/2;i++){
        char temp = str[i];
        str[i]=str[lenght-i-1];
        str[lenght-i-1]=temp;
    }
}


 void u64_to_ascii(signed long long n, char str[]){
    signed long long i, sign;
    if((sign=n)<0) n = -n;
    i=0;
    do{
        str[i++]=n%10+'0';
    }while ((n/=10)>0);
    if(sign<0)str[i++]='-';
    str[i]='\0';

    int lenght = 0;
    while(str[lenght]!='\0')lenght++;

    for(int i=0;i<lenght/2;i++){
        char temp = str[i];
        str[i]=str[lenght-i-1];
        str[lenght-i-1]=temp;
    }
}