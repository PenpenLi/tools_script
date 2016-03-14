/*!
 * \file PackHandler.h
 * \date 2015/02/02 18:04
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief : handler the pack
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __PACK_HANDLER_H__
#define __PACK_HANDLER_H__


#include <map>

using namespace std;


class PackHandler
{

public:
	PackHandler();
	~PackHandler();


	bool Init();

	void OnRecv(const char* pBuf, unsigned int dwLen);

private:
	/*
	@ Author:	xlm(2015-2-3 19:01)
	@ Parameter:
	@ Returns:
	@ Description: 获取服务器时间的应答包
	*/
	void __GetServerTimeAck(const char* szBuf, unsigned int dwLen);
	/*
	@ Author:	xlm(2015-2-5 19:49)
	@ Parameter:
	@ Returns:
	@ Description:获取是否显示立即领取的应答包
	*/
	void __GetShowImmeButtonTipAck(const char* szBuf, unsigned int dwLen);
	

private:
	typedef void (PackHandler::*FN_NET_MESSAGE)(const char* szBuf, unsigned int dwLen);
	//typedef hash_map<unsigned short, FN_NET_MESSAGE > NetMessageMap;
	typedef map<unsigned short, FN_NET_MESSAGE > NetMessageMap;

	NetMessageMap						m_mapNetMessage;

};

#endif