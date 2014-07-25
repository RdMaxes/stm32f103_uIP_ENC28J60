#include "spi.h"


void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
		//Enable Clock
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );			
	 	//Pin Configuration
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);				 
		//SPI Configuration						                               
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;	
		SPI_Init(SPI2, &SPI_InitStructure);  	
	
	SPI_Cmd(SPI2, ENABLE); 	
	SPI2_ReadWriteByte(0xff);	 
}   

//Set SPI speed  
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI_InitTypeDef  SPI_InitStructure;

	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
} 


u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
		{
			retry++;
			if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPI2, TxData);
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
		{
			retry++;
			if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2);
}
