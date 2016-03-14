//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Toast entities
// 
//*****************************************************************************

#include "VuGameImageEntity.h"
#include "VuEngine/Managers/VuToastManager.h"
#include "VuEngine/Assets/VuTextureAsset.h"


//*****************************************************************************
// VuUIToastImageEntity
//*****************************************************************************

class VuGameToastImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	virtual VuTexture *getTexture();
};

IMPLEMENT_RTTI(VuGameToastImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameToastImageEntity);


//*****************************************************************************
VuTexture *VuGameToastImageEntity::getTexture()
{
	if ( VuToastManager::IF() )
	{
		VuToast *pToast = VuToastManager::IF()->getActiveToast();
		if ( pToast && pToast->mpImage )
			return pToast->mpImage->getTexture();
	}

	return VUNULL;
}
