#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "spi2.h"
#include "myprintf.h"
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
	Usart2_Init(115200);
	Myprintf_Init(0x00,myputc);
	
	while (1) 
	{
		delay(8000);
	}
}
