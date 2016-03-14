//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  game entities
// 
//*****************************************************************************

#include "VuGameTextEntity.h"
#include "VuGameImageEntity.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


//*****************************************************************************
// Event Image Element
//*****************************************************************************
class VuEventImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuEventImageEntity() : mpTextureAsset(VUNULL) {}

	virtual void onGameInitialize()
	{
		VuGameImageBaseEntity::onGameInitialize();

		const std::string &assetName = "UI/SelectionIcons/Track_" + VuGameUtil::IF()->getTrack();
		if ( VuAssetFactory::IF()->doesAssetExist<VuTextureAsset>(assetName) )
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName);
	}
	virtual void onGameRelease()
	{
		VuGameImageBaseEntity::onGameRelease();

		VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
	}

	virtual VuTexture	*getTexture() { return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL; }

private:
	VuTextureAsset		*mpTextureAsset;
};

IMPLEMENT_RTTI(VuEventImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuEventImageEntity);


//*****************************************************************************
// Game Type Image Element
//*****************************************************************************
class VuGameTypeImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuGameTypeImageEntity() : mpTextureAsset(VUNULL) {}

	virtual void onGameInitialize()
	{
		VuGameImageBaseEntity::onGameInitialize();

		const std::string &assetName = "UI/SelectionIcons/Event_" + VuGameUtil::IF()->getGameType();
		if ( VuAssetFactory::IF()->doesAssetExist<VuTextureAsset>(assetName) )
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName);
	}
	virtual void onGameRelease()
	{
		VuGameImageBaseEntity::onGameRelease();

		VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
	}

	virtual VuTexture	*getTexture() { return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL; }

private:
	VuTextureAsset		*mpTextureAsset;
};

IMPLEMENT_RTTI(VuGameTypeImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameTypeImageEntity);


//*****************************************************************************
// Base Stage Image Element
//*****************************************************************************
class VuBaseStageImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuBaseStageImageEntity();
	~VuBaseStageImageEntity();

	virtual VuTexture	*getTexture();
	virtual int			getStage() = 0;

private:
	typedef std::vector<VuTextureAsset *> Images;
	Images		mImages;
};

IMPLEMENT_RTTI(VuBaseStageImageEntity, VuGameImageBaseEntity);

//*****************************************************************************
VuBaseStageImageEntity::VuBaseStageImageEntity()
{
	for ( int stage = 0; ; stage++ )
	{
		char assetName[64];
		VU_SPRINTF(assetName, sizeof(assetName), "UI/Icon/Stage%d", stage);

		if ( VuAssetFactory::IF()->doesAssetExist<VuTextureAsset>(assetName) )
			mImages.push_back(VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName));
		else
			break;
	}
}

//*****************************************************************************
VuBaseStageImageEntity::~VuBaseStageImageEntity()
{
	for ( auto pTextureAsset : mImages )
		VuAssetFactory::IF()->releaseAsset(pTextureAsset);
}

//*****************************************************************************
VuTexture *VuBaseStageImageEntity::getTexture()
{
	int stage = getStage();

	if ( stage >= 0 && stage < (int)mImages.size() )
		return mImages[stage]->getTexture();

	return VUNULL;
}


//*****************************************************************************
// Series Stage Image Element
//*****************************************************************************
class VuSeriesStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
		return VuGameUtil::IF()->seriesDB()[seriesName]["Stage"].asInt();
	}
};

IMPLEMENT_RTTI(VuSeriesStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSeriesStageImageEntity);


//*****************************************************************************
// Champ Stage Image Element
//*****************************************************************************
class VuChampStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		return VuGameUtil::IF()->getChampStage();
	}
};

IMPLEMENT_RTTI(VuChampStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuChampStageImageEntity);


//*****************************************************************************
// VuCurrentStageImageEntity
//*****************************************************************************
class VuCurrentStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		return VuGameManager::IF() ? VuGameManager::IF()->getCar(VuGameUtil::IF()->getSelectedCar()).mStage : 0;
	}
};

IMPLEMENT_RTTI(VuCurrentStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCurrentStageImageEntity);


//*****************************************************************************
// VuNextStageImageEntity
//*****************************************************************************
class VuNextStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		return VuGameManager::IF() ? VuGameManager::IF()->getCar(VuGameUtil::IF()->getSelectedCar()).mStage + 1 : 1;
	}
};

IMPLEMENT_RTTI(VuNextStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuNextStageImageEntity);


