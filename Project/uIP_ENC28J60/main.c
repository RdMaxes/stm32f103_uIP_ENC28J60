#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "spi2.h"
#include "enc28j60.h"
#include "timer6.h"
#include "uip.h"
#include "tapdev.h"
#include "timer.h" //it is timer.h of uIP Lib

static void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}


int main(void)
{
	LED_Init();

	while (1) 
	{
		LED1_ON(); delay(80000);
		LED1_OFF(); delay(80000);
		LED2_ON(); delay(80000);
		LED2_OFF(); delay(80000);				
	}
}
