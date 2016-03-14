//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Out of Bounds Manager
// 
//*****************************************************************************

#include "VuOutOfBoundsManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuOutOfBoundsManager, VuOutOfBoundsManager);


//*****************************************************************************
void VuOutOfBoundsManager::release()
{
	VUASSERT(mZones.size() == 0, "VuOutOfBoundsManager zone leak!");
}

//*****************************************************************************
bool VuOutOfBoundsManager::isOutOfBounds(const VuVector3 &position, float &recoverTime)
{
	for ( Zones::const_iterator iter = mZones.begin(); iter != mZones.end(); iter++ )
	{
		if ( VuDistSquared(iter->mPosition, position) < iter->mSquaredRadius )
		{
			if ( iter->mpIF->isOutOfBounds(position) )
			{
				recoverTime = iter->mRecoverTime;
				return true;
			}
		}
	}

	return false;
}

//*****************************************************************************
void VuOutOfBoundsManager::addZone(const VuOutOfBoundsIF *pIF, const VuVector3 &position, float radius, float recoverTime)
{
	Zone zone;

	zone.mPosition = position;
	zone.mSquaredRadius = radius*radius;
	zone.mRecoverTime = recoverTime;
	zone.mpIF = pIF;

	mZones.push_back(zone);
}

//*****************************************************************************
void VuOutOfBoundsManager::removeZone(const VuOutOfBoundsIF *pIF)
{
	for ( Zones::iterator iter = mZones.begin(); iter != mZones.end(); iter++ )
	{
		if ( iter->mpIF == pIF )
		{
			mZones.erase(iter);
			break;
		}
	}
}
