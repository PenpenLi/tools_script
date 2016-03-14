/*!
 * \file VuNetManager.h
 * \date 2015/01/28 14:37
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief : i just add the network mode 
 *
 * TODO: long description
 *
 * \note
*/



#ifndef __VU_NET_MANAGER_H__
#define __VU_NET_MANAGER_H__



#include "VuEngine/VuSystemComponent.h"

#include "VuPurple/NetModule/NetBaseDef.h"

class VuTcpSocket;

class PackBufPool;
class PackBuf;
class ILock;

class PackHandler;


class VuNetManager : public VuSystemComponent
{
public:

	DECLARE_SYSTEM_COMPONENT(VuNetManager)



public:
	VuNetManager();
	virtual ~VuNetManager();

	/*!
	* \author: xlm 2015/1/28
	* \brief: connect to a ip and port
	*/

	virtual bool connect(const char* szIp, unsigned short uPort, int nTimeoutMs = 1000);

	virtual void disConnect();

	virtual int send(const char* szData, int nDataSize);
	virtual int recv(char* szData, int nBufSize);


	/*!
	* \author: xlm 2015/1/28
	* \brief: set block or nonblock
	*/
	virtual bool setNonBlocking(bool nonBlock);
	/*!
	* \author: xlm 2015/1/28
	* \brief: set the timeout of send and recv
	*/
	virtual bool setTimeOut(int nRecvMS, int nSendMs);

	/*
						VuSystemComponent Interface
	*/

	/*
	@ Author:	xlm(2015-2-3 16:51)
	@ Parameter:
	@ Returns:
	@ Description:处理网络收报
	*/

	void NetProcess();

	virtual bool isConnected();

	void SetThreadHandleNULL();





	


protected:
	// called by game
	friend class VuPurple;

	/*!
 	* \author: xlm 2015/1/28  
 	* \brief: you should override it in the child class, and construct a VuTcpSocket or it'll not work
 	*/
	virtual bool	init();

	virtual void	postInit();
	virtual void	preRelease();
	virtual void	release();

	void				tick(float fdt);

private:

	/*
	@ Author:	xlm(2015-2-3 10:58)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	void __ParsePack(const char* szBuf, int nLen);






protected:

	VuTcpSocket*		m_pTcpSocket;


	char		m_szPackBuf[MAX_PACKET_LEN];
	int			m_nUsedPackBufLen;

	PackBufPool*	m_pPackBufPool;
	typedef std::list<PackBuf*> MsgList;  //
	MsgList              m_listMsg;

	VUHANDLE			m_hThread;

	PackHandler*		m_pPackHandler;

	bool		m_bIsConnected;
	VUHANDLE			m_hRecvLock;






};


#endif