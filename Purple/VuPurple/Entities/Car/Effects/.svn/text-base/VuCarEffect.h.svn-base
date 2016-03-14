//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarEffect class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuEntity;
class VuCarEntity;
class VuGfxDrawParams;
class VuJsonContainer;
class VuPfxSystemInstance;


class VuCarEffect
{
public:
	VuCarEffect() : mpData(VUNULL), mpCar(VUNULL), mpOriginator(VUNULL), mTimeRemaining(0.0f), mLoopingPfx(0), mReapplyType("Replace"), mpSplatPfx(VUNULL) {}
	virtual ~VuCarEffect() {}

	void			start(const VuJsonContainer &data, const char *effectName);
	void			apply(const VuJsonContainer &data);
	void			stop();
	void			tick(float fdt);
	void			draw(const VuGfxDrawParams &params);

	virtual void	onStart(const VuJsonContainer &data, const char *effectName) {}
	virtual void	onApply(const VuJsonContainer &data) {}
	virtual void	onStop(const VuJsonContainer &data) {}
	virtual void	onTick(float fdt) {}
	virtual void	onApplyForces(float fdt) {}
	virtual void	onDraw(const VuGfxDrawParams &params) {}

	const VuJsonContainer	*mpData;

	VuCarEntity			*mpCar;
	VuCarEntity			*mpOriginator;
	float				mTimeRemaining;
	std::string			mLoopingSfxName;
	VuAudioEvent		mLoopingSfx;
	std::string			mLoopingPfxName;
	VUUINT32			mLoopingPfx;
	std::string			mSplatPfxName;
	float				mSoftKillTime;
	std::string			mReapplyType;
	VuPfxSystemInstance	*mpSplatPfx;
	float				mSplatPfxDist;
	bool				mSplatPfxVisible;
};

#define IMPLEMENT_CAR_EFFECT(className) \
	VuCarEffect *Create##className() { return new className; }
