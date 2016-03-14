//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Text entity
// 
//*****************************************************************************

#include "VuGameTextEntity.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Assets/VuFontAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuEngine/VuEngine.h"

#include "VuPurple/Managers/VuPackManager.h"


//*****************************************************************************
// VuGameTextBaseEntity
//*****************************************************************************

IMPLEMENT_RTTI(VuGameTextBaseEntity, VuEntity);

//*****************************************************************************
VuGameTextBaseEntity::VuGameTextBaseEntity():
	mInitiallyVisible(true),
	mRect(0,0,100,100),
	mOffset(0,0),
	mAlpha(1.0f),
	mVisible(true)
{
	// properties
	addProperty(new VuBoolProperty("Visible", mInitiallyVisible));
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	addComponent(new Vu2dLayoutComponent(this, &VuGameTextBaseEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameTextBaseEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameTextBaseEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameTextBaseEntity, SetAlpha, VuRetVal::Void, VuParamDecl(1, VuParams::Float));

	// event handlers
	REG_EVENT_HANDLER(VuGameTextBaseEntity, OnUITick);
	REG_EVENT_HANDLER(VuGameTextBaseEntity, OnUIDraw);
}

//*****************************************************************************
void VuGameTextBaseEntity::onGameInitialize()
{
	mVisible = mInitiallyVisible;
}

//*****************************************************************************
void VuGameTextBaseEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);
}

//*****************************************************************************
void VuGameTextBaseEntity::OnUIDraw(const VuParams &params)
{
	if ( mVisible )
		drawText(getText(), mpTransitionComponent->alpha());
}

//*****************************************************************************
VuRetVal VuGameTextBaseEntity::SetAlpha(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mAlpha = accessor.getFloat();

	return VuRetVal();
}

//*****************************************************************************
void VuGameTextBaseEntity::drawLayout(bool bSelected)
{
	// draw rect
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, uiDrawParams.transform(mRect));
	}

	// draw text
	drawText(getText(), 1.0f);
}

//*****************************************************************************
VuVector2 VuGameTextBaseEntity::measureString()
{
	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
	const char *text = getText();
	return VuFontDraw::measureString(fontEntry.font(), text, fontEntry.params(), mRect.mWidth, mStringFormat, VuUI::IF()->getAuthoringAspectRatio());
}

//*****************************************************************************
void VuGameTextBaseEntity::drawText(const char *text, float alpha)
{
	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
	if (!text)
	{
		return;
	}
	if ( text[0] )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect rect = uiDrawParams.transform(mRect);
		mAnchor.apply(rect, rect);

		VuFontDrawParams fdParams = fontEntry.params();
		fdParams.mSize *= 0.5f*(uiDrawParams.mLocalScale.mX + uiDrawParams.mLocalScale.mY);

		VuGfxUtil::IF()->fontDraw()->drawString(
			uiDrawParams.mDepth,
			fontEntry.font(),
			text,
			fdParams,
			rect,
			mStringFormat,
			alpha*mAlpha,
			mOffset*uiDrawParams.mLocalScale*uiDrawParams.mInvAuthScale
		);
	}
}


//*****************************************************************************
// VuGameTextEntity
//*****************************************************************************

class VuGameTextEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuGameTextEntity();

	const char	*getText();

protected:

	VuRetVal	SetStringID(const VuParams &params);

	std::string	mStringID;
	std::string	mGamePadStringID;
	std::string	mKeyboardStringID;
};

IMPLEMENT_RTTI(VuGameTextEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameTextEntity);

//*****************************************************************************
VuGameTextEntity::VuGameTextEntity()
{
	// properties
	addProperty(new VuStringProperty("String ID", mStringID));
	addProperty(new VuStringProperty("Game Pad String ID", mGamePadStringID));
	addProperty(new VuStringProperty("Keyboard String ID", mKeyboardStringID));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameTextEntity, SetStringID, VuRetVal::Void, VuParamDecl(1, VuParams::String));
}

