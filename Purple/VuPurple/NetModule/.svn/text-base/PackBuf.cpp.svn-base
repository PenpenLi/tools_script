#include "PackBuf.h"
#include "string.h"

PackBuf::PackBuf() : m_nLength(MAX_PACKET_LEN), m_nUsedLength(0)
{
    memset(m_szBuf, 0, MAX_PACKET_LEN);
}

PackBuf::~PackBuf()
{

}

/*
@ Author:	xlm(2013-11-26 17:13)
@ Parameter:
@ Returns:
@ Description:得到使用长度
*/
/*
int  PackBuf::GetUsedLen()
{
    return m_nUsedLength;
}
*/


/*
@ Author:	xlm(2013-11-26 17:22)
@ Parameter:
@ Returns:
@ Description:设置数据
*/
int PackBuf::SetData(const char* szData, int nLen)
{
    if (nLen > m_nLength)
    {
        return -1;
    }
    memcpy(m_szBuf, szData, nLen);
    m_nUsedLength = nLen;
    return m_nUsedLength;
}


/*
@ Author:	xlm(2013-11-26 17:14)
@ Parameter: nBufLen指定szBuf的长度
@ Returns:
@ Description:取得数据的长度，返回的是真正使用的buf中的数据长度，如果返回值大于nBufLen,则说明所给的buf不够长
*/
int PackBuf::GetData(char* szBuf, int nBufLen)
{
    if (nBufLen>=m_nUsedLength)
    {
        memcpy(szBuf, m_szBuf, m_nUsedLength);
    }
    return m_nUsedLength;
}

/*
@ Author:	xlm(2013-11-26 17:17)
@ Parameter:
@ Returns:
@ Description:
*/
void PackBuf::Reset()
{
    memset(m_szBuf, 0, MAX_PACKET_LEN);
    m_nUsedLength = 0;
}