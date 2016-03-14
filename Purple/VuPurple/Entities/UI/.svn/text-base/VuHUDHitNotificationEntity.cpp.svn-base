//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuHUDHitNotificationEntity
// 
//*****************************************************************************

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Math/VuMathUtil.h"


class VuHUDHitNotificationEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDHitNotificationEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// event handlers
	void				ShowHitNotification(const VuParams &params);
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawImage(VuTexture *pTexture, const VuRect &rect, const VuUIDrawParams &uiDrawParams, const VuColor &color, float age);

	void				drawLayout(bool bSelected);

	// properties
	int					mViewport;
	float				mDuration;
	float				mFadeTime;
	float				mAngularFrequency;
	float				mDampingRatio;
	float				mArrowDelay;
	float				mEffectDelay;
	VuUIRectProperties	mDriverRect;
	VuUIRectProperties	mEffectRect;
	VuUIRectProperties	mArrowRect;
	VuUIImageProperties	mArrowImage;

	struct Notification
	{
		Notification() : mAge(0) {}
		VuTextureAsset	*mpDriverImage;
		VuTextureAsset	*mpEffectImage;
		float			mAge;
	};
	typedef std::queue<Notification> Notifications;

	Notifications		mNotifications;
};

IMPLEMENT_RTTI(VuHUDHitNotificationEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDHitNotificationEntity);


//*****************************************************************************
VuHUDHitNotificationEntity::VuHUDHitNotificationEntity():
	mViewport(0),
	mDuration(2.0f),
	mFadeTime(0.25f),
	mAngularFrequency(2.0f),
	mDampingRatio(0.5f),
	mArrowDelay(0.25f),
	mEffectDelay(0.25f)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuHUDHitNotificationEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuFloatProperty("Duration", mDuration));
	addProperty(new VuFloatProperty("FadeTime", mFadeTime));
	addProperty(new VuFloatProperty("AngularFrequency", mAngularFrequency));
	addProperty(new VuFloatProperty("DampingRatio", mDampingRatio));
	addProperty(new VuRectProperty("Driver Rect", mDriverRect));
	addProperty(new VuRectProperty("Effect Rect", mEffectRect));
	addProperty(new VuRectProperty("Arrow Rect", mArrowRect));
	mArrowImage.addProperties(getProperties(), "Arrow Image");

	// event handlers
	REG_EVENT_HANDLER(VuHUDHitNotificationEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDHitNotificationEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDHitNotificationEntity::onGameInitialize()
{
	// event handlers
	REG_EVENT_HANDLER(VuHUDHitNotificationEntity, ShowHitNotification);
}

//*****************************************************************************
void VuHUDHitNotificationEntity::onGameRelease()
{
	while ( mNotifications.size() )
	{
		VuAssetFactory::IF()->releaseAsset(mNotifications.front().mpEffectImage);
		mNotifications.pop();
	}

	mEventMap.unregisterHandler("ShowHitNotification");
}

//*****************************************************************************
void VuHUDHitNotificationEntity::ShowHitNotification(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int viewport = accessor.getInt();

	if ( viewport == mViewport )
	{
		VuAsset *pDriverAsset = accessor.getAsset();
		const char *image = accessor.getString();

		Notification notification;

		notification.mpEffectImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(image, VuAssetFactory::OPTIONAL_ASSET);
		if ( notification.mpEffectImage )
		{
			VUASSERT(pDriverAsset->isDerivedFrom(VuTextureAsset::msRTTI), "Bad Driver Icon");
			notification.mpDriverImage = static_cast<VuTextureAsset *>(pDriverAsset);

			mNotifications.push(notification);
		}
	}
}

//*****************************************************************************
void VuHUDHitNotificationEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( mNotifications.size() )
	{
		Notification &notification = mNotifications.front();

		notification.mAge += fdt;

		if ( notification.mAge >= mDuration )
		{
			VuAssetFactory::IF()->releaseAsset(notification.mpEffectImage);
			mNotifications.pop();
		}
	}
}

//*****************************************************************************
void VuHUDHitNotificationEntity::OnUIDraw(const VuParams &params)
{
	if ( mNotifications.size() )
	{
		Notification &notification = mNotifications.front();

		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		float alpha = 1.0f - VuLinStep(mDuration - mFadeTime, mDuration, notification.mAge);

		VuColor color(255,255,255);
		color.mA = (VUUINT8)VuRound(alpha*255.0f);

		drawImage(notification.mpDriverImage->getTexture(), mDriverRect, uiDrawParams, color, notification.mAge);
		drawImage(mArrowImage.getTexture(), mArrowRect, uiDrawParams, color, notification.mAge - mArrowDelay);
		drawImage(notification.mpEffectImage->getTexture(), mEffectRect, uiDrawParams, color, notification.mAge - mEffectDelay);
	}
}

//*****************************************************************************
void VuHUDHitNotificationEntity::drawImage(VuTexture *pTexture, const VuRect &rect, const VuUIDrawParams &uiDrawParams, const VuColor &color, float age)
{
	age = VuMax(age, 0.0f);

	float scale = 0.0f, vel = 0.0f;
	VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, 1.0f, age, mAngularFrequency*VU_2PI, mDampingRatio);

	VuRect dstRect = uiDrawParams.transform(rect);
	dstRect.scale(dstRect.getCenter(), scale);
	VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pTexture, color, dstRect);
}

//*****************************************************************************
void VuHUDHitNotificationEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect rect;

	rect = uiDrawParams.transform(mDriverRect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), rect);

	rect = uiDrawParams.transform(mEffectRect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), rect);

	rect = uiDrawParams.transform(mArrowRect);
	VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mArrowImage.getTexture(), rect);
}
