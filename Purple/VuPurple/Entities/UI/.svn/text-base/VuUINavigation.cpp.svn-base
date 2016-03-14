//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI Navigation Entities
// 
//*****************************************************************************

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/UI/VuUI.h"


//*****************************************************************************
// EventType
//*****************************************************************************
class VuUINavigationNodeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuUINavigationNodeEntity();

private:
	VuRetVal			Reset(const VuParams &params);
	VuRetVal			FromAbove(const VuParams &params);
	VuRetVal			FromBelow(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	VuEntity			*mpAboveEntity;
	VuEntity			*mpBelowEntity;
};

IMPLEMENT_RTTI(VuUINavigationNodeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuUINavigationNodeEntity);


//*****************************************************************************
VuUINavigationNodeEntity::VuUINavigationNodeEntity():
	mpAboveEntity(VUNULL),
	mpBelowEntity(VUNULL)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUINavigationNodeEntity, FromAbove);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUINavigationNodeEntity, FromBelow);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUINavigationNodeEntity, Reset);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, DefaultAbove);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, DefaultBelow);
}

//*****************************************************************************
VuRetVal VuUINavigationNodeEntity::FromAbove(const VuParams &params)
{
	mpAboveEntity = VuUI::IF()->getFocus();

	if ( mpBelowEntity )
	{
		VuScriptPlug *pPlug = mpBelowEntity->getComponent<VuScriptComponent>()->getPlug("SetFocus");
		if ( pPlug && pPlug->isInput() )
		{
			pPlug->execute();
			return VuRetVal();
		}
	}

	mpScriptComponent->getPlug("DefaultBelow")->execute();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUINavigationNodeEntity::FromBelow(const VuParams &params)
{
	mpBelowEntity = VuUI::IF()->getFocus();

	if ( mpAboveEntity )
	{
		VuScriptPlug *pPlug = mpAboveEntity->getComponent<VuScriptComponent>()->getPlug("SetFocus");
		if ( pPlug && pPlug->isInput() )
		{
			pPlug->execute();
			return VuRetVal();
		}
	}

	mpScriptComponent->getPlug("DefaultAbove")->execute();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUINavigationNodeEntity::Reset(const VuParams &params)
{
	mpAboveEntity = VUNULL;
	mpBelowEntity = VUNULL;
	return VuRetVal();
}
