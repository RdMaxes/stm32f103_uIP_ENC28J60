#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "led.h"

static void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}


int main(void)
{
	LED_Init();

	while (1) 
	{
		RED_ON(); delay(80000);
		RED_OFF(); delay(80000);
		GREEN_ON(); delay(80000);
		GREEN_OFF(); delay(80000);				
	}
}
