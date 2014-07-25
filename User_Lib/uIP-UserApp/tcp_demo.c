#include "uip.h"	    
#include "enc28j60.h"
#include "tcp_demo.h"

//TCP application
//Called by uIP via UIP_APPCALL
//provide TCP and http serice
void tcp_demo_appcall(void)
{	
  	
	switch(uip_conn->lport)//local port mission, port 80 and 1200
	{
//		case HTONS(80):
//			httpd_appcall(); 
//			break;
		case HTONS(1200):
		    tcp_server_demo_appcall(); 
			break;
		default:						  
		    break;
	}		    
	switch(uip_conn->rport)	//remote port mission, port:1400
	{
	    case HTONS(1400):
			tcp_client_demo_appcall();
	       break;
	    default: 
	       break;
	}   
}
//for print out message via serial port
void uip_log(char *m)
{			    
	//printf("uIP log:%s\r\n",m);
}

























