#include "VuNetManager.h"

#include "VuEngine/HAL/Net/VuNet.h"



#include "VuPurple/NetModule/VuGameSock.h"



#include "VuEngine/Managers/VuTickManager.h"


#include "VuPurple/NetModule/PacketParser.h"
#include "VuPurple/NetModule/PackBufPool.h"
#include "VuPurple/NetModule/PackBuf.h"
#include "VuPurple/NetModule/PackHandler.h"

#include "VuPurple/Protocol/GameProtoId.h"

#include "VuEngine/HAL/Thread/VuThread.h"

#if defined VUANDROID || defined VUIOS

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "VuEngine/Util/VuLog.h"

#endif





#define SERVER_IP "bbr.mopo.com"
#define SERVER_PORT 7003

//test



/*
#define SERVER_IP "172.16.8.89"
#define SERVER_PORT 7001*/

void NetThreadProc(void* pArg)
{
	//VuNetManager* pNetMgr = ;
	//	(VuNetManager*)pArg;
	//	if (!VuNetManager::IF())
	//{
	//	return;
	//}

	while (VuNetManager::IF() && VuNetManager::IF()->isConnected())
	{
		VuNetManager::IF()->NetProcess();

#if defined VUANDROID || defined VUIOS
		usleep(100);
#else
		Sleep(100);

#endif
		
	}

	VuNetManager::IF()->SetThreadHandleNULL();
}


IMPLEMENT_SYSTEM_COMPONENT(VuNetManager, VuNetManager);
VuNetManager::VuNetManager() : m_pTcpSocket(NULL), m_hThread(NULL), m_nUsedPackBufLen(0), m_pPackBufPool(NULL), m_bIsConnected(false), m_pPackHandler(NULL), m_hRecvLock(NULL)
{

}

VuNetManager::~VuNetManager()
{
	release();
}



void	VuNetManager::preRelease()
{
}



/*!
* \author: xlm 2015/1/28
* \brief: connect to a ip and port
*/


bool VuNetManager::connect(const char* szIp, unsigned short uPort, int nTimeoutMs)
{
	if (m_pTcpSocket)
	{
		VUPRINT("connect %s:%d", szIp, uPort);
		return m_pTcpSocket->connect(szIp, uPort, nTimeoutMs);
	}
	return false;
}

void VuNetManager::disConnect()
{
	if (m_pTcpSocket)
	{
		VUPRINT("disconnect");
		m_pTcpSocket->disconnect();
	}
}


int VuNetManager::send(const char* szData, int nDataSize)
{
	if (m_pTcpSocket)
	{
		return m_pTcpSocket->send(szData, nDataSize);
	}
	return 0;
}

int VuNetManager::recv(char* szData, int nBufSize)
{
	VUPRINT("receive a message");
	if (m_pTcpSocket)
	{
		return m_pTcpSocket->recv((void*)szData, nBufSize);
	}
	return 0;
}

/*!
* \author: xlm 2015/1/28
* \brief: set block or nonblock
*/
bool VuNetManager::setNonBlocking(bool nonBlock)
{
	if (m_pTcpSocket)
	{
		return m_pTcpSocket->setNonBlocking(nonBlock);
	}
	return false;
}
/*!
* \author: xlm 2015/1/28
* \brief: set the timeout of send and recv
*/
bool VuNetManager::setTimeOut(int nRecvMS, int nSendMs)
{
	if (m_pTcpSocket)
	{
		return m_pTcpSocket->setTimeOut(nRecvMS, nSendMs);
	}
	return false;
}

bool	VuNetManager::init()
{

	if (!m_pTcpSocket)
	{
		m_pTcpSocket = new VuGameSock();

	}

	if (!m_pTcpSocket)
	{
		VUPRINT("TcpSocket create failed");
		return false;
	}
		

	//Á´½ÓÔ¶³Ì

	m_pTcpSocket->setNonBlocking(true);
	m_pTcpSocket->setTimeOut(1000, 1000);

	// register phased tick
	VuTickManager::IF()->registerHandler(this, &VuNetManager::tick, "Network");


	m_pPackBufPool = new PackBufPool(20);

	m_pPackHandler = new PackHandler();
	if (m_pPackHandler)
	{
		m_pPackHandler->Init();
	}

	return m_pTcpSocket != NULL ? true : false;

}

void VuNetManager::SetThreadHandleNULL()
{
	m_hThread = NULL;
}

void	VuNetManager::postInit()
{
	VUPRINT("postInit ----------------------");
	//modify by liudan
	if (!m_pTcpSocket)
		return;

	bool bRet = m_pTcpSocket->connect(SERVER_IP, SERVER_PORT, 1000);
	if (!bRet)
	{
		VUPRINT("connect to %s:%d failed", SERVER_IP, SERVER_PORT);
		m_bIsConnected = false;
		return;
	}
	VUPRINT("connet to %s:%d ok ----------------------", SERVER_IP, SERVER_PORT);
	m_bIsConnected = true;

	m_hRecvLock = VuThread::IF()->createCriticalSection();
	m_hThread = VuThread::IF()->createThread(NetThreadProc, this);

	char szMsgData[64] = { 0 };
	char* pMsgBody = szMsgData + gnNetMsgHeaderSize;

	STNetMsgHeader* pHeader = (STNetMsgHeader*)szMsgData;
	pHeader->wMsgID = htons(MSG_G_GET_SERVER_TIME_REQ);
	pHeader->dwDataLen = htonl(0);

	m_pTcpSocket->send(szMsgData, gnNetMsgHeaderSize);

	VUPRINT("send MSG_G_GET_SERVER_TIME_REQ");
}

