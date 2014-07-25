#ifndef __TCP_DEMO_H__
#define __TCP_DEMO_H__		 
/* Since this file will be included by uip.h, we cannot include uip.h
   here. But we might need to include uipopt.h if we need the u8_t and
   u16_t datatypes. */
#include "uipopt.h"
#include "psock.h"

//comminication status
enum
{
	STATE_CMD		= 0,	//receiving command
	STATE_TX_TEST	= 1,	//continious Tx packet
	STATE_RX_TEST	= 2		//continious Rx packet  
};	 

//define uip_tcp_appstate_t variable type
//user cannot change the structure's name
//uip.h call structure below	  
struct tcp_demo_appstate
{
	uint8_t state;
	uint8_t *textptr;
	int textlen;
};	 
typedef struct tcp_demo_appstate uip_tcp_appstate_t;

void tcp_demo_appcall(void);
void tcp_client_demo_appcall(void);
void tcp_server_demo_appcall(void);

//define user application entrance
#ifndef UIP_APPCALL
	#define UIP_APPCALL tcp_demo_appcall  //define the uIP mission entrance function
#endif
/////////////////////////////////////TCP SERVER/////////////////////////////////////
extern uint8_t tcp_server_databuf[];   		//buffer for Tx data 
extern uint8_t tcp_server_sta;			    //server status
//tcp server º¯Êý
void tcp_server_aborted(void);
void tcp_server_timedout(void);
void tcp_server_closed(void);
void tcp_server_connected(void);
void tcp_server_newdata(void);
void tcp_server_acked(void);
void tcp_server_senddata(void);
/////////////////////////////////////TCP CLIENT/////////////////////////////////////
extern uint8_t tcp_client_databuf[];   		//buffer for Tx data  
extern uint8_t tcp_client_sta;				//client status 
void tcp_client_reconnect(void);
void tcp_client_connected(void);
void tcp_client_aborted(void);
void tcp_client_timedout(void);
void tcp_client_closed(void);
void tcp_client_acked(void);
void tcp_client_senddata(void);
////////////////////////////////////////////////////////////////////////////////////

#endif
