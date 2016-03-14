//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HUD LineMap class
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/VuGhostCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"


class VuHUDLineMapEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDLineMapEntity();

protected:
	// scripting
	VuRetVal			IsFirstLap(const VuParams &params) { return VuRetVal(mCurLap == 1); }
	VuRetVal			IsLastLap(const VuParams &params) { return VuRetVal(mCurLap == mLapCount); }

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawMap(float alpha);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int						mViewport;
	VuRect					mRect;
	VuVector2				mOwnerSize;
	VuVector2				mOpponentSize;
	float					mOwnerOffset;
	float					mOpponentOffset;
	VuUIAnchorProperties	mAnchor;
	VuColor					mDifferentLapColor;
	float					mCrashRotSpeed;

	struct Icon
	{
		float		mDistFromStartOfLap;
		int			mCurLap;
		bool		mIsHuman;
		bool		mIsOwner;
		bool		mHasCrashed;
		float		mCrashTimer;
		VuTexture	*mpTexture;
	};
	typedef std::vector<Icon> Icons;
	Icons			mIcons;
	int				mCurLap;
	int				mLapCount;
};

IMPLEMENT_RTTI(VuHUDLineMapEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDLineMapEntity);


//*****************************************************************************
VuHUDLineMapEntity::VuHUDLineMapEntity():
	mViewport(0),
	mRect(0,0,100,10),
	mOwnerSize(100, 100),
	mOpponentSize(50, 50),
	mOwnerOffset(-20),
	mOpponentOffset(20),
	mDifferentLapColor(192,192,192,128),
	mCrashRotSpeed(-VU_2PI),
	mCurLap(0),
	mLapCount(0)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuHUDLineMapEntity::drawLayout));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuVector2Property("Owner Size", mOwnerSize));
	addProperty(new VuVector2Property("Opponent Size", mOpponentSize));
	addProperty(new VuFloatProperty("Owner Offset", mOwnerOffset));
	addProperty(new VuFloatProperty("Opponent Offset", mOpponentOffset));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuColorProperty("Different Lap Color", mDifferentLapColor));
	addProperty(new VuAngleProperty("Crash Rot Speed", mCrashRotSpeed));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuHUDLineMapEntity, IsFirstLap, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuHUDLineMapEntity, IsLastLap, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LapChanged);

	// event handlers
	REG_EVENT_HANDLER(VuHUDLineMapEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDLineMapEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDLineMapEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( VuCarEntity *pOwnerCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		int curLap = VuMin(pOwnerCar->getStats().mCurLap, pOwnerCar->getStats().mLapCount);
		if ( curLap != mCurLap )
		{
			mCurLap = curLap;
			mLapCount = pOwnerCar->getStats().mLapCount;

			char str[16];
			VU_SPRINTF(str, sizeof(str), "%d", mCurLap);
			VuGameFontMacros::IF()->setMacro("CURRENT_LAP", str);

			mpScriptComponent->getPlug("LapChanged")->execute();
		}
	}
}

//*****************************************************************************
void VuHUDLineMapEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawMap(alpha);
}

//*****************************************************************************
void VuHUDLineMapEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect rect = uiDrawParams.transform(mRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), rect);

		// owner
		{
			VuVector2 pos = uiDrawParams.mPosition + mRect.getCenterLeft();
			pos.mX += 0.25f*mRect.mWidth;

			VuRect rect(pos.mX - 0.5f*mOwnerSize.mX, pos.mY - 0.5f*mOwnerSize.mY, mOwnerSize.mX, mOwnerSize.mY);
			rect *= uiDrawParams.mInvAuthScale;
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), rect);
		}

		// opponent
		{
			VuVector2 pos = uiDrawParams.mPosition + mRect.getCenterLeft();
			pos.mX += 0.75f*mRect.mWidth;

			VuRect rect(pos.mX - 0.5f*mOpponentSize.mX, pos.mY - 0.5f*mOpponentSize.mY, mOpponentSize.mX, mOpponentSize.mY);
			rect *= uiDrawParams.mInvAuthScale;
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), rect);
		}
	}
}

