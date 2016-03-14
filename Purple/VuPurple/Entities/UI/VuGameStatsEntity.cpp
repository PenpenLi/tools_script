//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stats entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuAchievementUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Util/VuStringUtil.h"


class VuGameStatsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameStatsEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawStats(float alpha);

	void				drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags);
	void				drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawRect(const VuRect &rect);

	float				calcScrollSize();
	float				calcScrollMax();

	VuUIAnchor			calcTextAnchor();
	VuUIAnchor			calcScrollBarAnchor();

	void				addDistanceStat(const char *name, float distance);
	void				addTimeStat(const char *name, float time);
	void				addIntStat(const char *name, int value);
	void				addStringStat(const char *name, const char *stringID);
	void				addTextStat(const char *name, const char *text);
	void				addBlankStat();

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	VuUIAnchor					mAnchor;
	VuRect						mRect;
	VuRect						mClipRect;
	float						mSpacing;
	float						mSeparatorSpacing;
	VuRect						mNameRect;
	std::string					mNameFont;
	VuUIStringFormatProperties	mNameStringFormat;
	VuRect						mValueRect;
	std::string					mValueFont;
	VuUIStringFormatProperties	mValueStringFormat;
	float						mScrollLag;
	float						mScrollResidualDamping;
	VuRect						mScrollBarRect;
	VuColor						mScrollBarColor;
	VuColor						mScrollBarBackColor;
	float						mNextPrevScrollSpeed;
	VuUIImageProperties			mBackgroundTexture;
	float						mBackgroundSpeed;
	float						mExtraScrollMax;

	typedef std::pair<std::string, std::string> Stat;
	typedef std::vector<Stat> Stats;
	Stats			mStats;

	enum eState { IDLE, SCROLLING };
	eState			mState;
	VuVector2		mInitialTouchDown;
	float			mScrollPos;
	float			mScrollTargetPos;
	float			mScrollVel;
	float			mScrollResidualVel;
	float			mScrollTouchPos;

	float			mAlpha;
};


IMPLEMENT_RTTI(VuGameStatsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameStatsEntity);


