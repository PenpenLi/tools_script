/*!
 * \file PacketParser.h
 * \date 2015/02/02 17:08
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief : for parse the packet 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __PACKET_PARSER_H__
#define __PACKET_PARSER_H__


class PacketParser
{
public:
	/*!
 	* \author: xlm 2015/2/2  
 	* \brief: do parse
 	*/
	static int ParsePacket(const char* pBuf, unsigned int dwLen);

};


#endif