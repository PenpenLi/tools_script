//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Achievement entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/VuGfxDrawParams.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Util/VuTimeUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


#define REWARD_IMAGE_COUNT 7


class VuCalendarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCalendarEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	VuRetVal			StartPfx(const VuParams &params);

	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawCalendar(float alpha);

	void				drawTile(int row, int dayOfWeek, int year, int month, int day, const std::tm &localTime, float alpha, const VuUIDrawParams &uiDrawParams);
	void				nextTile(int &row, int &dayOfWeek, int &day);
	VuTexture			*calcRewardImage(int year, int month, int day);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int							mCalendar;
	VuUIRectProperties			mTileRect;
	VuVector2					mSpacing;
	VuUIRectProperties			mDayRect;
	std::string					mDayFont;
	VuUIStringFormatProperties	mDayStringFormat;
	VuColor						mThisMonthTextColor;
	VuColor						mOtherMonthTextColor;
	VuUIImageProperties			mTodayImage;
	VuUIImageProperties			mThisMonthImage;
	VuUIImageProperties			mOtherMonthImage;
	VuUIImageProperties			mRewardImages[REWARD_IMAGE_COUNT];
	std::string					mPfxName;
	VuVector2					mPfxPos;
	float						mPfxDist;

	VuPfxSystemInstance	*mpPfxSystemInstance;
	VuVector2			mTodayOffset;
};


IMPLEMENT_RTTI(VuCalendarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCalendarEntity);


//*****************************************************************************
VuCalendarEntity::VuCalendarEntity():
	mCalendar(VuGameManager::DAILY_CHALLENGE_CALENDAR),
	mSpacing(0.0f,0.0f),
	mThisMonthTextColor(128,128,128),
	mOtherMonthTextColor(0,0,0),
	mPfxPos(0.0f,0.0f),
	mPfxDist(10.0f),
	mpPfxSystemInstance(VUNULL),
	mTodayOffset(0.0f,0.0f)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuCalendarEntity::drawLayout));
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	static VuStaticIntEnumProperty::Choice sCalendarChoices[] =
	{
		{ "Daily Challenge", VuGameManager::DAILY_CHALLENGE_CALENDAR },
		{ "Daily Reward", VuGameManager::DAILY_REWARD_CALENDAR },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Calendar", mCalendar, sCalendarChoices));
	addProperty(new VuRectProperty("Tile Rect", mTileRect));
	addProperty(new VuVector2Property("Spacing", mSpacing));
	addProperty(new VuRectProperty("Day Rect", mDayRect));
	addProperty(new VuFontEnumProperty("Day Font", mDayFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mDayStringFormat, "Day ");

	addProperty(new VuColorProperty("This Month Text Color", mThisMonthTextColor));
	addProperty(new VuColorProperty("Other Month Text Color", mOtherMonthTextColor));

	mTodayImage.addProperties(getProperties(), "Today Image");
	mThisMonthImage.addProperties(getProperties(), "This Month Image");
	mOtherMonthImage.addProperties(getProperties(), "Other Month Image");
	mRewardImages[0].addProperties(getProperties(), "Reward Image 1");
	mRewardImages[1].addProperties(getProperties(), "Reward Image 2");
	mRewardImages[2].addProperties(getProperties(), "Reward Image 3");
	mRewardImages[3].addProperties(getProperties(), "Reward Image 4");
	mRewardImages[4].addProperties(getProperties(), "Reward Image 5");
	mRewardImages[5].addProperties(getProperties(), "Reward Image 6");
	mRewardImages[6].addProperties(getProperties(), "Reward Image 7");

	addProperty(new VuStringProperty("Pfx Name", mPfxName));
	addProperty(new VuVector2Property("Pfx Pos", mPfxPos));
	addProperty(new VuFloatProperty("Pfx Dist", mPfxDist));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCalendarEntity, StartPfx);

	// event handlers
	REG_EVENT_HANDLER(VuCalendarEntity, OnUITick);
	REG_EVENT_HANDLER(VuCalendarEntity, OnUIDraw);
}

