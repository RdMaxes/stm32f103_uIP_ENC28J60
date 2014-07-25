#include "usart2.h"

//setup usart2
//baudrate: assigned baudrate
void Usart2_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure; 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//Rx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_3;	// USART2 Rx (PA.3)								
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_2;	// USART2 Tx (PA.2)
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Parameters for USART2
	USART_InitStructure.USART_BaudRate 				= baudrate;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;   
	// Configure USART2
	USART_Init(USART2, &USART_InitStructure);		
	USART_Cmd(USART2, ENABLE);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)	;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	    	
	/* Enable the USART2 Interrupt */																																												
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;																																												
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;																																												
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																																												
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																																												
//	NVIC_Init(&NVIC_InitStructure);		
}

//usart2 interrupt function
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//if rx data
	{	 
		res=USART2->DR; 	
		res = res+1; //dummy code		 
	}  											 
}  