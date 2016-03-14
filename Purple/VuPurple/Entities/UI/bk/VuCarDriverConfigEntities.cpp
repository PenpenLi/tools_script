//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarDriverConfigEntity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuUiCar.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuFSM.h"


class VuCarDriverConfigEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCarDriverConfigEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

private:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	// scripting
	VuRetVal			Select(const VuParams &params);
	VuRetVal			Back(const VuParams &params);
	VuRetVal			NextItem(const VuParams &params);
	VuRetVal			PrevItem(const VuParams &params);
	VuRetVal			NeedToBuyCar(const VuParams &params);
	VuRetVal			NeedToUpgradeCar(const VuParams &params);
	VuRetVal			NeedToLevelUpCar(const VuParams &params);
	VuRetVal			NeedToBuyDriver(const VuParams &params);
	VuRetVal			LevelUpCar(const VuParams &params);
	VuRetVal			BuyDriver(const VuParams &params);

	void				drawLayout(bool bSelected);

	void				updateCamera(float fdt);
	VuRect				calcScreenRect();

	const std::string	&getNextName(const VuJsonContainer &names, const std::string &curName);
	const std::string	&getPrevName(const VuJsonContainer &names, const std::string &curName);

	// FSM
	void				onSelectCarEnter();
	void				onSelectCarExit();
	void				onSelectCarTick(float fdt);

	void				onSelectDriverEnter();
	void				onSelectDriverExit();
	void				onSelectDriverTick(float fdt);

	void				onReadyEnter();

	void				onLeaveEnter();

	enum { QUICK_RACE_MODE, CAR_CHAMP_MODE};

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	int						mMode;
	VuRect					mRect;
	VuVector3				mCameraOffset;
	float					mCarRotationRate;
	float					mCarFadeRate;
	std::string				mAnimationAssetName;
	float					mLockedAlpha;
	VuUIAnchorProperties	mAnchor;

	VuFSM				mFSM;
	int					mStage;
	float				mAlpha;
	float				mCarRotation;
	VuCamera			mCamera;
	bool				mInputNext;
	bool				mInputPrev;

	std::string			mSelectedCar;
	std::string			mSelectedDriver;

	float				mCarAlpha;
	float				mCarTargetAlpha;
	float				mDriverAlpha;
	float				mDriverTargetAlpha;

	VuUiCar				mUiCar;
};

IMPLEMENT_RTTI(VuCarDriverConfigEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarDriverConfigEntity);


//*****************************************************************************
VuCarDriverConfigEntity::VuCarDriverConfigEntity():
	mUiCar(false),
	mMode(QUICK_RACE_MODE),
	mRect(50,50,100,100),
	mCameraOffset(0, -4, 1),
	mCarRotationRate(VU_PIDIV4),
	mStage(0),
	mAlpha(1.0f),
	mCarRotation(0.0f),
	mCarFadeRate(2.0f),
	mLockedAlpha(0.5f),
	mInputNext(false),
	mInputPrev(false),
	mCarAlpha(0.0f),
	mCarTargetAlpha(0.0f),
	mDriverAlpha(0.0f),
	mDriverTargetAlpha(0.0f)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuCarDriverConfigEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	static VuStaticIntEnumProperty::Choice sModeChoices[] = { { "Quick Race", QUICK_RACE_MODE }, { "Car Champ", CAR_CHAMP_MODE }, { VUNULL } };
	addProperty(new VuStaticIntEnumProperty("Mode", mMode, sModeChoices));
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuVector3Property("Camera Offset", mCameraOffset));
	addProperty(new VuAngleProperty("Car Rotation Rate", mCarRotationRate));
	addProperty(new VuFloatProperty("Car Fade Rate", mCarFadeRate));
	addProperty(new VuAssetProperty<VuAnimationAsset>("Animation Asset", mAnimationAssetName));
	addProperty(new VuFloatProperty("Locked Alpha", mLockedAlpha));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// event handlers
	REG_EVENT_HANDLER(VuCarDriverConfigEntity, OnUITick);
	REG_EVENT_HANDLER(VuCarDriverConfigEntity, OnUIDraw);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, Select);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, Back);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, NextItem);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, PrevItem);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarDriverConfigEntity, NeedToBuyCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarDriverConfigEntity, NeedToUpgradeCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarDriverConfigEntity, NeedToLevelUpCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarDriverConfigEntity, NeedToBuyDriver, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, LevelUpCar);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarDriverConfigEntity, BuyDriver);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCarBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCarEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCarSwitched);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverBegin);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverEnd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverSwitched);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReady);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnLeave);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LevelUpSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LevelUpFailure);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, BuyDriverSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, BuyDriverFailure);

	// create FSM
	{
		VuFSM::VuState *pState;
		
		pState = mFSM.addState("Initial");

		pState = mFSM.addState("SelectCar");
		pState->setEnterMethod(this, &VuCarDriverConfigEntity::onSelectCarEnter);
		pState->setExitMethod(this, &VuCarDriverConfigEntity::onSelectCarExit);
		pState->setTickMethod(this, &VuCarDriverConfigEntity::onSelectCarTick);

		pState = mFSM.addState("SelectDriver");
		pState->setEnterMethod(this, &VuCarDriverConfigEntity::onSelectDriverEnter);
		pState->setExitMethod(this, &VuCarDriverConfigEntity::onSelectDriverExit);
		pState->setTickMethod(this, &VuCarDriverConfigEntity::onSelectDriverTick);

		pState = mFSM.addState("Ready");
		pState->setEnterMethod(this, &VuCarDriverConfigEntity::onReadyEnter);

		pState = mFSM.addState("Leave");
		pState->setEnterMethod(this, &VuCarDriverConfigEntity::onLeaveEnter);

		// transitions
		mFSM.addTransition("Initial", "SelectDriver", "CarChamp");
		mFSM.addTransition("Initial", "SelectCar", "");

		mFSM.addTransition("SelectCar", "SelectDriver", "OnSelect");
		mFSM.addTransition("SelectCar", "Leave", "OnBack");

		mFSM.addTransition("SelectDriver", "Ready", "OnSelect");
		mFSM.addTransition("SelectDriver", "Leave", "OnBack & CarChamp");
		mFSM.addTransition("SelectDriver", "SelectCar", "OnBack");
	}
}

