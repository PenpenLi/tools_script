//*****************************************************************************
//
//  Copyright (c) 2012-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuGameSceneEntity
// 
//*****************************************************************************

#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Properties/VuBasicProperty.h"


class VuGameSceneEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameSceneEntity();

	virtual void	load(const VuJsonContainer &data);

private:
	bool	mEnableForLowComplexity;
	bool	mEnableForMediumComplexity;
	bool	mEnableForHighComplexity;
};


IMPLEMENT_RTTI(VuGameSceneEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameSceneEntity);


//*****************************************************************************
VuGameSceneEntity::VuGameSceneEntity():
	VuEntity(CAN_HAVE_CHILDREN),
	mEnableForLowComplexity(true),
	mEnableForMediumComplexity(true),
	mEnableForHighComplexity(true)
{
	// properties
	addProperty(new VuBoolProperty("Enable for Low", mEnableForLowComplexity));
	addProperty(new VuBoolProperty("Enable for Medium", mEnableForMediumComplexity));
	addProperty(new VuBoolProperty("Enable for High", mEnableForHighComplexity));
}

//*****************************************************************************
void VuGameSceneEntity::load(const VuJsonContainer &data)
{
	if ( mpTemplateAsset )
	{
		loadTemplated(data);
	}
	else
	{
		mProperties.load(data["Properties"]);
		mComponents.load(data["Components"]);

		VUUINT32 mask = (int(mEnableForLowComplexity)<<0) | ((int)mEnableForMediumComplexity<<1) | ((int)mEnableForHighComplexity<<2);
		if ( mask & (1 << VuGameUtil::IF()->getSceneComplexity()) )
			loadChildEntities(data["ChildEntities"]);

		onLoad(data);
	}
}
