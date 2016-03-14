//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Behavior Class
// 
//*****************************************************************************

#pragma once

//*****************************************************************************
#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Objects/VuRefObj.h"
#include "VuEngine/Method/VuParams.h"
#include "VuPurple/AI/VuAiInstance.h"

class VuAiInstance;

//*****************************************************************************
class VuAiBehavior
{
public:
	VuAiBehavior() : mpAiInstance(VUNULL), mScore(0.0f), mIsExpired(false), mIsDone(false), mCountdownTimer(0.0f), mTuningWeight(1.0f) {}

	virtual ~VuAiBehavior() {}

	virtual void		init(VuAiInstance* pInstance);

	virtual void		cleanup() { mpAiInstance = VUNULL; }

	// Scoring function so this behavior can rate its applicability to the current situation
	virtual float		score() = 0;

	// Called to "start" the Behaviors (starts reaction time timer)
	virtual void		startReacting();

	// Do any initialize on behavior start, such as setting the timer value or performing
	// game actions of some sort
	virtual bool		start();

	// Called every tick for the behavior to actively adjust the gameplay. Default 
	// implementation is to tick the timer and update expiration flags. Make sure
	// derived classes call VuAiBehavior::tick(fdt) before doing processing.
	// Returns FALSE if the timer has expired, and TRUE if it hasn't
	virtual bool		tick(float fdt);

	// Do any cleanup on behavior stop
	virtual bool		stop() { mIsExpired = true; mIsDone = true; mCountdownTimer = 0.0f; mReactionTime = 0.0f; return true; }

	// Accessors
	bool				isFinished() { return mIsExpired || mIsDone; }

	const std::string&	getName() { return mName;  }
	void				setName(const char* pName) { mName = pName; }

	const std::string&	getClassName() { return mClassName; }
	void				setClassName(const char* pName) { mClassName = pName; }

	VuParams&			getParams() { return mParams; }

	// Used to manually override the tuning weight. Must be called on the Behavior AFTER it has
	// been created in the factory
	void				overrideTuningWeight(const float weight) { mTuningWeight = weight; }

	// For changing reaction time between creation and when it's actually started
	void				overrideReactionTime(const float seconds) { mReactionTime = seconds;  }

protected:
	VuWeakRef<VuAiInstance> mpAiInstance;

	float				mScore;

	bool				mIsExpired;
	bool				mIsDone;
	bool				mIsReacting;

	float				mReactionTime;
	float				mCountdownTimer;
	float				mTimeInBehavior;

	// Resulting weight from tuning
	float				mTuningWeight;
	
	// For passing variables/values to a Behavior
	VuParams			mParams;

	// Name of Behavior
	std::string			mName;

	// "Class" of Behavior (maps to C++ class)
	std::string			mClassName;
};

//*****************************************************************************

class VuAiBehaviorFactory : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAiBehaviorFactory);

public:
	VuAiBehaviorFactory();

	typedef VuAiBehavior*	(*BehaviorCreateFunction)();

	typedef std::map<std::string, BehaviorCreateFunction> BehaviorCreatorMap;

	virtual bool			init();

	VuAiBehavior*			create(const std::string& name);
	void					destroy(VuAiBehavior* pBehavior);

	void					registerBehavior(const std::string& name, BehaviorCreateFunction function);

	// Map from string name of Behavior to Behavior Allocation Function
	BehaviorCreatorMap		mBehaviors;
};

#define REGISTER_BEHAVIOR(behavior_name_, behavior_class_)												\
{																										\
	extern VuAiBehavior *CreateVuAiBehavior##behavior_class_();											\
	registerBehavior(#behavior_name_, CreateVuAiBehavior##behavior_class_);								\
}

#define IMPLEMENT_BEHAVIOR_CREATE(behavior_class_)														\
	VuAiBehavior* CreateVuAiBehavior##behavior_class_() { VuAiBehavior* pNewBehavior = new VuAiBehavior##behavior_class_(); if (pNewBehavior) pNewBehavior->setClassName(#behavior_class_); return pNewBehavior; }

#define DECLARE_BEHAVIOR_CLASS(behavior_class_) class VuAiBehavior##behavior_class_ : public VuAiBehavior { public: virtual float score(); virtual bool start();};
#define DECLARE_BEHAVIOR_CLASS_WITH_TICK(behavior_class_) class VuAiBehavior##behavior_class_ : public VuAiBehavior { public: virtual bool tick(float fdt); virtual float score(); virtual bool start();};
#define DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(behavior_class_) class VuAiBehavior##behavior_class_ : public VuAiBehavior { public: virtual bool tick(float fdt); virtual float score(); virtual bool start(); virtual bool stop();};

// PowerUp Behaviors
DECLARE_BEHAVIOR_CLASS(UsePowerUp);

DECLARE_BEHAVIOR_CLASS_WITH_TICK(PowerUpThrow);
DECLARE_BEHAVIOR_CLASS_WITH_TICK(PowerUpSeek);
DECLARE_BEHAVIOR_CLASS(PowerUpDropped);
DECLARE_BEHAVIOR_CLASS(PowerUpGlobal);
DECLARE_BEHAVIOR_CLASS(PowerUpLongShot);
DECLARE_BEHAVIOR_CLASS_WITH_TICK(PowerUpShield);
DECLARE_BEHAVIOR_CLASS(PowerUpToughness);

// 
DECLARE_BEHAVIOR_CLASS(SpikedTires);
DECLARE_BEHAVIOR_CLASS(Boost);
DECLARE_BEHAVIOR_CLASS(PowerSlide);

// Character ability Behaviors
DECLARE_BEHAVIOR_CLASS_WITH_TICK(CharAbility);

// Driving Control Behaviors
DECLARE_BEHAVIOR_CLASS_WITH_TICK(DriveLazy);
DECLARE_BEHAVIOR_CLASS_WITH_TICK(LoseControl);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(BrakeForCorner);
DECLARE_BEHAVIOR_CLASS_WITH_TICK(LaneChange);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(GetInFrontOfRacer);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(GetInBackOfRacer);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(SlowDownForPack);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(SpeedUpForPack);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(MaintainMaxLead);
DECLARE_BEHAVIOR_CLASS_WITH_TICK_AND_STOP(HandlingBoost);
