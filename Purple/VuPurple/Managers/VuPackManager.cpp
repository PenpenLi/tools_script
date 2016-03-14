//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Advertising Manager
// 
//*****************************************************************************

#include "VuPurple/Managers/VuPackManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuPurple/Util/CommonUtility.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuPurple/Managers/VuStatsManager.h"

#define Pack_LastTime (30)


IMPLEMENT_SYSTEM_COMPONENT(VuPackManager, VuPackManager);
//*****************************************************************************
VuPackManager::VuPackManager()
{
	//std::time_t rawTime;
	//std::time(&rawTime);

	//if (m_LastLanXiangTime == 0)
	//{
	//	m_LastLanXiangTime = rawTime;
	//	m_LastSuperVipTime = rawTime;
	//}

	m_LastLanXiangTime = VuStatsManager::IF()->getIntStat("LastLanXiangTime");
	m_LastSuperVipTime = VuStatsManager::IF()->getIntStat("LastSuperVipTime");
}

//*****************************************************************************
bool VuPackManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuPackManager::tick, "Decision");

	return true;
}

//*****************************************************************************
void VuPackManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
}


//*****************************************************************************
void VuPackManager::tick(float fdt)
{
	fdt = VuTickManager::IF()->getRealDeltaTime();

	//mCheckTimer -= fdt;
	//if ( mCheckTimer <= 0 )
	//	checkForAds();
}

void VuPackManager::triggerLanXiangPack()
{
	//if sameday; do not change
	std::time_t rawTime;
	std::time(&rawTime);
	

	if (m_LastLanXiangTime == 0)
	{
		m_LastLanXiangTime = rawTime;
		VuStatsManager::IF()->addToIntStat("LastLanXiangTime", m_LastLanXiangTime);
		return;
	}

	if (IsSameDay(rawTime, m_LastLanXiangTime))
	{
		;
	}
	else
	{
		m_LastLanXiangTime = rawTime;
		VuStatsManager::IF()->addToIntStat("LastLanXiangTime", m_LastLanXiangTime);
	}
}
void VuPackManager::triggerSuperVipPack()
{
	std::time_t rawTime;
	std::time(&rawTime);


	if (m_LastSuperVipTime == 0)
	{
		m_LastSuperVipTime = rawTime;
		VuStatsManager::IF()->addToIntStat("LastSuperVipTime", m_LastLanXiangTime);
		return;
	}

	if (IsSameDay(rawTime, m_LastSuperVipTime))
	{
		;
	}
	else
	{
		m_LastSuperVipTime = rawTime;
		VuStatsManager::IF()->addToIntStat("LastSuperVipTime", m_LastLanXiangTime);
	}
}

VUINT VuPackManager::getLanXiangDiscountTime()
{
	std::time_t rawTime;
	std::time(&rawTime);

	if (rawTime <= m_LastLanXiangTime)
	{
		return 0;
	}
	else
	{
		VUINT iTime = rawTime - m_LastLanXiangTime;
		
		int lastTime = 0;
		if (iTime >= Pack_LastTime * 60 || iTime < 0)
		{
			;
		}
		else
		{
			lastTime = Pack_LastTime * 60 - iTime;
		}
		
		return lastTime;
	}
}

VUINT VuPackManager::getSuperVipDiscountTime()
{
	std::time_t rawTime;
	std::time(&rawTime);

	if (rawTime <= m_LastSuperVipTime)
	{
		return 0;
	}
	else
	{
		VUINT iTime = rawTime - m_LastSuperVipTime;

		int lastTime = 0;
		if (iTime >= Pack_LastTime * 60 || iTime < 0)
		{
			;
		}
		else
		{
			lastTime = Pack_LastTime * 60 - iTime;
		}
		
		return lastTime;
	}
}

int VuPackManager::getPackType()
{
	return ePackUnit(0);
}

bool VuPackManager::isPackOk(ePackUnit unit)
{
	std::time_t rawTime;
	std::time(&rawTime);
	if (unit == Pack_LanXiang)
	{
		if (VuBillingManager::IF()->isOwned("MonsterTruckPack"))
		{
			return false;
		}
		VUINT iTime = rawTime - m_LastLanXiangTime;

		if (iTime > Pack_LastTime * 60 || iTime < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (unit = Pack_SuperVip)
	{
		if (VuBillingManager::IF()->isOwned("SuperVipPack"))
		{
			return false;
		}

		VUINT iTime = rawTime - m_LastSuperVipTime;

	    if (iTime > Pack_LastTime * 60 || iTime < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}