//*****************************************************************************
// Quick Race Stage Image Element
//*****************************************************************************
class VuQuickRaceStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		return VuGameUtil::IF()->dataRead()["QuickRace"]["Stage"].asInt();
	}
};

IMPLEMENT_RTTI(VuQuickRaceStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuQuickRaceStageImageEntity);


//*****************************************************************************
// Car Champ Stage Image Element
//*****************************************************************************
class VuCarChampStageImageEntity : public VuBaseStageImageEntity
{
	DECLARE_RTTI

public:
	virtual int getStage()
	{
		return VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();
	}
};

IMPLEMENT_RTTI(VuCarChampStageImageEntity, VuBaseStageImageEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampStageImageEntity);


//*****************************************************************************
// Next Event Image Element
//*****************************************************************************
class VuNextEventImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuNextEventImageEntity() : mpTextureAsset(VUNULL) {}

	virtual void onGameInitialize()
	{
		VuGameImageBaseEntity::onGameInitialize();

		const std::string &champName = VuGameUtil::IF()->getChampName();
		int eventNumber = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();
		int nextEventNumber = eventNumber + 1;
		const std::string &trackName = VuGameUtil::IF()->champDB()[champName]["Events"][nextEventNumber].asString();

		const std::string &assetName = "UI/SelectionIcons/Track_" + trackName;
		if ( VuAssetFactory::IF()->doesAssetExist<VuTextureAsset>(assetName) )
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName);
	}
	virtual void onGameRelease()
	{
		VuGameImageBaseEntity::onGameRelease();

		VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
	}

	virtual VuTexture	*getTexture() { return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL; }

private:
	VuTextureAsset		*mpTextureAsset;
};

IMPLEMENT_RTTI(VuNextEventImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuNextEventImageEntity);


//*****************************************************************************
// VuCharacterAbilityImageEntity
//*****************************************************************************
class VuCharacterAbilityImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuCharacterAbilityImageEntity() : mpTextureAsset(VUNULL) {}
	~VuCharacterAbilityImageEntity() { VuAssetFactory::IF()->releaseAsset(mpTextureAsset); }

	virtual VuTexture *getTexture()
	{
		const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

		char assetName[256];
		VU_SPRINTF(assetName, sizeof(assetName), "UI/CharacterAbility/%s", selectedDriver.c_str());

		if ( mAssetName != assetName )
		{
			mAssetName = assetName;

			VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(mAssetName, VuAssetFactory::OPTIONAL_ASSET);
		}

		return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL;
	}

private:
	std::string		mAssetName;
	VuTextureAsset	*mpTextureAsset;
};

IMPLEMENT_RTTI(VuCharacterAbilityImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCharacterAbilityImageEntity);


//*****************************************************************************
// VuBossAbilityImageEntity
//*****************************************************************************
class VuBossAbilityImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuBossAbilityImageEntity() : mpTextureAsset(VUNULL) {}
	~VuBossAbilityImageEntity() { VuAssetFactory::IF()->releaseAsset(mpTextureAsset); }

	virtual VuTexture *getTexture()
	{
		const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

		char assetName[256];
		VU_SPRINTF(assetName, sizeof(assetName), "UI/CharacterAbility/%s", bossName);

		if ( mAssetName != assetName )
		{
			mAssetName = assetName;

			VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(mAssetName, VuAssetFactory::OPTIONAL_ASSET);
		}

		return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL;
	}

private:
	std::string		mAssetName;
	VuTextureAsset	*mpTextureAsset;
};

IMPLEMENT_RTTI(VuBossAbilityImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBossAbilityImageEntity);


//*****************************************************************************
// VuBossImageEntity
//*****************************************************************************
class VuBossImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuBossImageEntity() : mpTextureAsset(VUNULL) {}
	~VuBossImageEntity() { VuAssetFactory::IF()->releaseAsset(mpTextureAsset); }

	virtual VuTexture *getTexture()
	{
		const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

		char assetName[256];
		VU_SPRINTF(assetName, sizeof(assetName), "UI/Icon/Driver_%s", bossName);

		if ( mAssetName != assetName )
		{
			mAssetName = assetName;

			VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
			mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(mAssetName, VuAssetFactory::OPTIONAL_ASSET);
		}

		return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL;
	}

private:
	std::string		mAssetName;
	VuTextureAsset	*mpTextureAsset;
};

