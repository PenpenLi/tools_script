#include "VuGameSock.h"


#if defined VUANDROID || defined VUIOS

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "VuEngine/Util/VuLog.h"
#endif


VuGameSock::VuGameSock()
{
	mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#if defined VUANDROID || defined VUIOS
	if (mSocket == -1)
#else
	if (mSocket == INVALID_SOCKET)
#endif
	
	{
		VUPRINTF("Could not create socket!");
		//return VUNULL;
	}


	// bind socket to port
	/*sockaddr_in myAddr;
	memset(&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	myAddr.sin_addr.s_addr = ADDR_ANY;
	if (bind(socket, (sockaddr *)&myAddr, sizeof(myAddr)) == SOCKET_ERROR)
	{
		VUWARNING("Could not bind socket!");
		return VUNULL;
	}

	VuWinGameSock *pSocket = new VuWinGameSock();
	pSocket->mSocket = socket;

	return pSocket;*/

}

//*****************************************************************************
VuGameSock::~VuGameSock()
{
/*
	shutdown(mSocket, SD_BOTH);
	closesocket(mSocket);*/
}



//*****************************************************************************
int VuGameSock::send(const void *pData, int dataSize)
{
	int bytesSent = ::send(mSocket, (char *)pData, dataSize, 0);

	return bytesSent;
}

//*****************************************************************************
int VuGameSock::recv(void *pData, int bufSize)
{
	VUPRINTF("begin recv VuGameSock::recv");
	int bytesReceived = ::recv(mSocket, (char *)pData, bufSize, 0);
	VUPRINTF("end recv VuGameSock::recv");
	
	return bytesReceived;
}

