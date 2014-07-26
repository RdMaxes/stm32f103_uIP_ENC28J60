#include "tcp_demo.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	   

/*Modified from ATOM@ALIENTEK*/


uint8_t tcp_client_databuf[200];   	//Tx buffer	  
uint8_t tcp_client_sta;				//client status
//[7]: 0 = no connection; 1=connected
//[6]: 0 = no data; 1= get client data
//[5]: 0 = no data; 1= data ready to send

//MCU here is a client
//this function is called by UIP_APPCALL, which is defined as tcp_client_demo_appcall in tcp_demo.h
//function is called when TCP link created, new data arrived, data re-send request ...etc
void tcp_client_demo_appcall(void)
{		  
 	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(uip_aborted())tcp_client_aborted();		//connection stop	   
	if(uip_timedout())tcp_client_timedout();	//timeout   
	if(uip_closed())tcp_client_closed();		//connection closed	   
 	if(uip_connected())tcp_client_connected();	//success	    
	if(uip_acked())tcp_client_acked();			 
 	//get a new TCP packet 
	if (uip_newdata())
	{
		if((tcp_client_sta&(1<<6))==0)//no data 
		{
			if(uip_len>199)
			{		   
				((uint8_t*)uip_appdata)[199]=0;
			}		    
	    	strcpy((char*)tcp_client_databuf,uip_appdata);				   	  		  
			tcp_client_sta|=1<<6;//set rx data flag
		}				  
	}else if(tcp_client_sta&(1<<5))//data ready to send
	{
		s->textptr=tcp_client_databuf;
		s->textlen=strlen((const char*)tcp_client_databuf);
		tcp_client_sta&=~(1<<5);//clear flag
	}  
	//When re-send request, new data arrived, packet arrived, connection created ...etc 
	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
	{
		tcp_client_senddata();
	}											   
}

//reconnet to server
//server IP is set into 192.168.1.103, port = 1400
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(&ipaddr,192,168,200,3);	
	uip_connect(&ipaddr,htons(1400)); 	
}

//stop connection			    
void tcp_client_aborted(void)
{
	tcp_client_sta&=~(1<<7);	//set flag with no link
	tcp_client_reconnect();		//try to reconnet
	uip_log((char*)"tcp_client aborted!\r\n");
}

void tcp_client_timedout(void)
{
	tcp_client_sta&=~(1<<7);		   
	uip_log((char*)"tcp_client timeout!\r\n");
}

void tcp_client_closed(void)
{
	tcp_client_sta&=~(1<<7);	
	tcp_client_reconnect();		
	uip_log((char*)"tcp_client closed!\r\n");
}	 

void tcp_client_connected(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=1<<7;		
  	uip_log((char*)"tcp_client connected!\r\n");
	s->state=STATE_CMD; 		
	s->textlen=0;
	s->textptr=(uint8_t*)"Connet to RdMaxes Client!!\r\n";
	s->textlen=strlen((char *)s->textptr);	  
}

void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;
	uip_log((char*)"tcp_client acked!\r\n");		 
}

void tcp_client_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;	   
	if(s->textlen>0) {uip_send(s->textptr, s->textlen);}						
}