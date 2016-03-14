//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUp HUD entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"

#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/DB/VuStringDB.h"

//*****************************************************************************
// Slot
//*****************************************************************************

class VuHUDPowerUpSlotEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDPowerUpSlotEntity();



protected:
	// event handlers
	virtual void					OnUITick(const VuParams &params);
	virtual void					OnUIDraw(const VuParams &params);

	virtual void					drawLayout(bool bSelected);
	virtual void					drawSlot(float alpha);

	// components

	// properties
	int						mViewport;
	VuRect					mImageRect;
	VuRect					mInfoRect;
	std::string				mInfoFont;
	VuUIAnchorProperties	mAnchor;
	int						mSlotIndex;
	float					mScaleSpringCoeff;
	float					mScaleDampingCoeff;
	float					mFadeOutScale;
	float					mFadeOutTime;

	// constants
	float					mSlotMachineTickTime;

	struct Instance
	{
		explicit Instance(const VuPowerUp *pPowerUp, VUUINT32 instanceId);

		bool		operator < (const Instance &other) const { return mScale < other.mScale; }
		static bool	isDone(const Instance &instance) { return instance.mDone; }

		enum eState { STATE_BOING_IN, STATE_BOING_OUT, STATE_FADE_OUT };
		const VuPowerUp		*mpPowerUp;
		VUUINT32			mInstanceId;
		VuTexture			*mpTexture;
		eState				mState;
		float				mScale;
		float				mScaleVelocity;
		bool				mDone;
	};
	typedef std::list<Instance> Instances;
	Instances				mInstances;
	VUUINT32				mLastInstanceId;
};

IMPLEMENT_RTTI(VuHUDPowerUpSlotEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDPowerUpSlotEntity);

//*****************************************************************************
VuHUDPowerUpSlotEntity::VuHUDPowerUpSlotEntity():
	mViewport(0),
	mImageRect(0,0,0,0),
	mInfoRect(0,0,0,0),
	mSlotIndex(0),
	mScaleSpringCoeff(65.0f),
	mScaleDampingCoeff(5.0f),
	mFadeOutScale(2.0f),
	mFadeOutTime(0.5f),
	mLastInstanceId(0)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuHUDPowerUpSlotEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Info Rect", mInfoRect));
	addProperty(new VuFontEnumProperty("Info Font", mInfoFont));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuIntProperty("Slot Index", mSlotIndex));
	addProperty(new VuFloatProperty("Scale Spring Coeff", mScaleSpringCoeff));
	addProperty(new VuFloatProperty("Scale Damping Coeff", mScaleDampingCoeff));
	addProperty(new VuFloatProperty("Fade Out Scale", mFadeOutScale));
	addProperty(new VuFloatProperty("Fade Out Time", mFadeOutTime));

	// event handlers
	REG_EVENT_HANDLER(VuHUDPowerUpSlotEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDPowerUpSlotEntity, OnUIDraw);

	// constants
	mSlotMachineTickTime = VuGameUtil::IF()->constantDB()["PowerUps"]["SlotMachineTickTime"].asFloat();
}

//*****************************************************************************
void VuHUDPowerUpSlotEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	// update instances
	for ( Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++ )
	{
		if ( iter->mState == Instance::STATE_BOING_IN )
		{
			float acc = mScaleSpringCoeff*(1.0f - iter->mScale) - mScaleDampingCoeff*iter->mScaleVelocity;
			iter->mScale += iter->mScaleVelocity*fdt + 0.5f*acc*fdt*fdt;
			iter->mScaleVelocity += acc*fdt;
		}
		if ( iter->mState == Instance::STATE_BOING_OUT )
		{
			float acc = mScaleSpringCoeff*(0.0f - iter->mScale) - mScaleDampingCoeff*iter->mScaleVelocity;
			iter->mScale += iter->mScaleVelocity*fdt + 0.5f*acc*fdt*fdt;
			iter->mScaleVelocity += acc*fdt;
			iter->mDone = iter->mScale <= 0.0f;
		}
		else if ( iter->mState == Instance::STATE_FADE_OUT )
		{
			iter->mScale += fdt*(mFadeOutScale-1.0f)/mFadeOutTime;
			iter->mDone = iter->mScale >= mFadeOutScale;
		}
	}

	// check for new instance
	const VuPowerUp *pPowerUp = VUNULL;
	VUUINT32 instanceId = 0;
	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		const VuCarPowerUpController::Slot &slot = pCar->getPowerUpController()->getSlot(mSlotIndex);
		if ( !slot.mSlotMachineActive )
		{
			instanceId = slot.mInstanceId;
			pPowerUp = slot.mpPowerUp;
		}
	}

	if ( instanceId != mLastInstanceId )
	{
		// current instances fade out for slot 0 (power up was used) and boing out for other slots
		for ( Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++ )
		{
			iter->mState = (mSlotIndex == 0) ? Instance::STATE_FADE_OUT : Instance::STATE_BOING_OUT;
			iter->mpPowerUp = VUNULL;
			iter->mInstanceId = 0;
		}
		if ( instanceId )
			mInstances.push_back(Instance(pPowerUp, instanceId));
		mLastInstanceId = instanceId;
	}

	// slot machine trumps whatever is going on
	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
		if ( pCar->getPowerUpController()->getSlot(mSlotIndex).mSlotMachineActive )
			mInstances.clear();

	// sort and remove instances that are done
	mInstances.remove_if(Instance::isDone);
	mInstances.sort();
}

