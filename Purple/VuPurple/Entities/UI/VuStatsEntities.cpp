//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stat entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"


//*****************************************************************************
class VuBaseStatEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuBaseStatEntity();

	virtual void			onGameInitialize();

protected:
	void					OnUITick(const VuParams &params);
	void					OnUIDraw(const VuParams &params);

	virtual float			getValue(int partialBarIndex) = 0;

	void					drawLayout(bool bSelected);
	void					drawStat(float alpha);

	VuRect					calcRect(const VuUIDrawParams &uiDrawParams, float value0, float value1);

	VuRetVal				Show(const VuParams &params) { mVisible = true; return VuRetVal(); }
	VuRetVal				Hide(const VuParams &params) { mVisible = false; return VuRetVal(); }

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	bool					mVisible;
	VuRect					mRect;
	VuUIAnchorProperties	mAnchor;
	float					mAnimationTime;
	bool					mSequentialAnimation;

	struct PartialBar
	{
		float	mSourceValue;
		float	mTargetValue;
		float	mDisplayValue;
		VuColor	mColor;
		float	mTimer;
	};
	typedef std::vector<PartialBar> PartialBars;
	PartialBars				mPartialBars;
};

IMPLEMENT_RTTI(VuBaseStatEntity, VuEntity);

//*****************************************************************************
VuBaseStatEntity::VuBaseStatEntity():
	mVisible(true),
	mRect(0,0,0,0),
	mAnimationTime(1.0f),
	mSequentialAnimation(false)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuBaseStatEntity::drawLayout));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuBoolProperty("Initially Visible", mVisible));
	addProperty(new VuRectProperty("Rect", mRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuFloatProperty("Animation Time", mAnimationTime));
	addProperty(new VuBoolProperty("Sequential Animation", mSequentialAnimation));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBaseStatEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBaseStatEntity, Hide);

	// event handlers
	REG_EVENT_HANDLER(VuBaseStatEntity, OnUITick);
	REG_EVENT_HANDLER(VuBaseStatEntity, OnUIDraw);
}

//*****************************************************************************
void VuBaseStatEntity::onGameInitialize()
{
	for ( int i = 0; i < (int)mPartialBars.size(); i++ )
	{
		PartialBar &bar = mPartialBars[i];
		bar.mSourceValue = bar.mTargetValue = bar.mDisplayValue = getValue(i);
		bar.mTimer = mAnimationTime;
	}
}

//*****************************************************************************
void VuBaseStatEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	for ( int i = 0; i < (int)mPartialBars.size(); i++ )
	{
		PartialBar &bar = mPartialBars[i];
		float value = getValue(i);
		if ( value != bar.mTargetValue )
		{
			bar.mTargetValue = value;
			bar.mSourceValue = bar.mDisplayValue;
			bar.mTimer = 0.0f;
		}
	}

	for ( int i = 0; i < (int)mPartialBars.size(); i++ )
	{
		PartialBar &bar = mPartialBars[i];
		if ( bar.mTimer < mAnimationTime )
		{
			bar.mTimer = VuMin(bar.mTimer + fdt, mAnimationTime);
			bar.mDisplayValue = VuLerp(bar.mSourceValue, bar.mTargetValue, bar.mTimer/mAnimationTime);
			if ( mSequentialAnimation )
				break;
		}
	}
}

//*****************************************************************************
void VuBaseStatEntity::OnUIDraw(const VuParams &params)
{
	if ( mVisible )
		drawStat(mpTransitionComponent->alpha());
}

//*****************************************************************************
void VuBaseStatEntity::drawLayout(bool bSelected)
{
	for ( int i = 0; i < (int)mPartialBars.size(); i++ )
		mPartialBars[i].mDisplayValue = (float)(i + 1)/mPartialBars.size();

	drawStat(1.0f);
}

//*****************************************************************************
void VuBaseStatEntity::drawStat(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	// filled rect
	float lastValue = 0.0f;
	for ( PartialBar &bar : mPartialBars )
	{
		if ( bar.mDisplayValue > lastValue )
		{
			VuColor color = bar.mColor;
			color.mA = VuRound(color.mA*alpha);
			VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth, color, calcRect(uiDrawParams, lastValue, bar.mDisplayValue));

			lastValue = bar.mDisplayValue;
		}
	}
}

//*****************************************************************************
VuRect VuBaseStatEntity::calcRect(const VuUIDrawParams &uiDrawParams, float value0, float value1)
{
	VuRect rect = mRect;
	rect.mX += value0*rect.mWidth;
	rect.mWidth *= (value1 - value0);

	rect = uiDrawParams.transform(rect);
	mAnchor.apply(rect, rect);

	return rect;
}


//*****************************************************************************
// Car Stat Entity
//*****************************************************************************

class VuCarStatEntity : public VuBaseStatEntity
{
	DECLARE_RTTI

public:
	VuCarStatEntity();

protected:
	virtual float	getValue(int partialBarIndex);

	float			calcStat(const std::string &carName, int stage, int level);

	// properties
	std::string		mStat;

	float			mMinValue;
	float			mMaxValue;

	enum { FILLED_BAR, NEXT_BAR, POTENTIAL_BAR, FULL_POTENTIAL_BAR, EMPTY_BAR, NUM_BARS };
};


