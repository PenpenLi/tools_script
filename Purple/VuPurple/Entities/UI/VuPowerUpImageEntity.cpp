//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuPowerUpImageEntity
// 
//*****************************************************************************

#include "VuGameImageEntity.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuTextureAsset.h"


class VuPowerUpImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

protected:
	virtual VuTexture	*getTexture();
};

IMPLEMENT_RTTI(VuPowerUpImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPowerUpImageEntity);


//*****************************************************************************
VuTexture *VuPowerUpImageEntity::getTexture()
{
	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();

	if ( const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUpByName(powerUp.c_str()) )
		return pPowerUp->mpIconTextureAsset->getTexture();

	return VUNULL;
}
