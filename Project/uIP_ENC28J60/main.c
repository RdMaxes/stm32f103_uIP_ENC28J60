#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "spi2.h"
#include "usart2.h"
#include "myprintf.h"
#include "enc28j60.h"
#include "timer6.h"
#include "uip.h"
#include "tapdev.h"
#include "timer.h" //it is timer.h of uIP Lib

void uip_polling(void);	//prototype of uIP check job
 												
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	

static void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}


int main(void)
{
	u8 tcp_server_tsta=0XFF;
	u8 tcp_client_tsta=0XFF;
 	uip_ipaddr_t ipaddr;

	LED_Init();
	Usart2_Init(115200);
	Myprintf_Init(0x00,myputc);

 	while(tapdev_init())	//ENC28J60 
	{								   
		my_printf("\r\nENC28J60 Init Error!");	 
		delay(80000*200);
	};
	uip_init();	//uIP initialize

 	uip_ipaddr(ipaddr, 192,168,1,16);	//setup local IP
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,1,1); 	//setup Gateway
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//setup Mask
	uip_setnetmask(ipaddr);

	uip_listen(HTONS(1200));			//listen to port 1200, as TCP server
	uip_listen(HTONS(80));				//listen to port 80, as http server
  	tcp_client_reconnect();	   		    //listem to remote port 1400, as client
	
	while (1) 
	{
		uip_polling();	//check uIP  mission every cycle
		if(tcp_server_tsta!=tcp_server_sta)//TCP Server status change
		{															 
			if(tcp_server_sta&(1<<7)) my_printf("\r\nTCP Server Connected.");
			else my_printf("\r\nTCP Server Disconnected");
 			if(tcp_server_sta&(1<<6))	//Rx new data
			{
    			my_printf("TCP Server RX:%s\r\n",tcp_server_databuf); //print out Rx data
				tcp_server_sta&=~(1<<6);		//clear Rx data ready flag	
			}
			tcp_server_tsta=tcp_server_sta; //update current server status
		}
		if(tcp_client_tsta!=tcp_client_sta)//TCP Client status change
		{															 
			if(tcp_client_sta&(1<<7))my_printf("\r\nTCP Client Connected.");
			else my_printf("\r\nTCP Client Disconnected");
 			if(tcp_client_sta&(1<<6))	//Rx new data
			{
    			my_printf("\r\nTCP Client RX:%s\r\n",tcp_client_databuf);	//print out Rx data
				tcp_client_sta&=~(1<<6);	//clear Rx data ready flag
			}
			tcp_client_tsta=tcp_client_sta;	//update current server status
		}
		delay(8000);		
	}
}