IMPLEMENT_RTTI(VuCarStatEntity, VuBaseStatEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarStatEntity);

//*****************************************************************************
VuCarStatEntity::VuCarStatEntity()
{
	// properties
	static VuStaticStringEnumProperty::Choice sStatChoices[] = { "Accel", "Speed", "Handling", "Tough", VUNULL };
	addProperty(new VuStaticStringEnumProperty("Stat", mStat, sStatChoices));

	mMinValue = VuGameUtil::IF()->constantDB()["Game"]["MinCarStatValue"].asFloat();
	mMaxValue = VuGameUtil::IF()->constantDB()["Game"]["MaxCarStatValue"].asFloat();

	mPartialBars.resize(NUM_BARS);
	mPartialBars[FILLED_BAR].mColor.set(255,255,255);
	mPartialBars[NEXT_BAR].mColor.set(128,255,128);
	mPartialBars[POTENTIAL_BAR].mColor.set(128,128,128);
	mPartialBars[FULL_POTENTIAL_BAR].mColor.set(64,64,64);
	mPartialBars[EMPTY_BAR].mColor.set(0,0,0);

	addProperty(new VuColorProperty("Filled Color", mPartialBars[FILLED_BAR].mColor));
	addProperty(new VuColorProperty("Next Color", mPartialBars[NEXT_BAR].mColor));
	addProperty(new VuColorProperty("Potential Color", mPartialBars[POTENTIAL_BAR].mColor));
	addProperty(new VuColorProperty("Full Potential Color", mPartialBars[FULL_POTENTIAL_BAR].mColor));
	addProperty(new VuColorProperty("Empty Color", mPartialBars[EMPTY_BAR].mColor));
}

//*****************************************************************************
float VuCarStatEntity::getValue(int partialBarIndex)
{
	float value = 0.0f;

	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	const VuGameManager::Car &car = VuGameManager::IF()->getCar(selectedCar);

	if ( partialBarIndex == FILLED_BAR )
	{
		int level = car.getStat(mStat.c_str());
		value = calcStat(car.mName, car.mStage, level);
	}
	else if ( partialBarIndex == NEXT_BAR )
	{
		int level = VuMin(car.getStat(mStat.c_str()) + 1, VuGameUtil::IF()->getMaxCarLevel(car.mStage));
		value = calcStat(car.mName, car.mStage, level);
	}
	else if ( partialBarIndex == POTENTIAL_BAR )
	{
		value = calcStat(car.mName, car.mStage, VuGameUtil::IF()->getMaxCarLevel(car.mStage));
	}
	else if ( partialBarIndex == FULL_POTENTIAL_BAR )
	{
		//tony.hu
		const int maxLevel = VuGameUtil::IF()->carDB()[car.mName]["skycn_maxStage"].asInt();

		//value = calcStat(car.mName, maxLevel,/*3*/, VuGameUtil::IF()->getMaxCarLevel(3));
		value = calcStat(car.mName, maxLevel, VuGameUtil::IF()->getMaxCarLevel(maxLevel));
	}
	else if ( partialBarIndex == EMPTY_BAR )
	{
		value = 1.0f;
	}

	return value;
}

//*****************************************************************************
float VuCarStatEntity::calcStat(const std::string &carName, int stage, int level)
{
	char columnName[32];
	VU_SPRINTF(columnName, sizeof(columnName), "%s Stat", mStat.c_str());
	float value = VuGameUtil::IF()->interpolateCarLevel(carName, stage, level, columnName);
	value = (value - mMinValue)/(mMaxValue - mMinValue);

	return value;
}


//*****************************************************************************
// VuLevelUpStatEntity
//*****************************************************************************

class VuLevelUpStatEntity : public VuBaseStatEntity
{
	DECLARE_RTTI

public:
	VuLevelUpStatEntity();

protected:
	virtual float	getValue(int partialBarIndex);

	float			mMinValue;
	float			mMaxValue;

	enum { FILLED_BAR, EMPTY_BAR, NUM_BARS };
};


IMPLEMENT_RTTI(VuLevelUpStatEntity, VuBaseStatEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLevelUpStatEntity);

//*****************************************************************************
VuLevelUpStatEntity::VuLevelUpStatEntity()
{
	// properties
	mPartialBars.resize(NUM_BARS);
	mPartialBars[FILLED_BAR].mColor.set(255,255,255);
	mPartialBars[EMPTY_BAR].mColor.set(0,0,0);

	addProperty(new VuColorProperty("Filled Color", mPartialBars[FILLED_BAR].mColor));
	addProperty(new VuColorProperty("Empty Color", mPartialBars[EMPTY_BAR].mColor));
}

//*****************************************************************************
float VuLevelUpStatEntity::getValue(int partialBarIndex)
{
	float value = 0.0f;

	if ( partialBarIndex == FILLED_BAR )
	{
		const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
		const VuGameManager::Car &car = VuGameManager::IF()->getCar(selectedCar);

		value = car.getStageProgress();
	}
	else if ( partialBarIndex == EMPTY_BAR )
	{
		value = 1.0f;
	}

	return value;
}