//*****************************************************************************
VuGameStatsEntity::VuGameStatsEntity():
	mRect(0,0,0,0),
	mClipRect(0,0,0,0),
	mSpacing(0),
	mSeparatorSpacing(0),
	mNameRect(0,0,0,0),
	mValueRect(0,0,0,0),
	mScrollLag(0.1f),
	mScrollResidualDamping(0.99f),
	mScrollBarRect(0,0,0,0),
	mScrollBarColor(255,255,255),
	mScrollBarBackColor(0,0,0),
	mNextPrevScrollSpeed(360.0f),
	mBackgroundSpeed(1.0f),
	mExtraScrollMax(0.0f),
	mState(IDLE),
	mScrollPos(0),
	mScrollTargetPos(0),
	mScrollVel(0),
	mScrollResidualVel(0),
	mAlpha(1.0f)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(new Vu2dLayoutComponent(this, &VuGameStatsEntity::drawLayout));

	// properties
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuRectProperty("Clip Rect", mClipRect));
	addProperty(new VuFloatProperty("Spacing", mSpacing));
	addProperty(new VuFloatProperty("SeparatorSpacing", mSeparatorSpacing));

	addProperty(new VuRectProperty("NameRect", mNameRect));
	addProperty(new VuFontEnumProperty("NameFont", mNameFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mNameStringFormat, "Name");

	addProperty(new VuRectProperty("ValueRect", mValueRect));
	addProperty(new VuFontEnumProperty("ValueFont", mValueFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mValueStringFormat, "Value");

	addProperty(new VuFloatProperty("ScrollLag", mScrollLag));
	addProperty(new VuFloatProperty("ScrollResidualDamping", mScrollResidualDamping));
	addProperty(new VuRectProperty("ScrollBarRect", mScrollBarRect));
	addProperty(new VuColorProperty("ScrollBarColor", mScrollBarColor));
	addProperty(new VuColorProperty("ScrollBarBackColor", mScrollBarBackColor));

	addProperty(new VuFloatProperty("NextPrevScrollSpeed", mNextPrevScrollSpeed));

	mBackgroundTexture.addProperties(getProperties(), "BackgroundTexture");
	addProperty(new VuFloatProperty("BackgroundSpeed", mBackgroundSpeed));
	addProperty(new VuFloatProperty("ExtraScrollMax", mExtraScrollMax));

	// event handlers
	REG_EVENT_HANDLER(VuGameStatsEntity, OnUITick);
	REG_EVENT_HANDLER(VuGameStatsEntity, OnUITouch);
	REG_EVENT_HANDLER(VuGameStatsEntity, OnUIDraw);
}

//*****************************************************************************
void VuGameStatsEntity::onGameInitialize()
{
	addIntStat("CareerGamesPlayed", VuStatsManager::IF()->getIntStat("CareerGamesPlayed"));
	addIntStat("CareerWins", VuStatsManager::IF()->getIntStat("CareerWins"));
	addIntStat("CareerPodiums", VuStatsManager::IF()->getIntStat("CareerPodiums"));

	addBlankStat();

	char achievements[32];
	VU_SPRINTF(achievements, sizeof(achievements), "%d/%d", VuAchievementManager::IF()->unlockedCount(), VuAchievementManager::IF()->totalCount());
	addTextStat("Achievements", achievements);

	addBlankStat();

	addTimeStat("TimePlayed", VuStatsManager::IF()->getFloatStat("TotalTime"));
	addDistanceStat("DistanceDriven", VuStatsManager::IF()->getFloatStat("TotalDistance"));
	addDistanceStat("LongestJump", VuStatsManager::IF()->getFloatStat("LongestJump"));

	addBlankStat();

	addStringStat("FavoriteCar", ("Car_" + VuStatsManager::IF()->getFavoriteCar()).c_str());
	addStringStat("FavoriteDriver", ("Driver_" + VuStatsManager::IF()->getFavoriteDriver()).c_str());

	addBlankStat();

	addIntStat("BestShootingGalleryScore", VuStatsManager::IF()->getBestScore("ShootingGallery"));
	addIntStat("BestDemolitionDerbyScore", VuStatsManager::IF()->getBestScore("DemolitionDerby"));
	addIntStat("BestFollowLeaderScore", VuStatsManager::IF()->getBestScore("FollowLeader"));

	addBlankStat();

	addIntStat("SmashedSeagulls", VuStatsManager::IF()->getIntStat("SmashedSeagulls"));
	addIntStat("SmashedPalms", VuStatsManager::IF()->getIntStat("SmashedPalms"));
	addIntStat("SmashedCrabs", VuStatsManager::IF()->getIntStat("SmashedCrabs"));
	addIntStat("SmashedLavaMonsters", VuStatsManager::IF()->getIntStat("SmashedLavaMonsters"));
	addIntStat("SmashedDeathBats", VuStatsManager::IF()->getIntStat("SmashedDeathBats"));
	addIntStat("SmashedYeti", VuStatsManager::IF()->getIntStat("SmashedYeti"));
	addIntStat("SmashedPenguins", VuStatsManager::IF()->getIntStat("SmashedPenguins"));
}

//*****************************************************************************
void VuGameStatsEntity::onGameRelease()
{
	mStats.clear();
}

//*****************************************************************************
void VuGameStatsEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	int padIndex = 0;
	if ( VuInputManager::IF()->getButtonValue(padIndex, "Up") )
		mScrollTargetPos -= mNextPrevScrollSpeed*fdt;
	if ( VuInputManager::IF()->getButtonValue(padIndex, "Down") )
		mScrollTargetPos += mNextPrevScrollSpeed*fdt;

	if ( mState != SCROLLING )
	{
		mScrollResidualVel *= VuPow(1.0f - mScrollResidualDamping, fdt);
		mScrollTargetPos += mScrollResidualVel*fdt;
		mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
	}

	mScrollPos = VuMathUtil::smoothCD(mScrollPos, mScrollTargetPos, mScrollVel, mScrollLag, fdt);
}

