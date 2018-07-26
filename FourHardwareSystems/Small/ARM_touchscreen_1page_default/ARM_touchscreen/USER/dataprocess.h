#ifndef __DATAPROCESS_H
#define __DATAPROCESS_H

#include "stm32f10x.h"  


u8 CRC_check(u8 arr[],u8 start, u8 stop)

{
    u8 i,value;
    u16 tep=0;
 
    for(i=start;i<stop;i++)
    {
        tep+=arr[i];
    }
    value= tep%100;
    return value;
}

u8 Digital_handle(u8 arr[],u8 nbit)

{
   u8 i,value,j;  
   value=0;
   j=(nbit-1)*4; 
   for(i=0;i<4;i++)
   {
     value|=((arr[i+j]&1)<<i);
   }
   return value;
}




#endif