//*****************************************************************************
void VuHUDLineMapEntity::drawMap(float alpha)
{
	if ( VuTrackManager::IF()->isBuilt() )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		bool splitScreen = (VuCarManager::IF()->getLocalHumanCarCount() >= 2);

		VuCarEntity *pOwnerCar = splitScreen ? VUNULL : VuCarManager::IF()->getCameraTargetForViewport(mViewport);

		int curLap = 0;
		if ( !splitScreen && pOwnerCar )
			curLap = pOwnerCar->getStats().mCurLap;

		// make list of cars
		mIcons.clear();

		// add car icons
		for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
		{
			VuCarEntity *pCar = VuCarManager::IF()->getCar(i);

			// car is flashing during recovery, skip
			if ( pCar->isInGhostMode() && pCar->getGhostHide() )
				continue;

			// car is hidden (probably eliminated), skip
			if ( pCar->isHidden() )
				continue;

			// no driver portrait, skip
			if ( pCar->getDriver()->getPortraitImage() == VUNULL )
				continue;

			mIcons.resize(mIcons.size() + 1);
			Icon &icon = mIcons.back();

			const VuCarStats &stats = pCar->getStats();

			icon.mDistFromStartOfLap = stats.mDistFromStartOfLap;
			icon.mCurLap = stats.mCurLap;
			icon.mIsHuman = pCar->getDriver()->isHuman();
			icon.mIsOwner = (pCar == pOwnerCar);
			icon.mHasCrashed = pCar->hasCrashed();
			icon.mCrashTimer = pCar->getCrashTimer();
			icon.mpTexture = pCar->getDriver()->getPortraitImage()->getTexture();
		}

		// add ghost car icons
		for ( int i = 0; i < VuCarManager::IF()->getGhostCarCount(); i++ )
		{
			VuGhostCarEntity *pGhostCar = VuCarManager::IF()->getGhostCar(i);

			// no driver portrait, skip
			if ( pGhostCar->getPortraitImage() == VUNULL )
				continue;

			mIcons.resize(mIcons.size() + 1);
			Icon &icon = mIcons.back();

			icon.mDistFromStartOfLap = pGhostCar->getDistFromStartOfLap();
			icon.mCurLap = -1;
			icon.mIsHuman = false;
			icon.mIsOwner = false;
			icon.mHasCrashed = false;
			icon.mCrashTimer = 0.0f;
			icon.mpTexture = pGhostCar->getPortraitImage()->getTexture();
		}

		// sort list of cars
		std::sort(mIcons.begin(), mIcons.end(), [](const Icon &iconA, const Icon &iconB)
		{
			if ( iconA.mIsHuman != iconB.mIsHuman )
				return iconB.mIsHuman;
			return iconB.mDistFromStartOfLap > iconA.mDistFromStartOfLap;
		});

		// apply anchoring
		VuRect rect = uiDrawParams.transform(mRect);
		mAnchor.apply(rect, rect);

		float trackLength = VuTrackManager::IF()->getLength();

		// draw icons
		for ( int i = 0; i < (int)mIcons.size(); i++ )
		{
			Icon &icon = mIcons[i];

			float ratio = VuClamp(icon.mDistFromStartOfLap/trackLength, 0.0f, 1.0f);
			VuVector2 pos = VuLerp(rect.getCenterLeft(), rect.getCenterRight(), ratio);

			VuRect driverRect;
			if ( icon.mIsOwner )
			{
				// use owner properties to draw
				driverRect.mX = -0.5f*mOwnerSize.mX;
				driverRect.mY = -0.5f*mOwnerSize.mY;
				driverRect.mWidth = mOwnerSize.mX;
				driverRect.mHeight = mOwnerSize.mY;
				if ( !splitScreen )
					driverRect.mY += mOwnerOffset;
			}
			else
			{
				// use opponent properties to draw
				driverRect.mX = -0.5f*mOpponentSize.mX;
				driverRect.mY = -0.5f*mOpponentSize.mY;
				driverRect.mWidth = mOpponentSize.mX;
				driverRect.mHeight = mOpponentSize.mY;
				if ( !splitScreen )
					driverRect.mY += mOpponentOffset;
			}
			driverRect *= uiDrawParams.mInvAuthScale;

			driverRect += pos;

			// if car is not on current lap, use different lap color
			VuColor color(255,255,255);
			if ( (curLap > 0) && (icon.mCurLap != curLap) )
				color = mDifferentLapColor;

			// if car has crashed, spin the icon
			VuMatrix mat = VuGfxUtil::IF()->getMatrix();
			if ( icon.mHasCrashed )
			{
				float rotation = icon.mCrashTimer*mCrashRotSpeed;

				VuVector3 center(driverRect.getCenter().mX, driverRect.getCenter().mY, 0);
				float authAR = uiDrawParams.mAuthScale.mX/uiDrawParams.mAuthScale.mY;
				mat.loadIdentity();
				mat.translate(-center);
				mat.scale(VuVector3(authAR, 1.0f, 1.0f));
				mat.rotateZ(rotation);
				mat.scale(VuVector3(1.0f/authAR, 1.0f, 1.0f));
				mat.translate(center);

				mat = mat*VuGfxUtil::IF()->getMatrix();
			}

			// draw icon
			VuGfxUtil::IF()->pushMatrix(mat);
			{
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - i*GFX_SORT_DEPTH_STEP, icon.mpTexture, color, driverRect);
			}
			VuGfxUtil::IF()->popMatrix();
		}
	}
}
