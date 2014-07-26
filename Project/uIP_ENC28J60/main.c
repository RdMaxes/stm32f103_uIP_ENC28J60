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
#include "uip_arp.h"

void uip_polling(void);	//prototype of uIP check job

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	

static void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}

const uint8_t sudomymac[6]={0x04,0x02,0x35,0x00,0x00,0x01};	
int main(void)
{
	u8 tcp_server_tsta=0XFF;
	u8 tcp_client_tsta=0XFF;
 	uip_ipaddr_t ipaddr;

	LED_Init();
	//Usart2_Init(115200);
	//Myprintf_Init(0x00,myputc);

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


//uIP event check
//need to be called eevery cycle of main process
void uip_polling(void)
{
	u8 i;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
	if(timer_ok==0)//initialize timer at first time
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/2); //creat a timer for 0.5 sec 
		timer_set(&arp_timer,CLOCK_SECOND*10);	   //creat a timer for 10 sec 
	}				 
	uip_len=tapdev_read();	//read an IP packet form ehternet.
							//get data length uip_len which defined in uip.c 
	if(uip_len>0) 			//if data exist
	{   
		//handle IP packet, only the verified packet is used 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//IP packet? 
		{
			uip_arp_ipin();	//delete ethernet part
			uip_input();   	//IP packet handle
			//if data transmission is necessary
			//Tx data is in the uip_buf, length is uip_len	    
			if(uip_len>0)//response data is necessary
			{
				uip_arp_out();
				tapdev_send();
			}
		}else if (BUF->type==htons(UIP_ETHTYPE_ARP)) //is ARP packet?
		{
			uip_arp_arpin();
			//if data transmission is necessary
			//Tx data is in the uip_buf, length is uip_len	
 			if(uip_len>0)tapdev_send(); //send data via tapdev_send()	 
		}
	}else if(timer_expired(&periodic_timer))	//if 0.5sec timer expired
	{
		timer_reset(&periodic_timer);		//reset 0.5sec timer 
		//handle every TCP link  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//handle TCP mission  
			//if data transmission is necessary
			//Tx data is in the uip_buf, length is uip_len	    
			if(uip_len>0)//response data is necessary
			{
				uip_arp_out();
				tapdev_send();
			}
		}

		#if UIP_UDP	//if uIP_UDP enabled 
			//handle every UDP link 
			for(i=0;i<UIP_UDP_CONNS;i++)
			{
				uip_udp_periodic(i);	//handle UDP mission 
				//if data transmission is necessary
				//Tx data is in the uip_buf, length is uip_len	    
				if(uip_len>0)//response data is necessary
				{
					uip_arp_out();
					tapdev_send();
				}
			}
		#endif 

		//check if 10sec pass already, for updating ARP 
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}