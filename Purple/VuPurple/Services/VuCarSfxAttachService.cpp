//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Sfx Attach Service
// 
//*****************************************************************************

#include "VuCarSfxAttachService.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"


#if !VU_DISABLE_AUDIO

//*****************************************************************************
VuCarSfxAttachService::VuCarSfxAttachService():
	mpEvent(VUNULL)
{
}

//*****************************************************************************
void VuCarSfxAttachService::init(VuCarEntity *pParent, FMOD::Event *pEvent)
{
	mParent = pParent;
	mpEvent = pEvent;

	mpEvent->setCallback(staticCallback, this);

	tick(0.0f);
}

//*****************************************************************************
bool VuCarSfxAttachService::tick(float fdt)
{
	if ( mParent.get() && mpEvent )
	{
		const VuVector3 *pos = &mParent->getModelPosition();
		const VuVector3 *vel = &mParent->getLinearVelocity();

		mpEvent->set3DAttributes(reinterpret_cast<const FMOD_VECTOR *>(pos), reinterpret_cast<const FMOD_VECTOR *>(vel), VUNULL);

		return true;
	}

	if ( mpEvent )
		mpEvent->setCallback(0, 0);

	return false;
}

//*****************************************************************************
FMOD_RESULT F_CALLBACK VuCarSfxAttachService::staticCallback(FMOD_EVENT *event, FMOD_EVENT_CALLBACKTYPE type, void *param1, void *param2, void *userdata)
{
	return static_cast<VuCarSfxAttachService *>(userdata)->callback(type, param1, param2, userdata);
}

//*****************************************************************************
FMOD_RESULT VuCarSfxAttachService::callback(FMOD_EVENT_CALLBACKTYPE type, void *param1, void *param2, void *userdata)
{
	if ( type == FMOD_EVENT_CALLBACKTYPE_EVENTFINISHED )
		mpEvent = VUNULL;

	return FMOD_OK;
}

#endif