IMPLEMENT_RTTI(VuBossImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBossImageEntity);


//*****************************************************************************
// VuTuneUpButtonEntity
//*****************************************************************************
class VuTuneUpButtonEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTuneUpButtonEntity();

	virtual void			onGameInitialize() { mVisible = mInitiallyVisible; }

protected:
	// scripting
	VuRetVal				Show(const VuParams &params) { mVisible = true; return VuRetVal(); }
	VuRetVal				Hide(const VuParams &params) { mVisible = false; return VuRetVal(); }
	VuRetVal				Apply(const VuParams &params) { applyTuneUp(); return VuRetVal(); }

	// event handlers
	void					OnUITick(const VuParams &params);
	void					OnUITouch(const VuParams &params);
	void					OnUIDraw(const VuParams &params);

	void					drawLayout(bool bSelected);

	bool					isVisibleAndActive() { return mVisible && (mpTransitionComponent->getState() == VuTransitionBaseComponent::STATE_ACTIVE); }

	void					applyTuneUp();

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	bool						mInitiallyVisible;
	VuUIRectProperties			mImageRect;
	VuUIRectProperties			mTouchRect;
	VuUIRectProperties			mTextRect;
	VuUIRectProperties			mIconRect;
	std::string					mFont;
	VuUIStringFormatProperties	mStringFormat;
	VuUIImageProperties			mImage;
	VuUIImageProperties			mTouchIcon;
	VuUIImageProperties			mGamePadIcon;
	VuUIImageProperties			mRemoteIcon;
	VuUIImageProperties			mKeyboardIcon;
	VuUIAnchorProperties		mAnchor;

	bool						mVisible;

	bool						mNewTouch;
	bool						mTouchDown;
	bool						mTouchUp;
	VuVector2					mTouchPos;

	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState				mPressedState;
	float						mPressedScale;
};

IMPLEMENT_RTTI(VuTuneUpButtonEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTuneUpButtonEntity);


//*****************************************************************************
VuTuneUpButtonEntity::VuTuneUpButtonEntity():
	mInitiallyVisible(true),
	mVisible(true),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mPressedState(NOT_PRESSED)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuTuneUpButtonEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuBoolProperty("Visible", mInitiallyVisible));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuRectProperty("Icon Rect", mIconRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	mImage.addProperties(getProperties(), "Image");
	mTouchIcon.addProperties(getProperties(), "Touch Icon");
	mGamePadIcon.addProperties(getProperties(), "GamePad Icon");
	mRemoteIcon.addProperties(getProperties(), "Remote Icon");
	mKeyboardIcon.addProperties(getProperties(), "Keyboard Icon");
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTuneUpButtonEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTuneUpButtonEntity, Hide);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTuneUpButtonEntity, Apply);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnApplied);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnNeedMore);

	// event handlers
	REG_EVENT_HANDLER(VuTuneUpButtonEntity, OnUITick);
	REG_EVENT_HANDLER(VuTuneUpButtonEntity, OnUITouch);
	REG_EVENT_HANDLER(VuTuneUpButtonEntity, OnUIDraw);

	// constants
	mPressedScale = 1.0f - 0.01f*VuGameUtil::IF()->constantDB()["UI"]["PressedButtonSizeReduction"].asFloat();
}

//*****************************************************************************
void VuTuneUpButtonEntity::applyTuneUp()
{
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);

		// make sure car is not already tuned up
		if ( !pCar->isTunedUp() )
		{
			// attempt to use tune-up
			if ( VuGameManager::IF()->useTuneUp() )
			{
				pCar->applyTuneUp();
				mpScriptComponent->getPlug("OnApplied")->execute();
			}
			else
			{
				mpScriptComponent->getPlug("OnNeedMore")->execute();
			}
		}
	}
}

//*****************************************************************************
void VuTuneUpButtonEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	if ( isVisibleAndActive() )
	{
		bool pressed = false;

		// handle touch input
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect touchRect = uiDrawParams.transform(mTouchRect);
		mAnchor.apply(touchRect, touchRect);
		if ( mNewTouch )
		{
			if ( touchRect.contains(mTouchPos) )
			{
				mPressedState = PRESSED;
			}
		}
		if ( mPressedState == PRESSED )
		{
			if ( mTouchUp )
			{
				pressed = true;

				mPressedState = NOT_PRESSED;
			}
			if ( !touchRect.contains(mTouchPos) )
			{
				mPressedState = NOT_PRESSED;
			}
		}

		if ( mTouchUp )
		{
			mTouchDown = false;
			mTouchUp = false;
		}
		mNewTouch = false;

		// handle gamepad/remote/keyboard input
		if ( (VuTouch::IF()->getFocusPriority() == 0) && VuControlMethodManager::IF()->isGamePad() )
		{
			int padIndex = VuControlMethodManager::IF()->recentPadIndex();
			if ( VuInputManager::IF()->getButtonWasPressed(padIndex, "TuneUp") )
			{
				pressed = true;
			}
		}

		if ( pressed )
		{
			applyTuneUp();
		}
	}
}

