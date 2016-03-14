//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuSplitScreenConfigEntity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/Parts/VuCarWheel.h"
#include "VuPurple/Entities/Car/VuUiCar.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Model/VuModelSkin.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuFSM.h"


class VuSplitScreenConfigEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSplitScreenConfigEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

private:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIGamePad(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);

	void				updateCamera(float fdt);
	VuRect				calcScreenRect();
	void				drawText(const char *text);

	const std::string	&getNextName(const VuJsonContainer &names, const std::string &curName);
	const std::string	&getPrevName(const VuJsonContainer &names, const std::string &curName);

	// FSM
	void				onSelectCarEnter();
	void				onSelectCarExit();
	void				onSelectCarTick(float fdt);

	void				onSelectDriverEnter();
	void				onSelectDriverExit();
	void				onSelectDriverTick(float fdt);

	void				onSelectSkinEnter();
	void				onSelectSkinExit();
	void				onSelectSkinTick(float fdt);

	void				onReadyEnter();
	void				onReadyExit();

	void				onLeaveEnter();
	void				onLeaveExit();

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int							mViewport;
	VuRect						mRect;
	VuVector3					mCameraOffset;
	VuUIRectProperties			mTextRect;
	std::string					mFont;
	VuUIStringFormatProperties	mStringFormat;
	std::string					mReadyStringId;
	std::string					mLeaveStringId;
	float						mCarRotationRate;
	float						mCarFadeRate;
	std::string					mAnimationAssetName;

	VuFSM				mFSM;
	int					mPadIndex;
	int					mStage;
	float				mAlpha;
	float				mCarRotation;
	VuCamera			mCamera;
	bool				mInputNext;
	bool				mInputPrev;

	std::string			mSelectedCar;
	std::string			mSelectedSkin;
	std::string			mSelectedDriver;

	float				mCarAlpha;
	float				mCarTargetAlpha;
	float				mDriverAlpha;
	float				mDriverTargetAlpha;

	std::string			mText;

	VuUiCar				mUiCar;
};

IMPLEMENT_RTTI(VuSplitScreenConfigEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSplitScreenConfigEntity);