//*****************************************************************************
void VuCalendarEntity::onGameInitialize()
{
	mpPfxSystemInstance = VuPfx::IF()->createSystemInstance(mPfxName.c_str());
}

//*****************************************************************************
void VuCalendarEntity::onGameRelease()
{
	if ( mpPfxSystemInstance )
	{
		VuPfx::IF()->releaseSystemInstance(mpPfxSystemInstance);
		mpPfxSystemInstance = VUNULL;
	}
}

//*****************************************************************************
VuRetVal VuCalendarEntity::StartPfx(const VuParams &params)
{
	if ( mpPfxSystemInstance )
		mpPfxSystemInstance->start();

	return VuRetVal();
}

//*****************************************************************************
void VuCalendarEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( mpPfxSystemInstance )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuCamera &camera = VuViewportManager::IF()->getUiCamera();
		VuVector2 screenPos = uiDrawParams.transform(mPfxPos + mTodayOffset);
		float dist = (mPfxDist - camera.getNearPlane())/(camera.getFarPlane() - camera.getNearPlane());
		VuVector3 worldPos = camera.screenToWorld(VuVector3(screenPos.mX, screenPos.mY, dist));

		VuMatrix transform = camera.getTransform();
		transform.setTrans(worldPos);
		mpPfxSystemInstance->setMatrix(transform);

		mpPfxSystemInstance->tick(fdt, true);
	}
}

//*****************************************************************************
void VuCalendarEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawCalendar(alpha);
}

//*****************************************************************************
void VuCalendarEntity::drawLayout(bool bSelected)
{
	drawCalendar(1.0f);
}

//*****************************************************************************
void VuCalendarEntity::drawCalendar(float alpha)
{
	std::time_t rawTime;
	std::time(&rawTime);

	std::tm localTime;
	VU_LOCALTIME(&rawTime, &localTime);

	int year = localTime.tm_year + 1900;
	int month = localTime.tm_mon; // 0-11
	int day = localTime.tm_mday; // 1-31
	int dayOfWeek = (localTime.tm_wday + 6)%7; // we want Monday = 0

	int nextMonth = month + 1;
	int nextYear = year;
	if ( nextMonth > 11 )
	{
		nextMonth = 0;
		nextYear++;
	}

	int prevMonth = month - 1;
	int prevYear = year;
	if ( prevMonth < 0 )
	{
		nextMonth = 11;
		nextYear--;
	}

	int daysInThisMonth = VuTimeUtil::calcDaysInMonth(year, month);
	int daysInPrevMonth = VuTimeUtil::calcDaysInMonth(prevYear, prevMonth);

	int lastDayOfWeek = (dayOfWeek + daysInThisMonth - day)%7;
	int visibleDaysNextMonth = 6 - lastDayOfWeek;

	int curRow = 5;
	int curDayOfWeek = 6;
	int curDay = visibleDaysNextMonth;

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	// draw next month
	for ( int i = 0; i < visibleDaysNextMonth; i++ )
	{
		drawTile(curRow, curDayOfWeek, nextYear, nextMonth, curDay, localTime, alpha, uiDrawParams);
		nextTile(curRow, curDayOfWeek, curDay);
	}

	// draw current month
	curDay = daysInThisMonth;
	for ( int i = 0; i < daysInThisMonth; i++ )
	{
		drawTile(curRow, curDayOfWeek, year, month, curDay, localTime, alpha, uiDrawParams);
		nextTile(curRow, curDayOfWeek, curDay);
	}

	// draw prev month
	curDay = daysInPrevMonth;
	while ( curRow >= 0 )
	{
		drawTile(curRow, curDayOfWeek, prevYear, prevMonth, curDay, localTime, alpha, uiDrawParams);
		nextTile(curRow, curDayOfWeek, curDay);
	}

	// pfx
	if ( mpPfxSystemInstance )
	{
		const VuCamera &camera = VuViewportManager::IF()->getUiCamera();
		mpPfxSystemInstance->draw(VuGfxDrawParams(camera));
	}
}