//*****************************************************************************
void VuTuneUpButtonEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();
		
	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( isVisibleAndActive() && !mTouchDown )
		{
			mTouchDown = true;
			mNewTouch = true;
			mTouchPos = touch;
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mTouchUp = true;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mTouchPos = touch;
	}
}

//*****************************************************************************
void VuTuneUpButtonEntity::OnUIDraw(const VuParams &params)
{
	if ( !mVisible )
		return;

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	float alpha = mpTransitionComponent->alpha();
	float depth = uiDrawParams.mDepth;
	VuColor color(255, 255, 255, VuRound(255*alpha));

	// image
	if ( VuTexture *pTexture = mImage.getTexture() )
	{
		VuRect dstRect = mImageRect;
		if ( mPressedState == PRESSED )
			dstRect.scale(mImageRect.getCenter(), mPressedScale);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VuGfxUtil::IF()->drawTexture2d(depth, pTexture, color, dstRect);
		depth -= GFX_SORT_DEPTH_STEP;
	}

	// icon
	{
		VuTexture *pTexture = VUNULL;
		if ( VuControlMethodManager::IF()->isGamePad() )
		{
			int padIndex = VuControlMethodManager::IF()->recentPadIndex();
			VuGamePad::eDeviceType deviceType = VuGamePad::IF()->getController(padIndex).mDeviceType;

			if ( deviceType == VuGamePad::DEVICE_REMOTE_CONTROL )
				pTexture = mRemoteIcon.getTexture();
			else
				pTexture = mGamePadIcon.getTexture();
		}
		else if ( VuControlMethodManager::IF()->isKeyboard() )
		{
			pTexture = mKeyboardIcon.getTexture();
		}
		else
		{
			pTexture = mTouchIcon.getTexture();
		}

		if ( pTexture )
		{
			VuRect dstRect = mIconRect;
			if ( mPressedState == PRESSED )
				dstRect.scale(mImageRect.getCenter(), mPressedScale);

			dstRect = uiDrawParams.transform(dstRect);
			mAnchor.apply(dstRect, dstRect);

			VuGfxUtil::IF()->drawTexture2d(depth, pTexture, color, dstRect);
			depth -= GFX_SORT_DEPTH_STEP;
		}
	}

	// text
	if ( int count = VuGameManager::IF()->getTuneUps() )
	{
		char strText[16];
		VU_SPRINTF(strText, sizeof(strText), "%d", count);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuFontDrawParams fdParams = fontEntry.params();
		VuRect dstRect = mTextRect;

		if ( mPressedState == PRESSED )
		{
			fdParams.mSize *= mPressedScale;
			dstRect.scale(mImageRect.getCenter(), mPressedScale);
		}
		fdParams.mSize *= 0.5f*(mpTransformComponent->getWorldScale().mX + mpTransformComponent->getWorldScale().mY);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strText, fdParams, dstRect, mStringFormat, alpha);

		depth -= GFX_SORT_DEPTH_STEP;
	}
}

//*****************************************************************************
void VuTuneUpButtonEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	float depth = uiDrawParams.mDepth;

	if ( VuTexture *pTexture = mImage.getTexture() )
	{
		VuGfxUtil::IF()->drawTexture2d(depth, pTexture, uiDrawParams.transform(mImageRect));
		depth -= GFX_SORT_DEPTH_STEP;
	}

	if ( VuTexture *pTexture = mTouchIcon.getTexture() )
	{
		VuGfxUtil::IF()->drawTexture2d(depth, pTexture, uiDrawParams.transform(mIconRect));
		depth -= GFX_SORT_DEPTH_STEP;
	}

	if ( bSelected )
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuGfxUtil::IF()->drawRectangleOutline2d(depth, fontEntry.params().mColor, uiDrawParams.transform(mTouchRect));
		depth -= GFX_SORT_DEPTH_STEP;
	}
}