//*****************************************************************************
VuSplitScreenConfigEntity::VuSplitScreenConfigEntity():
	mUiCar(false),
	mViewport(0),
	mRect(50,50,100,100),
	mCameraOffset(0, -4, 1),
	mCarRotationRate(VU_PIDIV4),
	mPadIndex(0),
	mStage(0),
	mAlpha(1.0f),
	mCarRotation(0.0f),
	mCarFadeRate(2.0f),
	mInputNext(false),
	mInputPrev(false),
	mCarAlpha(0.0f),
	mCarTargetAlpha(0.0f),
	mDriverAlpha(0.0f),
	mDriverTargetAlpha(0.0f)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuSplitScreenConfigEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuVector3Property("Camera Offset", mCameraOffset));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	addProperty(new VuStringProperty("Ready String ID", mReadyStringId));
	addProperty(new VuStringProperty("Leave String ID", mLeaveStringId));
	addProperty(new VuStringProperty("Leave String ID", mLeaveStringId));
	addProperty(new VuAngleProperty("Car Rotation Rate", mCarRotationRate));
	addProperty(new VuFloatProperty("Car Fade Rate", mCarFadeRate));
	addProperty(new VuAssetProperty<VuAnimationAsset>("Animation Asset", mAnimationAssetName));

	// event handlers
	REG_EVENT_HANDLER(VuSplitScreenConfigEntity, OnUITick);
	REG_EVENT_HANDLER(VuSplitScreenConfigEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuSplitScreenConfigEntity, OnUIDraw);

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCarBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCarEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSkinBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSkinEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReadyBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReadyEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnLeaveBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnLeaveEnd);

	// create FSM
	{
		VuFSM::VuState *pState;
		
		pState = mFSM.addState("Initial");

		pState = mFSM.addState("SelectCar");
		pState->setEnterMethod(this, &VuSplitScreenConfigEntity::onSelectCarEnter);
		pState->setExitMethod(this, &VuSplitScreenConfigEntity::onSelectCarExit);
		pState->setTickMethod(this, &VuSplitScreenConfigEntity::onSelectCarTick);

		pState = mFSM.addState("SelectDriver");
		pState->setEnterMethod(this, &VuSplitScreenConfigEntity::onSelectDriverEnter);
		pState->setExitMethod(this, &VuSplitScreenConfigEntity::onSelectDriverExit);
		pState->setTickMethod(this, &VuSplitScreenConfigEntity::onSelectDriverTick);

		pState = mFSM.addState("SelectSkin");
		pState->setEnterMethod(this, &VuSplitScreenConfigEntity::onSelectSkinEnter);
		pState->setExitMethod(this, &VuSplitScreenConfigEntity::onSelectSkinExit);
		pState->setTickMethod(this, &VuSplitScreenConfigEntity::onSelectSkinTick);

		pState = mFSM.addState("Ready");
		pState->setEnterMethod(this, &VuSplitScreenConfigEntity::onReadyEnter);
		pState->setExitMethod(this, &VuSplitScreenConfigEntity::onReadyExit);

		pState = mFSM.addState("Leave");
		pState->setEnterMethod(this, &VuSplitScreenConfigEntity::onLeaveEnter);
		pState->setExitMethod(this, &VuSplitScreenConfigEntity::onLeaveExit);

		// transitions
		mFSM.addTransition("Initial", "SelectCar", "");

		mFSM.addTransition("SelectCar", "SelectDriver", "OnSelect");
		mFSM.addTransition("SelectCar", "Leave", "OnBack");

		mFSM.addTransition("SelectDriver", "SelectSkin", "OnSelect");
		mFSM.addTransition("SelectDriver", "SelectCar", "OnBack");

		mFSM.addTransition("SelectSkin", "Ready", "OnSelect");
		mFSM.addTransition("SelectSkin", "SelectDriver", "OnBack");

		mFSM.addTransition("Ready", "SelectSkin", "OnBack");

		mFSM.addTransition("Leave", "SelectCar", "OnSelect");
	}
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onGameInitialize()
{
	// determine pad index
	mPadIndex = VuGameUtil::IF()->dataRead()["SplitScreenPads"][mViewport].asInt();

	// determine car stage
	mStage = VuGameUtil::IF()->getChampStage();

	// get state
	mSelectedCar = VuGameUtil::IF()->dataRead()["SplitScreenConfig"][mViewport]["Car"].asString();
	mSelectedDriver = VuGameUtil::IF()->dataRead()["SplitScreenConfig"][mViewport]["Driver"].asString();
	mSelectedSkin = VuGameUtil::IF()->dataRead()["SplitScreenConfig"][mViewport]["Skin"].asString();

	if ( !VuGameUtil::IF()->carDB().hasMember(mSelectedCar) )
		mSelectedCar = VuGameUtil::IF()->constantDB()["Names"]["Cars"][0].asString();

	if ( !VuGameUtil::IF()->driverDB().hasMember(mSelectedDriver) )
		mSelectedDriver = VuGameUtil::IF()->constantDB()["Names"]["Drivers"][0].asString();

	while ( VuGameUtil::IF()->isDriverClaimed(mSelectedDriver) )
		mSelectedDriver = getNextName(VuGameUtil::IF()->constantDB()["Names"]["Drivers"], mSelectedDriver);

	if ( !VuGameUtil::IF()->carSkinDB().hasMember(mSelectedSkin) )
		mSelectedSkin = VuGameUtil::IF()->constantDB()["Names"]["CarSkins"][0].asString();

	VuGameUtil::IF()->claimDriver(mSelectedDriver);

	mFSM.begin();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onGameRelease()
{
	VuGameUtil::IF()->dataWrite()["SplitScreenConfig"][mViewport]["Car"].putValue(mSelectedCar);
	VuGameUtil::IF()->dataWrite()["SplitScreenConfig"][mViewport]["Driver"].putValue(mSelectedDriver);
	VuGameUtil::IF()->dataWrite()["SplitScreenConfig"][mViewport]["Skin"].putValue(mSelectedSkin);

	mFSM.end();

	VuGameUtil::IF()->discardDriver(mSelectedDriver);
}

//*****************************************************************************
void VuSplitScreenConfigEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mFSM.evaluate();
	mFSM.tick(fdt);

	mCarRotation = VuAngClamp(mCarRotation + mCarRotationRate*fdt);

	mUiCar.setCar(mSelectedCar, mStage);

	if ( mSelectedSkin.empty() )
		mUiCar.setSkin(mSelectedCar); // factory paint job
	else
		mUiCar.setSkin(mSelectedSkin);

	mUiCar.setDriver(mSelectedDriver);
	mUiCar.setDriverAnim(mAnimationAssetName);

	VuMatrix transform;
	transform.loadIdentity();
	transform.rotateZLocal(mCarRotation);

	mUiCar.setTransform(transform);
	mUiCar.tickDecision(fdt);
	mUiCar.tickAnim(fdt);

	updateCamera(fdt);

	mInputNext = false;
	mInputPrev = false;

	mCarAlpha = VuMathUtil::chase(mCarAlpha, mCarTargetAlpha, mCarFadeRate*fdt);
	mDriverAlpha = VuMathUtil::chase(mDriverAlpha, mDriverTargetAlpha, mCarFadeRate*fdt);
}