//*****************************************************************************
void VuHUDPowerUpSlotEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawSlot(alpha);
}

//*****************************************************************************
void VuHUDPowerUpSlotEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect imageRect = uiDrawParams.transform(mImageRect);

	VuColor color = bSelected ? VuColor(255,255,0) : VuColor(255,255,255);
	VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, color, imageRect);

	VuRect infoRect = uiDrawParams.transform(mInfoRect);

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - GFX_SORT_DEPTH_STEP, fontEntry.font(), "Text", fontEntry.params(), infoRect, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER);
}

//*****************************************************************************
void VuHUDPowerUpSlotEntity::drawSlot(float alpha)
{
	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuCarPowerUpController::Slot &slot = pCar->getPowerUpController()->getSlot(mSlotIndex);
		if ( slot.mSlotMachineActive )
		{
			VuRect imageRect = uiDrawParams.transform(mImageRect);
			mAnchor.apply(imageRect, imageRect);

			VuTextureAsset *pIcon = VUNULL;

			int count = (int)VuGameManager::IF()->getOwnedPowerUps().size();
			int index = VuTruncate(slot.mSlotMachineTimer/mSlotMachineTickTime)%count;
			for ( const auto &powerUp : VuGameManager::IF()->getOwnedPowerUps() )
			{
				if ( index == 0 )
				{
					pIcon = VuPowerUpManager::IF()->getPowerUpByName(powerUp.c_str())->mpIconTextureAsset;
					break;
				}
				index--;
			}

			if ( pIcon )
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pIcon->getTexture(), imageRect);
		}
		else
		{
			float depth = uiDrawParams.mDepth;
			for ( Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++ )
			{
				VuRect imageRect = uiDrawParams.transform(mImageRect);
				mAnchor.apply(imageRect, imageRect);

				imageRect.scale(imageRect.getCenter(), iter->mScale);

				float imageAlpha = alpha;
				if ( iter->mState == Instance::STATE_FADE_OUT )
					imageAlpha *= 1.0f - VuLinStep(1.0f, mFadeOutScale, iter->mScale);
				else
					imageAlpha *= VuMin(iter->mScale, 1.0f);

				VuColor color(255,255,255,VuRound(255*imageAlpha));
				VuGfxUtil::IF()->drawTexture2d(depth, iter->mpTexture, color, imageRect);
				depth -= GFX_SORT_DEPTH_STEP;

				if ( iter->mInstanceId == slot.mInstanceId )
				{
					if ( slot.mChargesRemaining > 1 )
					{
						char str[32];
						VU_SPRINTF(str, sizeof(str), "%d", slot.mChargesRemaining);

						const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
						VuFontDrawParams params = fontEntry.params();
						params.mSize *= iter->mScale;

						VuRect infoRect = uiDrawParams.transform(mInfoRect);
						mAnchor.apply(infoRect, infoRect);

						infoRect.scale(imageRect.getCenter(), iter->mScale);

						VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), str, params, infoRect, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER, alpha);
					}
				}
			}
		}
	}
}

//*****************************************************************************
VuHUDPowerUpSlotEntity::Instance::Instance(const VuPowerUp *pPowerUp, VUUINT32 instanceId):
	mpPowerUp(pPowerUp),
	mInstanceId(instanceId),
	mState(STATE_BOING_IN),
	mScale(0.0f),
	mScaleVelocity(0.0f),
	mDone(false)
{
	mpTexture = pPowerUp->mpIconTextureAsset->getTexture();
}










//*****************************************************************************
// Fixed PowerUp
//*****************************************************************************

