/*!
 * \file NetBaseDef.h
 * \date 2015/02/02 17:11
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief : the base defination of the net component
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __NET_BASE_DEF_H__
#define __NET_BASE_DEF_H__


#define MAX_PACKET_LEN    10240



struct STNetMsgHeader
{

	unsigned short wMsgID; //包头表示
	unsigned int	dwDataLen; //数据包长度
};

const unsigned int gnNetMsgHeaderSize = sizeof(STNetMsgHeader);

#endif