//*****************************************************************************
void VuSplitScreenConfigEntity::OnUIGamePad(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int channel = accessor.getInt();
	bool down = accessor.getBool();
	int padIndex = accessor.getInt();

	if ( padIndex == mPadIndex )
	{
		if ( down )
		{
			if ( channel == VuUIInputUtil::CHANNEL_LEFT )
			{
				mInputPrev = true;
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				mInputNext = true;
			}
			else if ( channel == VuUIInputUtil::CHANNEL_SELECT )
			{
				mFSM.pulseCondition("OnSelect");
			}
			else if ( channel == VuUIInputUtil::CHANNEL_BACK )
			{
				mFSM.pulseCondition("OnBack");
			}
		}
	}
}

//*****************************************************************************
void VuSplitScreenConfigEntity::OnUIDraw(const VuParams &params)
{
	mAlpha = 1.0f;

	drawText(mText.c_str());
	
	// use alternate viewport to draw car/driver
	int vp = VuGfxSort::IF()->getViewport();
	VuGfxSort::IF()->setViewport(mViewport + 1);

	VuRect rect = calcScreenRect();

	VuGfxUtil::IF()->submitSetViewportCommand(rect);
	VuGfxSort::IF()->submitCamera(mCamera);

	mUiCar.setCarColor(VuColor(VuVector4(1,1,1,mCarAlpha*mAlpha)));
	mUiCar.setDriverColor(VuColor(VuVector4(1,1,1,mDriverAlpha*mAlpha)));
	mUiCar.draw(VuGfxDrawParams(mCamera));

	VuGfxSort::IF()->setViewport(vp);
}

//*****************************************************************************
void VuSplitScreenConfigEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), uiDrawParams.transform(mRect));

	drawText("Selection");

	// draw rect
	if ( bSelected )
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, uiDrawParams.transform(mTextRect));
	}
}

//*****************************************************************************
void VuSplitScreenConfigEntity::updateCamera(float fdt)
{
	VuVector3 target(mUiCar.getAabb().getCenter());

	mCamera.setViewMatrix(target + mCameraOffset, target, VuVector3(0,0,1));

	VuRect rect = calcScreenRect();

	// update camera
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio()*rect.mWidth/rect.mHeight;
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	float nearDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraNearDist"].asFloat();
	float farDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraFarDist"].asFloat();
	mCamera.setProjMatrixVert(vertFov, aspectRatio, nearDist, farDist);
}

//*****************************************************************************
VuRect VuSplitScreenConfigEntity::calcScreenRect()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	VuRect rect = uiDrawParams.transform(mRect);

	VuVector2 v0 = VuUI::IF()->getCropMatrix().transform(rect.getTopLeft());
	VuVector2 v1 = VuUI::IF()->getCropMatrix().transform(rect.getBottomRight());

	rect.mX = v0.mX;
	rect.mY = v0.mY;
	rect.mWidth = v1.mX - v0.mX;
	rect.mHeight = v1.mY - v0.mY;

	return rect;
}

//*****************************************************************************
void VuSplitScreenConfigEntity::drawText(const char *text)
{
	if ( text && text[0] )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);

		VuRect rect = uiDrawParams.transform(mTextRect);

		VuGfxUtil::IF()->fontDraw()->drawString(
			uiDrawParams.mDepth,
			fontEntry.font(),
			text,
			fontEntry.params(),
			rect,
			mStringFormat,
			mAlpha
		);
	}
}

