//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Brain Class
// 
//*****************************************************************************

#pragma once

#include "VuPurple/AI/VuAiBehavior.h"

class VuAiInstance;

//*****************************************************************************
//
// generatePossibleBehaviors() flags for specializing behavior
//
typedef VUUINT32 eInclusionFlags;
const eInclusionFlags INCLUDE_POWERUPS = 1 << 0;
const eInclusionFlags INCLUDE_DEBUFFS = 1 << 1;
const eInclusionFlags INCLUDE_AVOIDANCE = 1 << 2;
const eInclusionFlags INCLUDE_BOOST = 1 << 3;
const eInclusionFlags INCLUDE_CORNER_BRAKING = 1 << 4;
const eInclusionFlags INCLUDE_SPIKED_TIRES = 1 << 5;
const eInclusionFlags INCLUDE_POWERSLIDE = 1 << 6;
const eInclusionFlags INCLUDE_RACE_POSITIONING = 1 << 7;
const eInclusionFlags INCLUDE_MAX_LEAD = 1 << 8;
const eInclusionFlags INCLUDE_ALL= 0xFFFFFFFF;

//*****************************************************************************
// Base Class for Brains
//
// Different Brains interpret the world differently
//
class VuAiBrain
{
public:
	typedef std::vector<VuAiBehavior*> BehaviorList;

	VuAiBrain()  {}
	virtual ~VuAiBrain() { release(); }

	virtual void			init(VuAiInstance* pInstance) { mpAiInstance = pInstance; mPossibleBehaviors.clear(); mActiveBehaviors.clear(); }
	virtual void			release() { clearList(mPossibleBehaviors); clearList(mActiveBehaviors); }

	virtual void			refreshSensors() = 0;

	// Create any behaviors that this Brain likes to create and that are 
	// applicable
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL) = 0;

	// Sort behaviors based on the scores they provide
	virtual float			sort();

	// Select the highest priority behavior
	virtual VuAiBehavior*	select();

	// Do think cycle: generate, sort and select the best Behavior
	virtual void			think();

	// Execute current Active behaviors during the tickDecision() phase 
	virtual void			tickBehaviors(float fdt);

	// Iterate through active behaviors and remove ones that don't matter any more
	virtual void			expireActiveBehaviors();

	// Determine whether this Brain has a named Behavior as active
	virtual bool			hasActiveBehavior(const std::string& name);

	// Utility function for BehaviorList reset and deallocate
	void					clearList(BehaviorList &list);
	VuAiBehavior*			addPossibleBehavior(const std::string& name);

	const std::string&		getAiBrainName() { return mAiBrainName; }
	void					setAiBrainName(const std::string& name) { mAiBrainName = name; }

protected:
	static bool				behaviorSortFunction(VuAiBehavior* pOne, VuAiBehavior* pTwo);

protected:
	VuWeakRef<VuAiInstance> mpAiInstance;

	BehaviorList			mPossibleBehaviors;
	BehaviorList			mActiveBehaviors;

	std::string				mAiBrainName;
};

//*****************************************************************************
// Brain Factory Class
//*****************************************************************************
class VuAiBrainFactory : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAiBrainFactory);

public:
	// Factory Types
	typedef VuAiBrain*	(*BrainCreateFunction)();
	typedef std::map<std::string, BrainCreateFunction> BrainCreatorMap;

	// Factory Functions
	VuAiBrainFactory();

	virtual bool			init();
	VuAiBrain*				create(const std::string& name);
	void					destroy(VuAiBrain* pBrain);
	void					registerBrain(const std::string& name, BrainCreateFunction function);

	// Map from string name of Brain to Brain Allocation Function
	BrainCreatorMap			mBrains;
};

// Factory Macros
#define REGISTER_BRAIN(BrainName__)																\
	extern VuAiBrain *CreateVuAiBrain##BrainName__();											\
	registerBrain(#BrainName__, CreateVuAiBrain##BrainName__);

#define IMPLEMENT_BRAIN_CREATE(BrainName__)														\
	VuAiBrain* CreateVuAiBrain##BrainName__() { return new VuAiBrain##BrainName__(); }


//*****************************************************************************
// VuAiBrainDefault
//*****************************************************************************
class VuAiBrainDefault : public VuAiBrain
{
public:
	// Functions used by all Brains
	virtual void			init(VuAiInstance* pInstance);
	virtual void			refreshSensors();
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL);

	// Functions for this Brain specifically
	virtual void			generateForPowerUpsByGroup(const std::string& groupName);
	virtual void			generateForPowerUpsAllPossible();
	virtual	void			generateForPowerUps();
	void					generateForAvoidance();
	void					generateForDebuffs();

	std::vector<const char*> mPowerUpSelectionList;
};

//*****************************************************************************
// VuAiBrainLazy
//*****************************************************************************
class VuAiBrainLazy : public VuAiBrain
{
	virtual void			refreshSensors() {}				// We just don't care
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL);
};

class VuAiBrainBossDefault : public VuAiBrainDefault
{
public:
	virtual void			init(VuAiInstance* pInstance)
	{
		VuAiBrainDefault::init(pInstance);

		// Wait 10 seconds before using a powerup
		pInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + 10.0f;
	}

	virtual void			refreshSensors() { VuAiBrainDefault::refreshSensors(); }
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL) { return VuAiBrainDefault::generatePossibleBehaviors(flags); }
};

#define DECLARE_BOSS_BRAIN_CLASS(__name__)					\
class VuAiBrain##__name__ : public VuAiBrainBossDefault		\
{															\
	virtual void			refreshSensors() {}				\
	void					generateForCharacterAbility();	\
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL);	\
};

#define DECLARE_BOSS_BRAIN_CLASS_WITH_INIT(__name__)		\
class VuAiBrain##__name__ : public VuAiBrainBossDefault		\
{															\
	virtual void			init(VuAiInstance* pInstance);	\
	virtual void			refreshSensors() {}				\
	void					generateForCharacterAbility();	\
	virtual int				generatePossibleBehaviors(eInclusionFlags flags = INCLUDE_ALL);	\
};

DECLARE_BOSS_BRAIN_CLASS(BossHula);
DECLARE_BOSS_BRAIN_CLASS(BossBeachBro);
DECLARE_BOSS_BRAIN_CLASS(BossDisco);
DECLARE_BOSS_BRAIN_CLASS_WITH_INIT(BossLucha);
DECLARE_BOSS_BRAIN_CLASS_WITH_INIT(BossTribal);
DECLARE_BOSS_BRAIN_CLASS_WITH_INIT(BossRoller);
DECLARE_BOSS_BRAIN_CLASS(BossAlien);
DECLARE_BOSS_BRAIN_CLASS_WITH_INIT(BossBunny);
DECLARE_BOSS_BRAIN_CLASS(BossMcSkelly);

