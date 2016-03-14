//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuHUDScoreThermometerEntity
// 
//*****************************************************************************


#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"


class VuHUDScoreThermometerEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDScoreThermometerEntity();

	void				onGameInitialize();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				draw(float value, float *pMarkValues, float alpha);

	// components
	VuScriptComponent		*mpScriptComponent;

	// properties
	int						mViewport;
	VuUIImageProperties		mBackImage;
	VuUIRectProperties		mBackRect;
	VuUIImageProperties		mBarImage;
	VuUIRectProperties		mBarRect;
	float					mBarStart;
	float					mBarEnd;
	VuUIImageProperties		mMarkImages[3];
	VuUIImageProperties		mMarkReachedImages[3];
	VuUIRectProperties		mMarkRect;
	VuUIAnchorProperties	mAnchor;

	struct Mark
	{
		Mark() : mEnabled(false), mScore(1), mReached(false) {}
		bool update(int score)
		{
			if ( mEnabled && !mReached && score >= mScore )
			{
				mReached = true;
				return true;
			}
			return false;
		}
		bool	mEnabled;
		int		mScore;
		bool	mReached;
	};
	Mark	mMarks[3];
};

IMPLEMENT_RTTI(VuHUDScoreThermometerEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDScoreThermometerEntity);


//*****************************************************************************
VuHUDScoreThermometerEntity::VuHUDScoreThermometerEntity():
	mViewport(0),
	mBarStart(0),
	mBarEnd(1)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuHUDScoreThermometerEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	mBackImage.addProperties(getProperties(), "Back Image");
	addProperty(new VuRectProperty("Back Rect", mBackRect));
	mBarImage.addProperties(getProperties(), "Bar Image");
	addProperty(new VuRectProperty("Bar Rect", mBarRect));
	addProperty(new VuFloatProperty("Bar Start", mBarStart));
	addProperty(new VuFloatProperty("Bar End", mBarEnd));
	mMarkImages[0].addProperties(getProperties(), "Mark 1 Image");
	mMarkImages[1].addProperties(getProperties(), "Mark 2 Image");
	mMarkImages[2].addProperties(getProperties(), "Mark 3 Image");
	mMarkReachedImages[0].addProperties(getProperties(), "Mark 1 Reached Image");
	mMarkReachedImages[1].addProperties(getProperties(), "Mark 2 Reached Image");
	mMarkReachedImages[2].addProperties(getProperties(), "Mark 3 Reached Image");
	addProperty(new VuRectProperty("Mark Rect", mMarkRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// event handlers
	REG_EVENT_HANDLER(VuHUDScoreThermometerEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDScoreThermometerEntity, OnUIDraw);

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReached1);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReached2);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReached3);
}

//*****************************************************************************
void VuHUDScoreThermometerEntity::onGameInitialize()
{
	const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

	mMarks[2].mEnabled = criteria[0].getValue(mMarks[2].mScore);
	mMarks[1].mEnabled = criteria[1].getValue(mMarks[1].mScore);
	mMarks[0].mEnabled = criteria[2].getValue(mMarks[0].mScore);
}

//*****************************************************************************
void VuHUDScoreThermometerEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		const VuCarStats &stats = pCar->getStats();

		if ( mMarks[0].update(stats.mScore) )
			mpScriptComponent->getPlug("OnReached1")->execute();

		if ( mMarks[1].update(stats.mScore) )
			mpScriptComponent->getPlug("OnReached2")->execute();

		if ( mMarks[2].update(stats.mScore) )
			mpScriptComponent->getPlug("OnReached3")->execute();
	}
}

//*****************************************************************************
void VuHUDScoreThermometerEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		float maxValue = (float)mMarks[2].mScore;

		float markValues[3];
		for ( int i = 0; i < 3; i++ )
			markValues[i] = mMarks[i].mScore/maxValue;

		draw(pCar->getStats().mScore/maxValue, markValues, alpha);
	}
}

//*****************************************************************************
void VuHUDScoreThermometerEntity::drawLayout(bool bSelected)
{
	for ( int i = 0; i < 3; i++ )
		mMarks[i].mEnabled = true;

	float markValues[3] = { 0.6f, 0.8f, 1.0f };
	draw(0.8f, markValues, 1.0f);
}

//*****************************************************************************
void VuHUDScoreThermometerEntity::draw(float value, float *pMarkValues, float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuColor color(255,255,255);
	color.mA = (VUUINT8)VuRound(255*alpha);

	float depth = uiDrawParams.mDepth;

	// background
	{
		VuRect dstRect = uiDrawParams.transform(mBackRect);
		mAnchor.apply(dstRect, dstRect);
		VuGfxUtil::IF()->drawTexture2d(depth, mBackImage.getTexture(), color, dstRect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// bar
	{
		float ratio = VuMin(mBarRect.mWidth, VuLerp(mBarStart, mBarEnd, value))/mBarRect.mWidth;
		VuRect dstRect = mBarRect;
		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);
		dstRect.mWidth *= ratio;
		VuRect srcRect(0.0f, 0.0f, ratio, 1.0f);
		VuGfxUtil::IF()->drawTexture2d(depth, mBarImage.getTexture(), color, dstRect, srcRect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// marks
	for ( int i = 0; i < 3; i++ )
	{
		if ( mMarks[i].mEnabled )
		{
			float ratio = VuMin(mBarRect.mWidth, VuLerp(mBarStart, mBarEnd, pMarkValues[i]))/mBarRect.mWidth;

			VuRect barRect = uiDrawParams.transform(mBarRect);
			VuVector2 pos0 = VuLerp(barRect.getCenterLeft(), barRect.getCenterRight(), ratio);
			mAnchor.apply(barRect, barRect);
			VuVector2 pos1 = VuLerp(barRect.getCenterLeft(), barRect.getCenterRight(), ratio);

			VuRect dstRect = mMarkRect;
			dstRect.mX += VuMin(mBarRect.mWidth, VuLerp(mBarStart, mBarEnd, pMarkValues[i]));
			dstRect = uiDrawParams.transform(dstRect);
			dstRect += (pos1 - pos0);

			VuUIImageProperties &image = (value >= pMarkValues[i]) ? mMarkReachedImages[i] : mMarkImages[i];
			VuGfxUtil::IF()->drawTexture2d(depth, image.getTexture(), color, dstRect);
		}
	}
	depth -= GFX_SORT_DEPTH_STEP;
}