//*****************************************************************************
const std::string &VuSplitScreenConfigEntity::getNextName(const VuJsonContainer &names, const std::string &curName)
{
	int next = 0;
	for ( int i = 0; i < names.size(); i++ )
	{
		if ( names[i].asString() == curName )
		{
			next = i + 1;
			break;
		}
	}

	if ( next >= names.size() )
		next = 0;

	return names[next].asString();
}

//*****************************************************************************
const std::string &VuSplitScreenConfigEntity::getPrevName(const VuJsonContainer &names, const std::string &curName)
{
	int prev = 0;
	for ( int i = 0; i < names.size(); i++ )
	{
		if ( names[i].asString() == curName )
		{
			prev = i - 1;
			break;
		}
	}

	if ( prev < 0 )
		prev = names.size() - 1;

	return names[prev].asString();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectCarEnter()
{
	mCarTargetAlpha = 1.0f;
	mDriverTargetAlpha = 0.0f;
	mpScriptComponent->getPlug("OnCarBegin")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectCarExit()
{
	mpScriptComponent->getPlug("OnCarEnd")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectCarTick(float fdt)
{
	if ( mInputNext )
	{
		mSelectedCar = getNextName(VuGameUtil::IF()->constantDB()["Names"]["Cars"], mSelectedCar);
		mSelectedSkin = mSelectedCar;
	}

	if ( mInputPrev )
	{
		mSelectedCar = getPrevName(VuGameUtil::IF()->constantDB()["Names"]["Cars"], mSelectedCar);
		mSelectedSkin = mSelectedCar;
	}

	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Car_%s", mSelectedCar.c_str());
	mText = VuStringDB::IF()->getString(stringId).c_str();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectDriverEnter()
{
	mDriverTargetAlpha = 1.0f;
	mpScriptComponent->getPlug("OnDriverBegin")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectDriverExit()
{
	mpScriptComponent->getPlug("OnDriverEnd")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectDriverTick(float fdt)
{
	if ( mInputNext )
	{
		VuGameUtil::IF()->discardDriver(mSelectedDriver);
		do {
			mSelectedDriver = getNextName(VuGameUtil::IF()->constantDB()["Names"]["Drivers"], mSelectedDriver);
		} while ( VuGameUtil::IF()->isDriverClaimed(mSelectedDriver) );
		VuGameUtil::IF()->claimDriver(mSelectedDriver);
	}

	if ( mInputPrev )
	{
		VuGameUtil::IF()->discardDriver(mSelectedDriver);
		do {
			mSelectedDriver = getPrevName(VuGameUtil::IF()->constantDB()["Names"]["Drivers"], mSelectedDriver);
		} while ( VuGameUtil::IF()->isDriverClaimed(mSelectedDriver) );
		VuGameUtil::IF()->claimDriver(mSelectedDriver);
	}

	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Driver_%s", mSelectedDriver.c_str());
	mText = VuStringDB::IF()->getString(stringId).c_str();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectSkinEnter()
{
	mpScriptComponent->getPlug("OnSkinBegin")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectSkinExit()
{
	mpScriptComponent->getPlug("OnSkinEnd")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onSelectSkinTick(float fdt)
{
	if ( mInputNext )
		mSelectedSkin = getNextName(VuGameUtil::IF()->constantDB()["Names"]["CarSkins"], mSelectedSkin);

	if ( mInputPrev )
		mSelectedSkin = getPrevName(VuGameUtil::IF()->constantDB()["Names"]["CarSkins"], mSelectedSkin);

	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "CarSkin_%s", mSelectedSkin.c_str());
	mText = VuStringDB::IF()->getString(stringId).c_str();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onReadyEnter()
{
	mText = VuStringDB::IF()->getString(mReadyStringId);

	mpScriptComponent->getPlug("OnReadyBegin")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onReadyExit()
{
	mpScriptComponent->getPlug("OnReadyEnd")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onLeaveEnter()
{
	mText = VuStringDB::IF()->getString(mLeaveStringId);
	mCarTargetAlpha = 0.0f;

	mpScriptComponent->getPlug("OnLeaveBegin")->execute();
}

//*****************************************************************************
void VuSplitScreenConfigEntity::onLeaveExit()
{
	mpScriptComponent->getPlug("OnLeaveEnd")->execute();
}
