//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarEffectController class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"

class VuEntity;
class VuCarEntity;
class VuCarEffect;
class VuJsonContainer;
class VuGfxDrawParams;

class VuCarEffectController
{
public:
	VuCarEffectController(VuCarEntity *pCar);
	~VuCarEffectController();

	void			onGameInitialize();
	void			onGameRelease();

	void			onTickDecision(float fdt);
	void			onApplyForces(float fdt);

	void			draw(const VuGfxDrawParams &params);

	void			applyEffect(const char *effect, VuEntity *pOriginator);
	void			stopAllEffects();

	typedef std::map<std::string, VuCarEffect *> ActiveEffects;
	const ActiveEffects	&getActiveEffects() { return mActiveEffects; }

	// sub-effects
	enum eSubEffect { OILY_TIRES, BOOST, SHIELD, TOUGHNESS, NITRO, TRACTION, TORNADO, FREEZE, ON_FIRE, CONFUSION, LOW_GRAVITY, GHOST, SUB_EFFECT_COUNT };
	VUUINT32		isActive(eSubEffect subEffect) { return mSubEffects[subEffect]; }

	void			pushSubEffect(eSubEffect subEffect) { mSubEffects[subEffect]++; }
	void			popSubEffect(eSubEffect subEffect) { mSubEffects[subEffect]--; }

	void			pushWheelSurfaceOverride(const char *surfaceType);
	void			popWheelSurfaceOverride();

	void				pushToughness(const char *smashCarEffect);
	void				popToughness();
	const std::string	&getToughnessSmashCarEffect() { return mToughnessSmashCarEffect; }

	void			pushTraction();
	void			popTraction();


private:
	typedef VuCarEffect *(*CreateEffectFn)();
	typedef std::map<std::string, CreateEffectFn> EffectCreators;
	EffectCreators	mEffectCreators;

	VuCarEntity		*mpCar;

	ActiveEffects	mActiveEffects;

	int				mSubEffects[SUB_EFFECT_COUNT];
	int				mWheelSurfaceOverrideCount;
	std::string		mToughnessSmashCarEffect;
};

