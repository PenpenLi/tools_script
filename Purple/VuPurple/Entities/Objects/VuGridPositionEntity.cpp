//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Grid position entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"


class VuGridPositionEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGridPositionEntity();

	void				onGameInitialize();
	void				onGameRelease();

private:
	enum { STANDARD, BOOST_BLITZ };

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;

	// properties
	int						mType;
};


IMPLEMENT_RTTI(VuGridPositionEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGridPositionEntity);


//*****************************************************************************
VuGridPositionEntity::VuGridPositionEntity():
	mType(STANDARD)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);

	// properties
	static VuStaticIntEnumProperty::Choice typeChoices[] =
	{
		{ "Standard", STANDARD },
		{ "Boost Blitz", BOOST_BLITZ },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, typeChoices));
}

//*****************************************************************************
void VuGridPositionEntity::onGameInitialize()
{
	if ( mType == STANDARD )
		VuCarManager::IF()->addGridPosition(this);
	else if ( mType == BOOST_BLITZ )
		VuCarManager::IF()->setBoostBlitzStart(this);
}

//*****************************************************************************
void VuGridPositionEntity::onGameRelease()
{
	if ( mType == STANDARD )
		VuCarManager::IF()->removeGridPosition(this);
	else if ( mType == BOOST_BLITZ )
		VuCarManager::IF()->setBoostBlitzStart(VUNULL);
}
