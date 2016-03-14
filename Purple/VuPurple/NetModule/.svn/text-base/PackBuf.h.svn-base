/********************************************************************
	created:	2013-11-26   17:08
	filename: 	PackBuf.h
	author:		xlm, foreverxuliming@gmail.com	
	purpose:	

*********************************************************************/
#ifndef __PACK_BUF_H__
#define __PACK_BUF_H__

#include "NetBaseDef.h"

class PackBuf
{
public:
       friend class PackBufPool;
	/*
	@ Author:	xlm(2013-11-26 17:13)
	@ Parameter:
	@ Returns:
	@ Description:得到使用长度
	*/
    inline int  GetUsedLen() {return m_nUsedLength;}

	/*
	@ Author:	xlm(2013-11-26 17:22)
	@ Parameter:
	@ Returns:
	@ Description:设置数据
	*/
    int SetData(const char* szData, int nLen);

	/*
	@ Author:	xlm(2013-11-26 17:14)
	@ Parameter: nBufLen指定szBuf的长度
	@ Returns:
	@ Description:取得数据的长度，返回的是真正使用的buf中的数据长度，如果返回值大于nBufLen,则说明所给的buf不够长
	*/
    int GetData(char* szBuf, int nBufLen);

	/*
	@ Author:	xlm(2013-11-27 9:47)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
    inline const char* GetBuf() {return m_szBuf;}

	/*
	@ Author:	xlm(2013-11-26 17:17)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
    void Reset();

protected:
    PackBuf();
    ~PackBuf();

private:

   char     m_szBuf[MAX_PACKET_LEN];      //缓冲区
   int      m_nUsedLength;  //使用的长度
   int      m_nLength;     //总长度

};


#endif // __PACK_BUF_H__