//*****************************************************************************
void VuCarDriverConfigEntity::onGameInitialize()
{
	// read state
	if ( mMode == QUICK_RACE_MODE )
	{
		mStage = VuGameUtil::IF()->dataRead()["QuickRace"]["Stage"].asInt();
		mSelectedCar = VuGameUtil::IF()->dataRead()["QuickRace"]["Car"].asString();
		mSelectedDriver = VuGameUtil::IF()->dataRead()["QuickRace"]["Driver"].asString();
	}
	else if ( mMode == CAR_CHAMP_MODE )
	{
		mStage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();
		mSelectedCar = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
		mSelectedDriver = VuGameUtil::IF()->dataRead()["CarChamp"]["Driver"].asString();
	}

	if ( !VuGameUtil::IF()->carDB().hasMember(mSelectedCar) )
		mSelectedCar = VuGameUtil::IF()->constantDB()["Names"]["Cars"][0].asString();

	if ( !VuGameUtil::IF()->driverDB().hasMember(mSelectedDriver) )
		mSelectedDriver = VuGameUtil::IF()->constantDB()["Names"]["Drivers"][0].asString();

	mFSM.begin();

	mFSM.setCondition("QuickRace", mMode == QUICK_RACE_MODE);
	mFSM.setCondition("CarChamp", mMode == CAR_CHAMP_MODE);
}

//*****************************************************************************
void VuCarDriverConfigEntity::onGameRelease()
{
	mFSM.end();
}

//*****************************************************************************
void VuCarDriverConfigEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	mFSM.evaluate();
	mFSM.tick(fdt);

	mCarRotation = VuAngClamp(mCarRotation + mCarRotationRate*fdt);

	const VuGameManager::Car &car = VuGameManager::IF()->getCar(mSelectedCar);

	mUiCar.setCar(mSelectedCar, mStage, car.mIsGold);
	mUiCar.setSkin(car.mDecal, car.mPaintColor, car.mDecalColor);
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

	mCarAlpha = mpTransitionComponent->alpha()*VuMathUtil::chase(mCarAlpha, mCarTargetAlpha, mCarFadeRate*fdt);
	mDriverAlpha = mpTransitionComponent->alpha()*VuMathUtil::chase(mDriverAlpha, mDriverTargetAlpha, mCarFadeRate*fdt);
}

