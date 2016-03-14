#include "PackHandler.h"

#include "NetBaseDef.h"

#include "VuPurple/Protocol/GameProtoId.h"
//#include "VuPurple/Protocol/GameProto.pb.h"

#include "VuPurple/Protocol/GameProtoOld.h"


#if defined VUANDROID || defined VUIOS

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "VuEngine/Util/VuLog.h"




uint64_t htonll(uint64_t val) {
	return (((uint64_t)htonl(val)) << 32) + htonl(val >> 32);
}


#else
#include "VuEngine/Prefix/VuDebugSettings.h"


#endif

uint64_t ntohll(uint64_t val) {
	return (((uint64_t)ntohl(val)) << 32) + ntohl(val >> 32);
}



#include "VuPurple/Managers/VuNetManager.h"

/*

#include "VuEngine/Managers/VuProfileManager.h"*/

#include "VuPurple/Managers/VuGameManager.h"
#include <time.h>


/*判断大端还是小端， 1：小端  0：大端*/
int GetEndian()
{
	union
	{
		short s;
		char c[sizeof(short)];
	}un;

	un.s = 0x0102;

	if (2 == sizeof(short))
	{
		if (1 == un.c[0] && 2 == un.c[1])
		{
			//printf("big-endian\n");
			return 0;
		}
		else if (2 == un.c[0] && 1 == un.c[1])
		{
			//printf("little-endian\n");
			return 1;
		}
		else
		{
			//printf("unknown\n");
			return 3;
		}
	}
	else
	{
		//printf("sizeof(short) = %d\n", sizeof(short));
	}

	return 0;
}



#define REGISTER_MSG_HANDLE_PROC(MSGID, Proc) m_mapNetMessage[MSGID]=Proc

PackHandler::PackHandler()
{

}


PackHandler::~PackHandler()
{

}


bool PackHandler::Init()
{
	REGISTER_MSG_HANDLE_PROC(MSG_G_GET_SERVER_TIME_ACK, &PackHandler::__GetServerTimeAck);
	REGISTER_MSG_HANDLE_PROC(MSG_G_GET_SHOW_IMM_BUTTON_TIP_ACK, &PackHandler::__GetShowImmeButtonTipAck);

	return true;
}

void PackHandler::OnRecv(const char* pBuf, unsigned int dwLen)
{

	if (dwLen >= gnNetMsgHeaderSize)
	{
		STNetMsgHeader* pNetHeader = (STNetMsgHeader*)(pBuf);
		unsigned short wMsgID = ntohs(pNetHeader->wMsgID);
		unsigned int dwDataLen = ntohl(pNetHeader->dwDataLen);
		NetMessageMap::iterator it = m_mapNetMessage.find(wMsgID);
		if (it != m_mapNetMessage.end() && it->second)
		{
			(this->*it->second)(pBuf+gnNetMsgHeaderSize, dwLen-gnNetMsgHeaderSize);

		}


	}
}

/*
@ Author:	xlm(2015-2-3 19:01)
@ Parameter:
@ Returns:
@ Description:
*/
void PackHandler::__GetServerTimeAck(const char* szBuf, unsigned int dwLen)
{

	
	/*GameProto::PKG_G_GET_SERVER_TIME_ACK stAck;
	bool bRet = stAck.ParseFromArray(szBuf, dwLen);
	if (!bRet)
	{
		return;
	}
	if (stAck.errcode() != 0)
	{
		return;
	}

	unsigned long long uServerTime = stAck.svrtime();*/

	VUPRINT("PackHandler::__GetServerTimeAck size:%d", dwLen);

	

	

	PKG_G_GET_SERVER_TIME_ACK* pAck = (PKG_G_GET_SERVER_TIME_ACK*)szBuf;
	if (pAck /*&& pAck->errCode == 0*/)
	{
		//VUPRINTF(asctime(gmtime((time_t*)&(pAck->time))));
		VuGameManager::IF()->setCurrentServerTime(pAck->time);

		if (pAck->time == 0)
		{
			VUPRINT("server time i 0");
		}

		int nsize = sizeof(PKG_G_GET_SERVER_TIME_ACK);
		VUPRINTF("PKG_G_GET_SERVER_TIME_ACK size is:%d", nsize);


		

		char szMsg[64] = { 0 };
		sprintf(szMsg, "%llu", pAck->time);
		VUPRINT("server time:%s", szMsg);

		/*int nMode =  GetEndian();
		if (nMode == 0)
			VUPRINT("big endian");
		else
			VUPRINT("small endian");
*/


		//VuGameManager::IF()->setShowImmediateGetButtonTip(pAck->byShow == 1 ? true : false);

		
		//VUPRINT("immebuttontip:%d", pAck->byShow);

		char szMsg1[64] = { 0 };
		STNetMsgHeader* pHeader = (STNetMsgHeader*)szMsg1;
		pHeader->wMsgID = htons(MSG_G_GET_SHOW_IMM_BUTTON_TIP_REQ);
		pHeader->dwDataLen = htonl(0);
		VuNetManager::IF()->send(szMsg1, gnNetMsgHeaderSize);
		//m_pTcpSocket->send(szMsg1, gnNetMsgHeaderSize);

	
	}


}



/*
@ Author:	xlm(2015-2-5 19:49)
@ Parameter:
@ Returns:
@ Description:获取是否显示立即领取的应答包
*/
void PackHandler::__GetShowImmeButtonTipAck(const char* szBuf, unsigned int dwLen)
{
	PKG_G_GET_SHOW_IMM_BUTTON_TIP_ACK* pAck = (PKG_G_GET_SHOW_IMM_BUTTON_TIP_ACK*)szBuf;
	if (pAck && pAck->errCode == 0)
	{
		VUPRINTF("received __GetShowImmeButtonTipAck, show=%d", pAck->byShow);
		VuGameManager::IF()->setShowImmediateGetButtonTip(pAck->byShow == 1 ? true : false);

	}
}