class VuHUDFixedPowerUpSlotEntity : public VuHUDPowerUpSlotEntity
{
	DECLARE_RTTI

public:
	VuHUDFixedPowerUpSlotEntity();

	void				gameInitialize();

protected:
	// event handlers
	virtual void					OnUITick(const VuParams &params);
	virtual void					OnUIDraw(const VuParams &params);

	virtual void					drawLayout(bool bSelected);
	virtual void					drawSlot(float alpha);

protected:
	std::string				mText;
	VuRect					mTextRect;

};


IMPLEMENT_RTTI(VuHUDFixedPowerUpSlotEntity, VuHUDPowerUpSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDFixedPowerUpSlotEntity);


//*****************************************************************************

VuHUDFixedPowerUpSlotEntity::VuHUDFixedPowerUpSlotEntity()
{
	addProperty(new VuRectProperty("Text Rect", mTextRect));
}



void VuHUDFixedPowerUpSlotEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if (mInstances.empty())
	{
		if (VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport))
		{

			const VuCarPowerUpController::Slot &slot1 = pCar->getPowerUpController()->GetExtraFrontSlot();
			if (slot1.mpPowerUp)
			{
				mInstances.push_back(Instance(slot1.mpPowerUp, 1));

			}	
			const VuCarPowerUpController::Slot &slot2 = pCar->getPowerUpController()->GetExtraBehindSlot();
			if (slot2.mpPowerUp && slot2.mpPowerUp->mName != slot1.mpPowerUp->mName)
			{
				mInstances.push_back(Instance(slot2.mpPowerUp, 2));
			}
		}
	}

	// update instances
	for (Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++)
	{
		if (iter->mState == Instance::STATE_BOING_IN)
		{
			float acc = mScaleSpringCoeff*(1.0f - iter->mScale) - mScaleDampingCoeff*iter->mScaleVelocity;
			iter->mScale += iter->mScaleVelocity*fdt + 0.5f*acc*fdt*fdt;
			iter->mScaleVelocity += acc*fdt;
		}
		if (iter->mState == Instance::STATE_BOING_OUT)
		{
			float acc = mScaleSpringCoeff*(0.0f - iter->mScale) - mScaleDampingCoeff*iter->mScaleVelocity;
			iter->mScale += iter->mScaleVelocity*fdt + 0.5f*acc*fdt*fdt;
			iter->mScaleVelocity += acc*fdt;
			iter->mDone = iter->mScale <= 0.0f;
		}
		else if (iter->mState == Instance::STATE_FADE_OUT)
		{
			iter->mScale += fdt*(mFadeOutScale - 1.0f) / mFadeOutTime;
			iter->mDone = iter->mScale >= mFadeOutScale;
		}
	}



	/*// check for new instance
	const VuPowerUp *pPowerUp = VUNULL;
	VUUINT32 instanceId = 0;
	if (VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport))
	{
		const VuCarPowerUpController::Slot &slot = pCar->getPowerUpController()->getSlot(mSlotIndex);
		if (!slot.mSlotMachineActive)
		{
			instanceId = slot.mInstanceId;
			pPowerUp = slot.mpPowerUp;
		}
	}

	if (instanceId != mLastInstanceId)
	{
		// current instances fade out for slot 0 (power up was used) and boing out for other slots
		for (Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++)
		{
			iter->mState = (mSlotIndex == 0) ? Instance::STATE_FADE_OUT : Instance::STATE_BOING_OUT;
			iter->mpPowerUp = VUNULL;
			iter->mInstanceId = 0;
		}
		if (instanceId)
			mInstances.push_back(Instance(pPowerUp, instanceId));
		mLastInstanceId = instanceId;
	}
*/

	

	/*// slot machine trumps whatever is going on
	if (VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport))
		if (pCar->getPowerUpController()->getSlot(mSlotIndex).mSlotMachineActive)
			mInstances.clear();

	// sort and remove instances that are done
	mInstances.remove_if(Instance::isDone);
	mInstances.sort();*/
}

//*****************************************************************************
void VuHUDFixedPowerUpSlotEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawSlot(alpha);
}

//*****************************************************************************
void VuHUDFixedPowerUpSlotEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect imageRect = uiDrawParams.transform(mImageRect);

	VuColor color = bSelected ? VuColor(255, 255, 0) : VuColor(255, 255, 255);
	VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, color, imageRect);

	VuRect infoRect = uiDrawParams.transform(mInfoRect);

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - GFX_SORT_DEPTH_STEP, fontEntry.font(), "Text", fontEntry.params(), infoRect, VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER);
}

