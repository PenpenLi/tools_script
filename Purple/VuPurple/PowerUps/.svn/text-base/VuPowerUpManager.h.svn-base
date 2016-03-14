//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUpManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Math/VuVector2.h"

class VuPowerUpGroup;
class VuTextureAsset;
class VuJsonContainer;
class VuEntity;
class VuCarEntity;


class VuPowerUp
{
public:
	VuPowerUp() : mPrice(0), mpIconTextureAsset(VUNULL) {}

	std::string			mName;
	VUUINT32			mHashedName;
	int					mPrice;
	VuTextureAsset		*mpIconTextureAsset;
};


class VuPowerUpManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuPowerUpManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuPowerUpManager();

	void				setPowerUpsEnabled(bool enabled) { mPowerUpsEnabled = enabled; }
	bool				getPowerUpsEnabled() { return mPowerUpsEnabled; }

	int					getGroupCount() { return (int)mGroups.size(); }
	VuPowerUpGroup		*getGroup(int index) { return mGroups[index]; }
	VuPowerUpGroup		*getGroupByName(const char *name);

	const VuPowerUp		*choosePowerUp(VuCarEntity *pCar, VUUINT32 groupMask, int &charges);
	const VuPowerUp		*choosePowerUpFromGroup(VuCarEntity *pCar, VuPowerUpGroup *pGroup, int &charges);

	int					getPowerUpCount() { return (int)mPowerUps.size(); }
	const VuPowerUp		*getPowerUp(int index) { return mPowerUps[index]; }
	const VuPowerUp		*getPowerUpByName(const char *name);
	const VuPowerUp		*getForcedPowerUp() { return mpForcePowerUp; }

	typedef std::vector<VuPowerUp *> PowerUps;
	const PowerUps		&getPowerUps() { return mPowerUps; }

protected:
	typedef std::vector<VuPowerUpGroup *> Groups;

	static VuPowerUpGroup	*createGroup(const char *groupName, const VuJsonContainer &groupData);

	bool			mPowerUpsEnabled;
	Groups			mGroups;
	PowerUps		mPowerUps;
	const VuPowerUp	*mpForcePowerUp;
};


class VuPowerUpGroup
{
public:
	class VuVariation
	{
	public:
		VuVariation() : mCharges(1), mWeightMethod(WM_Fixed), mFixedWeight(1.0f) {}

		float					calcWeight(VuCarEntity *pCar) const;

		enum eWeightMethod { WM_Fixed, WM_DistBehindLeader };

		int						mCharges;
		eWeightMethod			mWeightMethod;
		float					mFixedWeight;
		std::vector<VuVector2>	mLinearWeightCurve;
	};
	typedef std::vector<VuVariation> Variations;
	typedef std::vector<VuPowerUp *> PowerUps;

	VuPowerUp			*getPowerUp(const char *name);
	VuPowerUp			*getDefaultPowerUp() { return mPowerUps.front(); }
	void				appendPowerUpNames(std::vector<const char*>& list);

	std::string			mName;
	VUUINT32			mHashedName;
	Variations			mVariations;
	PowerUps			mPowerUps;
};
