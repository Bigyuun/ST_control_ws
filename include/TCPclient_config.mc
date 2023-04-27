
#pragma once
#include <SysDef.mh>
#include "..\include\SDK\SDK_ApossC.mc"
#include "user_definition.mh"
#include "TCP_definition.mh"



/*********************************************************************
** State Machine Setup Parameters
*********************************************************************/

/*********************************************************************
** State Definitions
*********************************************************************/




/*********************************************************************
** Initialization Functions
*********************************************************************/

/** status of ethernet connection
SOCK_STATUS_INIT = 0,
SOCK_STATUS_WAITING = 1,
SOCK_STATUS_CONNECTING = 2,
SOCK_STATUS_READY = 3,
SOCK_STATUS_CLOSED = 4,
SOCK_STATUS_ERRORSENDING = -1,
SOCK_STATUS_ERROR = -2			*/
long TCP_get_connection_status(void)
{
	status = EthernetGetConnectionStatus(socketHandle);

	switch(status){

		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
	}

	if(status == -2) {
		printf("Socket Error! Check the connection (Error value : %ld)\n", status);
	}
	return status;
}

void TCP_receiveHandler(void)
{
	long i;

	retVal = EthernetReceiveTelegram(socketHandle, receiveData);
	//if(retVal !=0) print("TCP socket Error. check the network");
	//printf("rec : %ld\n", receiveData);
	//Dprint(receiveData[0]);
	//print(receiveData);
	for(i=0; i<NUM_OF_MOTORS;i++){
		target_val[i].ub0 = receiveData[BUFFER_TYPE*i+0];
		target_val[i].ub1 = receiveData[BUFFER_TYPE*i+1];
		target_val[i].ub2 = receiveData[BUFFER_TYPE*i+2];
		target_val[i].ub3 = receiveData[BUFFER_TYPE*i+3];
	}
	return;
}

/*********************************************************************
** Aposs Main Program
*********************************************************************/
long TCP_client_open(void)
{
	socketHandle = EthernetOpenClient(PROT_TCP, g_IP, g_PORT);

	//InterruptSetup(ETHERNET, TCP_receiveHandler, socketHandle);
//	InterruptSetup(TIME, interrupt_test, 1000);
//	InterruptEnablSe(ALL);

	if(socketHandle < 0) printf("There was an error: %ld \r\n", socketHandle);
	else printf("Success. The handle is: %ld \r\n", socketHandle);

	TCP_get_connection_status();
	print("status : ", status);

	return(1);
}

long TCP_sendmsg(long sendmsg[])
{
	//retVal = EthernetSendTelegram(socketHandle, sendData, BUFFER_SIZE);
	retVal = EthernetSendTelegram(socketHandle, sendmsg, BUFFER_SIZE);
	//if(retVal !=0) print("TCP socket Error. check the network");
	return retVal;
}