//*****************************************************************************
void VuHUDFixedPowerUpSlotEntity::drawSlot(float alpha)
{
	if (VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport))
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		//const VuCarPowerUpController::Slot &slot = pCar->getPowerUpController()->getSlot(mSlotIndex);

// modify by xlm,[/2015/4/1 ] 

		const VuCarPowerUpController::Slot &slot = pCar->getPowerUpController()->GetExtraSlot();

		int nExtraPowerupPrice = 0;
		nExtraPowerupPrice = pCar->getPowerUpController()->GetExtraPowerupPrice();

		if (slot.mSlotMachineActive)
		{
			VuRect imageRect = uiDrawParams.transform(mImageRect);
			mAnchor.apply(imageRect, imageRect);

			VuTextureAsset *pIcon = VUNULL;
			pIcon = VuPowerUpManager::IF()->getPowerUpByName(pCar->getPowerUpController()->GetExtraPowerupName().c_str())->mpIconTextureAsset;
			/*int count = (int)VuGameManager::IF()->getOwnedPowerUps().size();
			int index = VuTruncate(slot.mSlotMachineTimer / mSlotMachineTickTime) % count;
			for (const auto &powerUp : VuGameManager::IF()->getOwnedPowerUps())
			{
				if (index == 0)
				{
					pIcon = VuPowerUpManager::IF()->getPowerUpByName(powerUp.c_str())->mpIconTextureAsset;
					break;
				}
				index--;
			}*/
			

			if (pIcon)
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pIcon->getTexture(), imageRect);

			if (nExtraPowerupPrice > 0)
			{
				//渲染价格
				std::string bodyText = VuStringDB::IF()->getString("ExtraPowerupPrice");
				char  strTemp[20] = { 0 };
				sprintf(strTemp, "%d",nExtraPowerupPrice);
				VuStringUtil::replace(bodyText, "[EXTRA_POWERUP_ITEM_PRICE]", strTemp);
				float depth = uiDrawParams.mDepth;
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
				VuFontDrawParams params = fontEntry.params();

				VuRect textRect = uiDrawParams.transform(mTextRect);
				mAnchor.apply(textRect, textRect);
				depth -= GFX_SORT_DEPTH_STEP;
				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), bodyText.c_str(), params, textRect, VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER, alpha);
			}
		}
		else
		{
			float depth = uiDrawParams.mDepth;

			if (nExtraPowerupPrice > 0)
			{
				//渲染价格
				std::string bodyText = VuStringDB::IF()->getString("ExtraPowerupPrice");
				char  strTemp[20] = { 0 };
				sprintf(strTemp, "%d", nExtraPowerupPrice);
				VuStringUtil::replace(bodyText, "[EXTRA_POWERUP_ITEM_PRICE]", strTemp);
				float depth = uiDrawParams.mDepth;
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
				VuFontDrawParams params = fontEntry.params();

				VuRect textRect = uiDrawParams.transform(mTextRect);
				mAnchor.apply(textRect, textRect);
				VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - GFX_SORT_DEPTH_STEP, fontEntry.font(), bodyText.c_str(), params, textRect, VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER, alpha);
			}

			for (Instances::iterator iter = mInstances.begin(); iter != mInstances.end(); iter++)
			{
				VuRect imageRect = uiDrawParams.transform(mImageRect);
				mAnchor.apply(imageRect, imageRect);

				imageRect.scale(imageRect.getCenter(), iter->mScale);

				float imageAlpha = alpha;
				if (iter->mState == Instance::STATE_FADE_OUT)
					imageAlpha *= 1.0f - VuLinStep(1.0f, mFadeOutScale, iter->mScale);
				else
					imageAlpha *= VuMin(iter->mScale, 1.0f);

				VuColor color(255, 255, 255, VuRound(255 * imageAlpha));
				
				if (iter->mInstanceId == slot.mInstanceId)
				{
					VuGfxUtil::IF()->drawTexture2d(depth, iter->mpTexture, color, imageRect);
					depth -= GFX_SORT_DEPTH_STEP;

					if (slot.mChargesRemaining > 1)
					{
						char str[32];
						VU_SPRINTF(str, sizeof(str), "%d", slot.mChargesRemaining);

						const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mInfoFont);
						VuFontDrawParams params = fontEntry.params();
						params.mSize *= iter->mScale;

						VuRect infoRect = uiDrawParams.transform(mInfoRect);
						mAnchor.apply(infoRect, infoRect);

						infoRect.scale(imageRect.getCenter(), iter->mScale);

						VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), str, params, infoRect, VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER, alpha);
					}
				}

				
			}
		}
	}
}