//*****************************************************************************
const char *VuGameTextEntity::getText()
{
	if ( VuControlMethodManager::IF() )
	{
		if ( !mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD) )
			return VuStringDB::IF()->getStringSelf(mGamePadStringID).c_str();

		if ( !mKeyboardStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_KEYBOARD) )
			return VuStringDB::IF()->getStringSelf(mKeyboardStringID).c_str();
	}

	return VuStringDB::IF()->getStringSelf(mStringID).c_str();
}

//*****************************************************************************
VuRetVal VuGameTextEntity::SetStringID(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mStringID = accessor.getString();

	return VuRetVal();
}


//*****************************************************************************
// VuGameTextEntity
//*****************************************************************************

class VuGameAlternativeTextEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuGameAlternativeTextEntity();

	const char	*getText();

protected:

	VuRetVal	SetStringID(const VuParams &params);

	std::string	mDefaultStringID;
/*
	std::string	mGamePadStringID;
	std::string	mKeyboardStringID;*/

	int			mWeekDay;
	std::string mWeekDayStringId;
	
};

IMPLEMENT_RTTI(VuGameAlternativeTextEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameAlternativeTextEntity);

//*****************************************************************************
VuGameAlternativeTextEntity::VuGameAlternativeTextEntity() : mWeekDay(-1)
{
	// properties
	addProperty(new VuStringProperty("DefaultString", mDefaultStringID));
	addProperty(new VuStringProperty("WeekDayString", mWeekDayStringId));
	addProperty(new VuIntProperty("WeekDay",mWeekDay));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameAlternativeTextEntity, SetStringID, VuRetVal::Void, VuParamDecl(1, VuParams::String));
}

//*****************************************************************************
const char *VuGameAlternativeTextEntity::getText()
{
	/*if (VuControlMethodManager::IF())
	{
		if (!mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD))
			return VuStringDB::IF()->getStringSelf(mGamePadStringID).c_str();

		if (!mKeyboardStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_KEYBOARD))
			return VuStringDB::IF()->getStringSelf(mKeyboardStringID).c_str();
	}

	return VuStringDB::IF()->getStringSelf(mStringID).c_str();*/

	//这里就要根据实际情况来判断是星期几G
	

	if (VuEngine::IF()->editorMode())
	{
		return VuStringDB::IF()->getStringSelf(mDefaultStringID).c_str();

	}
	else
	{
		int nWeedDay = VuGameManager::IF()->getWeekDay();
		if (nWeedDay == mWeekDay)
		{
			return VuStringDB::IF()->getStringSelf(mWeekDayStringId).c_str();
		}
		else
			return VuStringDB::IF()->getStringSelf(mDefaultStringID).c_str();

	}
	

}

//*****************************************************************************
VuRetVal VuGameAlternativeTextEntity::SetStringID(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mDefaultStringID = accessor.getString();

	return VuRetVal();
}













//*****************************************************************************
// VuGameTimerTextEntity
//*****************************************************************************
// modify by xlm,[/2015/4/17 ] 

class VuGameTimerTextEntity : public VuGameTextEntity
{
	DECLARE_RTTI

public:
	VuGameTimerTextEntity();

	
	const char	*getText();


protected:

	int getRemainTime();
	const char	*getText_inner();

protected:

	bool		mLoop;  
	int			mDuration;   //

	time_t		mStartTime;

	std::string  mTimeContent;
};

IMPLEMENT_RTTI(VuGameTimerTextEntity, VuGameTextEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameTimerTextEntity);

//*****************************************************************************
VuGameTimerTextEntity::VuGameTimerTextEntity()
{
	time(&mStartTime);

	addProperty(new VuBoolProperty("loop", mLoop));
	addProperty(new VuIntProperty("duration", mDuration));
}