//*****************************************************************************
void VuCalendarEntity::drawTile(int row, int dayOfWeek, int year, int month, int day, const std::tm &localTime, float alpha, const VuUIDrawParams &uiDrawParams)
{
	VuVector2 offset = VuVector2(dayOfWeek, row)*mSpacing;

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mDayFont);
	VuFontDrawParams fdParams = fontEntry.params();

	VuUIImageProperties *pImage = &mOtherMonthImage;
	fdParams.mColor = mOtherMonthTextColor;

	if ( month == localTime.tm_mon )
	{
		if ( day == localTime.tm_mday )
		{
			pImage = &mTodayImage;
			fdParams.mColor = fontEntry.params().mColor;
			mTodayOffset = offset;
		}
		else
		{
			pImage = &mThisMonthImage;
			fdParams.mColor = mThisMonthTextColor;
		}
	}

	float depth = uiDrawParams.mDepth;

	// draw background image
	{
		VuRect dstRect = uiDrawParams.transform(mTileRect + offset);
		VuColor color(255,255,255);
		color.mA = (VUUINT8)VuRound(alpha*255.0f);
		VuGfxUtil::IF()->drawTexture2d(depth, pImage->getTexture(), color, dstRect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// draw reward image
	if ( VuTexture *pRewardTexture = calcRewardImage(year, month, day) )
	{
		VuRect dstRect = uiDrawParams.transform(mTileRect + offset);
		VuColor color(255,255,255);
		color.mA = (VUUINT8)VuRound(alpha*255.0f);
		VuGfxUtil::IF()->drawTexture2d(depth, pRewardTexture, color, dstRect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// draw text
	{
		char str[16];
		VU_SPRINTF(str, sizeof(str), "%d", day);
		VuRect dstRect = uiDrawParams.transform(mDayRect + offset);
		VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(),str, fdParams, dstRect, mDayStringFormat, alpha);
	}
	depth -= GFX_SORT_DEPTH_STEP;
}

//*****************************************************************************
void VuCalendarEntity::nextTile(int &row, int &dayOfWeek, int &day)
{
	day--;
	dayOfWeek--;
	if ( dayOfWeek < 0 )
	{
		dayOfWeek = 6;
		row--;
	}
}

//*****************************************************************************
VuTexture *VuCalendarEntity::calcRewardImage(int year, int month, int day)
{
	if ( VuGameManager::IF() )
	{
		int daysSince2000 = VuTimeUtil::calcDaysSince2000(year, month, day);
		int daysInRow = VuGameManager::IF()->getCalendarDaysInRow(VuGameManager::eCalendars(mCalendar), daysSince2000);
		if ( daysInRow > 0 )
		{
			int imageIndex = (daysInRow - 1)%REWARD_IMAGE_COUNT;
			return mRewardImages[imageIndex].getTexture();
		}
	}

	return VUNULL;
}


//*****************************************************************************
// VuCheckCalendarEntity
//*****************************************************************************
class VuCheckCalendarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCheckCalendarEntity();

private:
	VuRetVal IsTodayDone(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mCalendar;
};

IMPLEMENT_RTTI(VuCheckCalendarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckCalendarEntity);

//*****************************************************************************
VuCheckCalendarEntity::VuCheckCalendarEntity():
	mCalendar(VuGameManager::DAILY_CHALLENGE_CALENDAR)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	static VuStaticIntEnumProperty::Choice sCalendarChoices[] =
	{
		{ "Daily Challenge", VuGameManager::DAILY_CHALLENGE_CALENDAR },
		{ "Daily Reward", VuGameManager::DAILY_REWARD_CALENDAR },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Calendar", mCalendar, sCalendarChoices));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckCalendarEntity, IsTodayDone, VuRetVal::Bool, VuParamDecl());

}

//*****************************************************************************
VuRetVal VuCheckCalendarEntity::IsTodayDone(const VuParams &params)
{
	int daysSince2000 = VuTimeUtil::calcDaysSince2000();
	bool isDone = VuGameManager::IF()->getCalendarDay(VuGameManager::eCalendars(mCalendar), daysSince2000);
	return VuRetVal(isDone);
}
