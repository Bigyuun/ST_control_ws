
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
	retVal = EthernetReceiveTelegram(socketHandle, receiveData);
	//if(retVal !=0) print("TCP socket Error. check the network");
	printf("rec : %ld", receiveData[0]);
	print(receiveData);
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
//	InterruptEnable(ALL);

	if(socketHandle < 0) printf("There was an error: %ld \r\n", socketHandle);
	else printf("Success. The handle is: %ld \r\n", socketHandle);

	TCP_get_connection_status();
	print("status : ", status);

	return(1);
}

long TCP_sendmsg()
{
	retVal = EthernetSendTelegram(socketHandle, actual_pos, 8);
	//if(retVal !=0) print("TCP socket Error. check the network");
	return retVal;
}