//*****************************************************************************
void VuGameStatsEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();
		
	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect screenRect = uiDrawParams.transform(mRect);
		mAnchor.apply(screenRect, screenRect);

		if ( screenRect.contains(touch) )
		{
			mState = SCROLLING;
			mInitialTouchDown = touch;
			mScrollTouchPos = mScrollTargetPos;
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mState = IDLE;
		mScrollResidualVel = mScrollVel;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		if ( mState == SCROLLING )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuVector2 touchDelta = uiDrawParams.transformInv(touch) - uiDrawParams.transformInv(mInitialTouchDown);

			mScrollTargetPos = mScrollTouchPos - touchDelta.mY;
			mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
		}
	}
}

//*****************************************************************************
void VuGameStatsEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawStats(alpha);
}

//*****************************************************************************
void VuGameStatsEntity::drawLayout(bool bSelected)
{
	// generate stats
	if ( mStats.empty() )
		for ( int i = 0; i < 25; i++ )
			mStats.push_back(Stat("Name", "Value"));

	if ( bSelected )
	{
		drawRect(mRect);
		drawRect(mClipRect);
		drawRect(mNameRect);
		drawRect(mValueRect);
	}

	drawStats(1.0f);

	drawRect(mScrollBarRect);
}

//*****************************************************************************
void VuGameStatsEntity::drawStats(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mAlpha = alpha;
	VuUIAnchor textAnchor = calcTextAnchor();
	VuUIAnchor scrollBarAnchor = calcScrollBarAnchor();

	VuVector2 offset(0.0f, -mScrollPos);

	for ( Stats::const_iterator iter = mStats.begin(); iter != mStats.end(); iter++ )
	{
		if ( iter->first.empty() )
		{
			offset.mY += mSeparatorSpacing;
		}
		else
		{
			// name/value
			drawText(iter->first.c_str(), mNameRect + offset, textAnchor, mNameFont, mNameStringFormat);
			drawText(iter->second.c_str(), mValueRect + offset, textAnchor, mValueFont, mValueStringFormat);

			offset.mY += mSpacing;
		}
	}

	float scrollMax = calcScrollMax();
	if ( scrollMax > 0.0f )
	{
		// draw scroll bar
		float scrollRatio = mScrollPos/scrollMax;
		VuRect scrollBarRect = mScrollBarRect;
		VuRect scrollIndicatorRect = scrollBarRect;

		float size = mRect.mHeight/calcScrollSize();
		scrollIndicatorRect.mHeight *= size;
		scrollIndicatorRect.mY += scrollRatio*(scrollBarRect.mHeight - scrollIndicatorRect.mHeight);

		drawFilledRect(scrollIndicatorRect, scrollBarAnchor, mScrollBarColor, -2*GFX_SORT_DEPTH_STEP);
		drawFilledRect(scrollBarRect, scrollBarAnchor, mScrollBarBackColor, -GFX_SORT_DEPTH_STEP);
	}

	if ( VuTexture *pTexture = mBackgroundTexture.getTexture() )
	{
		VuColor color(255,255,255);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mRect);
		mAnchor.apply(dstRect, dstRect);

		VuRect srcRect(0,0,1,1);
		srcRect.mY += mBackgroundSpeed*mScrollPos/mRect.mHeight;

		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2*GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	}
}

//*****************************************************************************
void VuGameStatsEntity::drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(font);

	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);

	VuFontDrawParams params = fontEntry.params();
	params.mClip = true;
	params.mClipRect = uiDrawParams.transform(mClipRect);
	mAnchor.apply(params.mClipRect, params.mClipRect);

	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), text, params, dstRect, flags, mAlpha);
}

