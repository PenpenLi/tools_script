#include "PackBufPool.h"
/*#include "VuPurple/Util/linux_lock.h"*/

#include "PackBuf.h"
#include "VuEngine/HAL/Thread/VuThread.h"



PackBufPool::PackBufPool(int nInitSize )
{
   for (int i=0; i<nInitSize; i++)
   {
       m_listPackBuf.push_back(new PackBuf);
   }
   m_hLock = VuThread::IF()->createCriticalSection();
  /* CSLock* pLock = new CSLock;
   m_pLock = (ILock*)pLock;*/
}

PackBufPool::~PackBufPool()
{
    PackBufList::iterator it = m_listPackBuf.begin();
    while (it != m_listPackBuf.end())
    {
        delete *it;
        ++it;
    }
    m_listPackBuf.clear();
	if (m_hLock)
	{
		VuThread::IF()->deleteCriticalSection(m_hLock);
		m_hLock = NULL;
	}
   /* if (m_pLock)
    {
        delete m_pLock;
        m_pLock = NULL;
    }*/

}

/*
@ Author:	xlm(2013-11-26 17:26)
@ Parameter:
@ Returns:
@ Description:
*/
PackBuf* PackBufPool::GetPackBuf()
{
    if (!m_listPackBuf.empty())
    {
		VuThread::IF()->enterCriticalSection(m_hLock);
        //m_pLock->Lock();
        PackBuf* pBuf =  m_listPackBuf.back();
        m_listPackBuf.pop_back();
       // m_pLock->UnLock();
		VuThread::IF()->leaveCriticalSection(m_hLock);
        return pBuf;
    }
    PackBuf* pNewBuf = new PackBuf;
   /* if (pNewBuf)
    {
       m_pLock->Lock();
       m_listPackBuf.push_back(pNewBuf);
       m_pLock->UnLock();
       
    }*/
    return pNewBuf;
}

/*
@ Author:	xlm(2013-11-26 17:26)
@ Parameter:
@ Returns:
@ Description:
*/
void PackBufPool::FreePackBuf(PackBuf* pBuf)
{
    if (pBuf)
    {
        pBuf->Reset();
		VuThread::IF()->enterCriticalSection(m_hLock);
       // m_pLock->Lock();
        m_listPackBuf.push_back(pBuf);
		VuThread::IF()->leaveCriticalSection(m_hLock);
        //m_pLock->UnLock();
    }
}