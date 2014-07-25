#ifndef _SPI_H
#define _SPI_H
#include "stm32f10x.h"

/*Function Prototype*/ 				  	    													  
void SPI2_Init(void);	
void SPI2_SetSpeed(u8 SpeedSet);   
u8 SPI2_ReadWriteByte(u8 TxData);
		 
#endif