//*****************************************************************************
void VuGameStatsEntity::drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	if ( VuTexture *pTexture = image.getTexture() )
	{
		color.mA = VuRound(color.mA*mAlpha);
		if ( color.mA )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect clipRect = uiDrawParams.transform(mClipRect);
			mAnchor.apply(clipRect, clipRect);

			VuRect dstRect = uiDrawParams.transform(rect);
			anchor.apply(dstRect, dstRect);

			VuRect srcRect(0,0,1,1);

			if ( dstRect.intersects(clipRect) )
			{
				VuRect dstRectOrig = dstRect;
				dstRect = VuRect::intersection(clipRect, dstRect);
				srcRect.mWidth = dstRect.mWidth/dstRectOrig.mWidth;
				srcRect.mHeight = dstRect.mHeight/dstRectOrig.mHeight;

				if ( dstRectOrig.getLeft() < clipRect.getLeft() )
					srcRect.mX = 1.0f - srcRect.mWidth;

				if ( dstRectOrig.getTop() < clipRect.getTop() )
					srcRect.mY = 1.0f - srcRect.mHeight;

				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + depth, pTexture, color, dstRect, srcRect);
			}
		}
	}
}

//*****************************************************************************
void VuGameStatsEntity::drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	color.mA = VuRound(color.mA*mAlpha);

	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);

	VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth + depth, color, dstRect);
}

//*****************************************************************************
void VuGameStatsEntity::drawRect(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect dstRect = uiDrawParams.transform(rect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), dstRect);
}

//*****************************************************************************
float VuGameStatsEntity::calcScrollSize()
{
	float size = 0.0f;
	for ( int i = 0; i < (int)mStats.size(); i++ )
		size += mStats[i].first.empty() ? mSeparatorSpacing : mSpacing;

	size += mExtraScrollMax;

	return size;
}

//*****************************************************************************
float VuGameStatsEntity::calcScrollMax()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect rect = uiDrawParams.transform(mRect);
	mAnchor.apply(rect, rect);
	rect = uiDrawParams.transformInv(rect);

	float scrollMax = calcScrollSize() - rect.mHeight;
	scrollMax = VuMax(scrollMax, 0.0f);

	return scrollMax;
}

//*****************************************************************************
VuUIAnchor VuGameStatsEntity::calcTextAnchor()
{
	VuUIAnchor scrollAnchor = mAnchor;

	if ( scrollAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM )
		scrollAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;

	return scrollAnchor;
}

//*****************************************************************************
VuUIAnchor VuGameStatsEntity::calcScrollBarAnchor()
{
	VuUIAnchor scrollAnchor = mAnchor;

	if ( scrollAnchor.mAnchorH == VuUIAnchor::ANCHOR_LEFT_RIGHT )
		scrollAnchor.mAnchorH = VuUIAnchor::ANCHOR_LEFT;

	return scrollAnchor;
}

//*****************************************************************************
void VuGameStatsEntity::addDistanceStat(const char *name, float distance)
{
	char str[64];
	VuGameUtil::distanceFormat(distance, str, sizeof(str));
	addTextStat(name, str);
}

//*****************************************************************************
void VuGameStatsEntity::addTimeStat(const char *name, float time)
{
	char str[64];
	if ( time >= 3600.0f )
		VuStringUtil::timeFormatSeconds(VuStringUtil::HH_MM_SS, time, str, sizeof(str));
	else
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, time, str, sizeof(str));
	addTextStat(name, str);
}

//*****************************************************************************
void VuGameStatsEntity::addIntStat(const char *name, int value)
{
	char str[64];
	VuStringUtil::integerFormat(value, str, sizeof(str));
	addTextStat(name, str);
}

//*****************************************************************************
void VuGameStatsEntity::addStringStat(const char *name, const char *stringID)
{
	const std::string &text = VuStringDB::IF()->getString(stringID);
	addTextStat(name, text.c_str());
}

//*****************************************************************************
void VuGameStatsEntity::addTextStat(const char *name, const char *text)
{
	char stringID[64];
	VU_SPRINTF(stringID, sizeof(stringID), "Stat_%s", name);

	mStats.push_back(Stat(VuStringDB::IF()->getString(stringID), text));
}

//*****************************************************************************
void VuGameStatsEntity::addBlankStat()
{
	mStats.push_back(Stat());
}