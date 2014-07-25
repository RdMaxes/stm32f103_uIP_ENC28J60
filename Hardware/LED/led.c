#include "led.h"

//LED configuration 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    //Enable LED1 Clock
	RCC_APB2PeriphClockCmd(LED1_CLK, ENABLE);
	//Configure LED1 Pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 	LED1_Pin; 	
	GPIO_Init(LED1_Port, &GPIO_InitStructure);

    //Enable LED2 Clock
	RCC_APB2PeriphClockCmd(LED2_CLK, ENABLE);
	//Configure LED2 Pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 	LED2_Pin; 	
	GPIO_Init(LED2_Port, &GPIO_InitStructure);
}