//*****************************************************************************
void VuCarDriverConfigEntity::OnUIDraw(const VuParams &params)
{
	mAlpha = 1.0f;

	// use alternate viewport to draw car/driver
	int vp = VuGfxSort::IF()->getViewport();
	VuGfxSort::IF()->setViewport(1);

	VuRect rect = calcScreenRect();

	VuGfxUtil::IF()->submitSetViewportCommand(rect);
	VuGfxSort::IF()->submitCamera(mCamera);

	mUiCar.setCarColor(VuColor(VuVector4(1,1,1,mCarAlpha*mAlpha)));
	mUiCar.setDriverColor(VuColor(VuVector4(1,1,1,mDriverAlpha*mAlpha)));
	mUiCar.draw(VuGfxDrawParams(mCamera));

	VuGfxSort::IF()->setViewport(2);

	VuGfxUtil::IF()->submitSetViewportCommand(VuRect(0,0,1,1));
	VuGfxSort::IF()->submitCamera(VuViewportManager::IF()->getUiCamera());

	VuGfxSort::IF()->setViewport(vp);
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::Select(const VuParams &params)
{
	mFSM.pulseCondition("OnSelect");
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::Back(const VuParams &params)
{
	mFSM.pulseCondition("OnBack");
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::NextItem(const VuParams &params)
{
	mInputNext = true;
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::PrevItem(const VuParams &params)
{
	mInputPrev = true;
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::NeedToBuyCar(const VuParams &params)
{
	if ( VuGameUtil::IF()->isDemoMode() )
		return VuRetVal(false);

	if ( mFSM.getCurState()->getName() != "SelectCar" )
		return VuRetVal(false);

	if ( VuGameManager::IF()->getCar(mSelectedCar).mIsOwned )
		return VuRetVal(false);

	return VuRetVal(true);
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::NeedToUpgradeCar(const VuParams &params)
{
	if ( VuGameUtil::IF()->isDemoMode() )
		return VuRetVal(false);

	if ( mFSM.getCurState()->getName() != "SelectCar" )
		return VuRetVal(false);

	const VuGameManager::Car &car = VuGameManager::IF()->getCar(mSelectedCar);

	if ( car.mStage >= mStage )
		return VuRetVal(false);

	if ( (car.mStage == mStage - 1) && car.areStatsMaxed() )
		return VuRetVal(false);

	return VuRetVal(true);
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::NeedToLevelUpCar(const VuParams &params)
{
	if ( VuGameUtil::IF()->isDemoMode() )
		return VuRetVal(false);

	if ( mFSM.getCurState()->getName() != "SelectCar" )
		return VuRetVal(false);

	if ( VuGameManager::IF()->getCar(mSelectedCar).mStage >= mStage )
		return VuRetVal(false);

	return VuRetVal(true);
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::NeedToBuyDriver(const VuParams &params)
{
	if ( VuGameUtil::IF()->isDemoMode() )
		return VuRetVal(false);

	if ( mFSM.getCurState()->getName() != "SelectDriver" )
		return VuRetVal(false);

	if ( VuGameManager::IF()->getDriver(mSelectedDriver).isOnTeam() )
		return VuRetVal(false);

	return VuRetVal(true);
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::LevelUpCar(const VuParams &params)
{
	if ( VuGameManager::IF()->purchaseCarLevelUp(mSelectedCar, mStage) )
	{
		mpScriptComponent->getPlug("LevelUpSuccess")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("LevelUpFailure")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarDriverConfigEntity::BuyDriver(const VuParams &params)
{
	if ( VuGameManager::IF()->purchaseDriver(mSelectedDriver) )
	{
		mpScriptComponent->getPlug("BuyDriverSuccess")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("BuyDriverFailure")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
void VuCarDriverConfigEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), uiDrawParams.transform(mRect));
}

//*****************************************************************************
void VuCarDriverConfigEntity::updateCamera(float fdt)
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
VuRect VuCarDriverConfigEntity::calcScreenRect()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	VuRect rect = uiDrawParams.transform(mRect);
	mAnchor.apply(rect, rect);

	VuVector2 v0 = VuUI::IF()->getCropMatrix().transform(rect.getTopLeft());
	VuVector2 v1 = VuUI::IF()->getCropMatrix().transform(rect.getBottomRight());

	rect.mX = v0.mX;
	rect.mY = v0.mY;
	rect.mWidth = v1.mX - v0.mX;
	rect.mHeight = v1.mY - v0.mY;

	return rect;
}

//*****************************************************************************
const std::string &VuCarDriverConfigEntity::getNextName(const VuJsonContainer &names, const std::string &curName)
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
const std::string &VuCarDriverConfigEntity::getPrevName(const VuJsonContainer &names, const std::string &curName)
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
void VuCarDriverConfigEntity::onSelectCarEnter()
{
	mpScriptComponent->getPlug("OnCarBegin")->execute();
}

//*****************************************************************************
void VuCarDriverConfigEntity::onSelectCarExit()
{
	// write state
	if ( mMode == QUICK_RACE_MODE )
		VuGameUtil::IF()->dataWrite()["QuickRace"]["Car"].putValue(mSelectedCar);

	mpScriptComponent->getPlug("OnCarEnd")->execute();
}

//*****************************************************************************
void VuCarDriverConfigEntity::onSelectCarTick(float fdt)
{
	if ( mInputNext )
	{
		mSelectedCar = getNextName(VuGameUtil::IF()->constantDB()["Names"]["Cars"], mSelectedCar);
		mpScriptComponent->getPlug("OnCarSwitched")->execute();
	}

	if ( mInputPrev )
	{
		mSelectedCar = getPrevName(VuGameUtil::IF()->constantDB()["Names"]["Cars"], mSelectedCar);
		mpScriptComponent->getPlug("OnCarSwitched")->execute();
	}

	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Car_%s", mSelectedCar.c_str());
	VuGameFontMacros::IF()->setMacro("CONFIG_CAR_NAME", VuStringDB::IF()->getString(stringId).c_str());

	char strPrice[32];
	VuStringUtil::integerFormat(VuGameUtil::IF()->getCarLevelUpPrice(mSelectedCar, mStage), strPrice, sizeof(strPrice));
	VuGameFontMacros::IF()->setMacro("CONFIG_CAR_PRICE", strPrice);

	const VuGameManager::Car &car = VuGameManager::IF()->getCar(mSelectedCar);
	mCarTargetAlpha = (car.mIsOwned && car.mStage >= mStage) ? 1.0f : mLockedAlpha;
	mDriverTargetAlpha = 0.0f;

	if ( VuGameUtil::IF()->isDemoMode() )
		mCarTargetAlpha = 1.0f;
}

//*****************************************************************************
void VuCarDriverConfigEntity::onSelectDriverEnter()
{
	mpScriptComponent->getPlug("OnDriverBegin")->execute();
}

//*****************************************************************************
void VuCarDriverConfigEntity::onSelectDriverExit()
{
	// write state
	if ( mMode == QUICK_RACE_MODE )
		VuGameUtil::IF()->dataWrite()["QuickRace"]["Driver"].putValue(mSelectedDriver);
	else if ( mMode == CAR_CHAMP_MODE )
		VuGameUtil::IF()->dataWrite()["CarChamp"]["Driver"].putValue(mSelectedDriver);

	mpScriptComponent->getPlug("OnDriverEnd")->execute();
}

//*****************************************************************************
void VuCarDriverConfigEntity::onSelectDriverTick(float fdt)
{
	if ( mInputNext )
	{
		mSelectedDriver = getNextName(VuGameUtil::IF()->constantDB()["Names"]["Drivers"], mSelectedDriver);
		mpScriptComponent->getPlug("OnDriverSwitched")->execute();
	}

	if ( mInputPrev )
	{
		mSelectedDriver = getPrevName(VuGameUtil::IF()->constantDB()["Names"]["Drivers"], mSelectedDriver);
		mpScriptComponent->getPlug("OnDriverSwitched")->execute();
	}

	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Driver_%s", mSelectedDriver.c_str());
	VuGameFontMacros::IF()->setMacro("CONFIG_DRIVER_NAME", VuStringDB::IF()->getString(stringId).c_str());

	char strPrice[32];
	VuStringUtil::integerFormat(VuGameUtil::IF()->getDriverPrice(mSelectedDriver), strPrice, sizeof(strPrice));
	VuGameFontMacros::IF()->setMacro("CONFIG_DRIVER_PRICE", strPrice);

	mCarTargetAlpha = 1.0f;
	mDriverTargetAlpha = VuGameManager::IF()->getDriver(mSelectedDriver).isOnTeam() ? 1.0f : mLockedAlpha;

	if ( VuGameUtil::IF()->isDemoMode() )
		mDriverTargetAlpha = 1.0f;
}

//*****************************************************************************
void VuCarDriverConfigEntity::onReadyEnter()
{
	mpScriptComponent->getPlug("OnReady")->execute();
}

//*****************************************************************************
void VuCarDriverConfigEntity::onLeaveEnter()
{
	mpScriptComponent->getPlug("OnLeave")->execute();
}