//*****************************************************************************
const char *VuGameTimerTextEntity::getText_inner()
{

	if (VuControlMethodManager::IF())
	{
		if (!mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD))
			return VuStringDB::IF()->getStringSelf(mGamePadStringID).c_str();

		if (!mKeyboardStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_KEYBOARD))
			return VuStringDB::IF()->getStringSelf(mKeyboardStringID).c_str();
	}

	return VuStringDB::IF()->getStringSelf(mStringID).c_str();
}

int VuGameTimerTextEntity::getRemainTime()
{
	
	if (VuEngine::IF()->editorMode())
	{
		return 0;
	}

	
	time_t nowTime;
	time(&nowTime);
	int nSecond = nowTime - VuGameManager::IF()->getGameStartTime();
	/*nSecond = nSecond / 1000;*/

	if (mLoop)
	{
		nSecond = nSecond%mDuration;

	}
	
	return mDuration - nSecond;

}


const char* VuGameTimerTextEntity::getText()
{
	int nRemainTime = getRemainTime();

	if (!mLoop && nRemainTime < 0)
	{
		return mTimeContent.c_str();
	}

	const char* szTempText = getText_inner();
	mTimeContent = szTempText;
	if (szTempText[0])
	{
		char str[256];
		VuStringUtil::timeFormatSeconds(VuStringUtil::HH_MM_SS, (int)nRemainTime, str, sizeof(str));
		VuStringUtil::replace(mTimeContent, "[TIMER_CONTENT]", str);
		return mTimeContent.c_str();
	}
	else
		return NULL;
}


//modify by liudan
class VuPackTimerTextEntity : public VuGameTextEntity
{
	DECLARE_RTTI

public:
	VuPackTimerTextEntity();


	const char	*getText();


protected:

	int getRemainTime();
	const char	*getText_inner();

protected:


	time_t		mStartTime;

	std::string  mTimeContent;

	// properties
	std::string			mType;
};

IMPLEMENT_RTTI(VuPackTimerTextEntity, VuGameTextEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPackTimerTextEntity);

//*****************************************************************************
VuPackTimerTextEntity::VuPackTimerTextEntity()
:mType("LanXiangPack")
{
	time(&mStartTime);

	static VuStaticStringEnumProperty::Choice sTypeChoices[] =
	{
		"LanXiangPack",
		"SuperVipPack",
		VUNULL
	};
	addProperty(new VuStaticStringEnumProperty("Type", mType, sTypeChoices));

}

//*****************************************************************************
const char *VuPackTimerTextEntity::getText_inner()
{

	if (VuControlMethodManager::IF())
	{
		if (!mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD))
			return VuStringDB::IF()->getStringSelf(mGamePadStringID).c_str();

		if (!mKeyboardStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_KEYBOARD))
			return VuStringDB::IF()->getStringSelf(mKeyboardStringID).c_str();
	}

	return VuStringDB::IF()->getStringSelf(mStringID).c_str();
}

int VuPackTimerTextEntity::getRemainTime()
{

	if (VuEngine::IF()->editorMode())
	{
		return 0;
	}

	int nSecond = 0;
	if (mType == "LanXiangPack")
	{
		nSecond = VuPackManager::IF()->getLanXiangDiscountTime();
	}
	else if (mType == "SuperVipPack")
	{
		nSecond = VuPackManager::IF()->getSuperVipDiscountTime();
	}

	return  nSecond;
}


const char* VuPackTimerTextEntity::getText()
{
	int nRemainTime = getRemainTime();

	if (nRemainTime < 0)
	{
		return mTimeContent.c_str();
	}

	const char* szTempText = getText_inner();
	mTimeContent = szTempText;
	if (szTempText[0])
	{
		char str[256];
		VuStringUtil::timeFormatSeconds(VuStringUtil::HH_MM_SS, (int)nRemainTime, str, sizeof(str));
		VuStringUtil::replace(mTimeContent, "[PACK_TIMER_CONTENT]", str);
		return mTimeContent.c_str();
	}
	else
		return NULL;
}
