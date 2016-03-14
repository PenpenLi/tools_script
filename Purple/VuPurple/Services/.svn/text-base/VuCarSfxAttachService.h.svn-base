//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Sfx Attach Service
// 
//*****************************************************************************

#include "VuEngine/Services/VuService.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"

class VuCarEntity;


#if !VU_DISABLE_AUDIO

class VuCarSfxAttachService : public VuService
{
public:
	VuCarSfxAttachService();
	~VuCarSfxAttachService() {}

	void			init(VuCarEntity *pParent, FMOD::Event *pEvent);
	virtual bool	tick(float fdt);

private:
	static FMOD_RESULT F_CALLBACK	staticCallback(FMOD_EVENT *event, FMOD_EVENT_CALLBACKTYPE type, void *param1, void *param2, void *userdata);
	FMOD_RESULT						callback(FMOD_EVENT_CALLBACKTYPE type, void *param1, void *param2, void *userdata);

	VuWeakRef<VuCarEntity>	mParent;
	FMOD::Event				*mpEvent;
};

#endif // VU_DISABLE_AUDIO
