#include "PacketParser.h"
#include "NetBaseDef.h"


#if defined VUANDROID || defined VUIOS

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

int PacketParser::ParsePacket(const char* pBuf, unsigned int dwLen)
{
	if (dwLen < gnNetMsgHeaderSize)
	{
		return 0;
	}

	STNetMsgHeader* pNetMsgHeader = (STNetMsgHeader*)pBuf;
	unsigned int nPkgLen = ntohl(pNetMsgHeader->dwDataLen) + gnNetMsgHeaderSize;
	if (nPkgLen < 0) //出错了，不可能发生这样的事，如果发生，则证明网络出问题，应该关闭了
	{
		return -1;
	}
	if (nPkgLen <= dwLen)
	{
		return nPkgLen;
	}


	return 0;
}
