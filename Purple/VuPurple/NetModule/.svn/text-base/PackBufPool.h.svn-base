/********************************************************************
	created:	2013-11-26   17:25
	filename: 	PackBufPool.h
	author:		xlm, foreverxuliming@gmail.com	
	purpose:	

*********************************************************************/
#ifndef __PACK_BUF_POOL_H__
#define __PACK_BUF_POOL_H__

#include <list>
using namespace std;
class PackBuf;
class ILock;

class PackBufPool
{
public:
    PackBufPool(int nInitSize = 5);
    ~PackBufPool();
	/*
	@ Author:	xlm(2013-11-26 17:26)
	@ Parameter:
	@ Returns:
	@ Description: 用完之后一定要使用FreePackBuf回收
	*/
    PackBuf* GetPackBuf();
	/*
	@ Author:	xlm(2013-11-26 17:26)
	@ Parameter:
	@ Returns:
	@ Description: 回收PackBuf
	*/
    void FreePackBuf(PackBuf* pBuf);
private:
typedef std::list<PackBuf*> PackBufList;
    PackBufList         m_listPackBuf;
  //  ILock*              m_pLock;
	VUHANDLE			m_hLock;
};


#endif // !__PACK_BUF_POOL_H__
