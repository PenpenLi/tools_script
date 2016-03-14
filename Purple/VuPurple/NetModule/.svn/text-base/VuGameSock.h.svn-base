/********************************************************************
	Copyright (c) 2014-2016  xuliming
	created:	2015-2-3   16:29
	filename: 	VuWinGameSock.h
	author:		xlm, foreverxuliming@gmail.com	
	brief:		

*********************************************************************/

#ifndef __VU_WIN_GAME_SOCK_H__
#define __VU_WIN_GAME_SOCK_H__


#if defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
#include "VuEngine/HAL/Net/Winsock/VuWinsockSocket.h"
#else
#include "VuEngine/HAL/Net/Linux/VuLinuxNet.h"
#endif


#if defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
class VuGameSock : public VuWinsockTcpSocket
#else
class VuGameSock : public VuLinuxTcpSocket
#endif


{
public:
	VuGameSock();
	virtual ~VuGameSock();

	
	
	virtual int			send(const void *pData, int dataSize);
	virtual int			recv(void *pData, int bufSize);



};



#endif