void	VuNetManager::release()
{

	if (m_pTcpSocket)
	{
		m_pTcpSocket->disconnect();
		delete m_pTcpSocket;
		m_pTcpSocket = NULL;
	}

	m_bIsConnected = false;

	/*if (m_hThread)
	{
		VuThread::IF()->joinThread(m_hThread);
		m_hThread = NULL;
	}

	if (m_hRecvLock)
	{
		VuThread::IF()->deleteCriticalSection(m_hRecvLock);
		m_hRecvLock = NULL;
	}*/

	if (m_pPackBufPool)
	{
		delete m_pPackBufPool;
		m_pPackBufPool = NULL;
	}

	VuTickManager::IF()->unregisterHandlers(this);
}


void VuNetManager::tick(float fdt)
{
	
	//if (m_bIsConnected)
	{
		while (!m_listMsg.empty())
		{
			VUPRINT("VuNetManager::tick");
			VuThread::IF()->enterCriticalSection(m_hRecvLock);
			PackBuf* pPackBuf = m_listMsg.back();

			m_listMsg.pop_back();
			VuThread::IF()->leaveCriticalSection(m_hRecvLock);

			if (m_pPackHandler)
			{
				m_pPackHandler->OnRecv(pPackBuf->GetBuf(), pPackBuf->GetUsedLen());
			}
			if (m_pPackBufPool)
			{
				m_pPackBufPool->FreePackBuf(pPackBuf);
			}

		}
	}


}

void VuNetManager::NetProcess()
{

	static char szReadBuf[MAX_PACKET_LEN] = { 0 };
	if (!m_pTcpSocket)
	{
		return;
	}
	VUPRINT("begin recv in VuNetManager::NetProcess");
	int nSize = m_pTcpSocket->recv(szReadBuf, MAX_PACKET_LEN);

	VUPRINT("VuNetManager::NetProcess recv size: %d", nSize);
	if (nSize == 0)
	{
		m_bIsConnected = false;
		m_pTcpSocket->disconnect();


		/*if (m_hThread)
		{
			VuThread::IF()->destroyEvent(m_hThread);
			//VuThread::IF()->joinThread(m_hThread);
			m_hThread = NULL;
		}*/

		if (m_pTcpSocket)
		{
			delete m_pTcpSocket;
			m_pTcpSocket = NULL;
		}

	}
	else if (nSize == -1)
	{
	}
	else
	{
		VUPRINT("__ParsePack VuNetManager::NetProcess");
		__ParsePack(szReadBuf, nSize);
	}
}


/*
@ Author:	xlm(2015-2-3 10:58)
@ Parameter:
@ Returns:
@ Description:
*/
void VuNetManager::__ParsePack(const char* szBuf, int nLen)
{
	VUPRINT("__ParsePack nLen=%d", nLen);
	if (m_nUsedPackBufLen + nLen > MAX_PACKET_LEN || nLen == 0)
		return;
	memcpy(m_szPackBuf + m_nUsedPackBufLen, szBuf, nLen);
	m_nUsedPackBufLen += nLen;
	char* szStart = m_szPackBuf;
	while (m_nUsedPackBufLen > 0)
	{
		int nUsed = m_nUsedPackBufLen;

		nUsed = PacketParser::ParsePacket(szStart, m_nUsedPackBufLen);
		if (nUsed == 0)
		{
			if (m_nUsedPackBufLen >= MAX_PACKET_LEN)
			{
				VUPRINT("cm_nUsedPackBufLen >= MAX_PACKET_LEN");
				//overflow, outof memory!
				return;
			}
			break;
		}
		else if (nUsed > 0)
		{
			if (nUsed > m_nUsedPackBufLen)
			{
				//don't have enough buffer to hold the package
				VUPRINT("nUsed > m_nUsedPackBufLen");

				return;
			}

			PackBuf* pPackBuf = m_pPackBufPool->GetPackBuf();
			if (pPackBuf)
			{
				pPackBuf->SetData(szStart, nUsed);

				VuThread::IF()->enterCriticalSection(m_hRecvLock);
				m_listMsg.push_back(pPackBuf);

				VuThread::IF()->leaveCriticalSection(m_hRecvLock);


/*
				STNetMsgHeader* pNetHeader = (STNetMsgHeader*)(szStart);
				unsigned short wMsgId = ntohs(pNetHeader->wMsgID);
				unsigned int dwDataLen = ntohl(pNetHeader->dwDataLen);*/

				szStart += nUsed;

				m_nUsedPackBufLen -= nUsed;


			}

		}
		else
		{
			return;
		}

	}

	if (m_nUsedPackBufLen > 0 && (szStart != m_szPackBuf))
	{
		memmove(m_szPackBuf, szStart, m_nUsedPackBufLen);
	}

}

bool VuNetManager::isConnected()
{
	return m_bIsConnected;
}



