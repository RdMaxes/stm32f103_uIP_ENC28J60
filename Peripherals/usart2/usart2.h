#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stdio.h"



/*Function Prototype*/
void Usart2_Init(uint32_t baudrate);
void USART2_IRQHandler(void);
#endif

