//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAchievementsEntity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
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


class VuAchievementsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuAchievementsEntity();
	~VuAchievementsEntity();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawAchievements(float alpha);

	void				drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags);
	void				drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawImage(const VuTextureAsset *pTextureAsset, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawRect(const VuRect &rect);

	float				calcScrollSize();
	float				calcScrollMax();

	VuUIAnchor			calcTextAnchor();
	VuUIAnchor			calcImageAnchor();

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	VuUIAnchor					mAnchor;
	VuUIRectProperties			mRect;
	VuUIRectProperties			mClipRect;
	float						mSpacing;
	VuUIRectProperties			mTitleRect;
	std::string					mTitleFont;
	VuUIStringFormatProperties	mTitleStringFormat;
	VuUIRectProperties			mDescRect;
	std::string					mDescFont;
	VuUIStringFormatProperties	mDescStringFormat;
	std::string					mPCStringID;
	VuUIRectProperties			mPCRect;
	std::string					mPCFont;
	VuUIStringFormatProperties	mPCStringFormat;
	VuUIRectProperties			mProgressRect;
	std::string					mProgressFont;
	VuUIStringFormatProperties	mProgressStringFormat;
	VuUIRectProperties			mImageRect;
	float						mScrollLag;
	float						mScrollResidualDamping;
	VuUIRectProperties			mScrollBarRect;
	VuColor						mScrollBarColor;
	VuColor						mScrollBarBackColor;
	float						mNextPrevScrollSpeed;
	VuUIImageProperties			mBackgroundTexture;
	float						mBackgroundSpeed;
	float						mExtraScrollMax;
	VuUIImageProperties			mLockedImage;
	VuUIImageProperties			mUnlockedImage;

	struct Achievement
	{
		Achievement(int id, int pc) : mId(id), mPC(pc) {}
		int		mId;
		int		mPC;
	};
	typedef std::deque<Achievement *> Achievements;
	Achievements	mAchievements;

	enum eState { IDLE, SCROLLING };
	eState			mState;
	VuVector2		mInitialTouchDown;
	float			mScrollPos;
	float			mScrollTargetPos;
	float			mScrollVel;
	float			mScrollResidualVel;
	float			mScrollTouchPos;

	float			mAlpha;
	std::string		mText;
};


IMPLEMENT_RTTI(VuAchievementsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAchievementsEntity);


//*****************************************************************************
VuAchievementsEntity::VuAchievementsEntity():
	mSpacing(0),
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
	addComponent(new Vu2dLayoutComponent(this, &VuAchievementsEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuRectProperty("Clip Rect", mClipRect));
	addProperty(new VuFloatProperty("Spacing", mSpacing));

	addProperty(new VuRectProperty("TitleRect", mTitleRect));
	addProperty(new VuFontEnumProperty("TitleFont", mTitleFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mTitleStringFormat, "Title");

	addProperty(new VuRectProperty("DescRect", mDescRect));
	addProperty(new VuFontEnumProperty("DescFont", mDescFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mDescStringFormat, "Desc");

	addProperty(new VuStringProperty("PCStringID", mPCStringID));
	addProperty(new VuRectProperty("PCRect", mPCRect));
	addProperty(new VuFontEnumProperty("PCFont", mPCFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mPCStringFormat, "PC");

	addProperty(new VuRectProperty("ProgressRect", mProgressRect));
	addProperty(new VuFontEnumProperty("ProgressFont", mProgressFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mProgressStringFormat, "Progress");

	addProperty(new VuRectProperty("ImageRect", mImageRect));

	addProperty(new VuFloatProperty("ScrollLag", mScrollLag));
	addProperty(new VuFloatProperty("ScrollResidualDamping", mScrollResidualDamping));
	addProperty(new VuRectProperty("ScrollBarRect", mScrollBarRect));
	addProperty(new VuColorProperty("ScrollBarColor", mScrollBarColor));
	addProperty(new VuColorProperty("ScrollBarBackColor", mScrollBarBackColor));

	addProperty(new VuFloatProperty("NextPrevScrollSpeed", mNextPrevScrollSpeed));

	mBackgroundTexture.addProperties(getProperties(), "BackgroundTexture");
	addProperty(new VuFloatProperty("BackgroundSpeed", mBackgroundSpeed));
	addProperty(new VuFloatProperty("ExtraScrollMax", mExtraScrollMax));

	mLockedImage.addProperties(getProperties(), "LockedImage");
	mUnlockedImage.addProperties(getProperties(), "UnlockedImage");

	// event handlers
	REG_EVENT_HANDLER(VuAchievementsEntity, OnUITick);
	REG_EVENT_HANDLER(VuAchievementsEntity, OnUITouch);
	REG_EVENT_HANDLER(VuAchievementsEntity, OnUIDraw);

	if ( VuAchievementManager::IF() )
	{
		for ( const auto &ach : VuAchievementManager::IF()->getAchievements() )
		{
			mAchievements.push_back(new Achievement(ach.mId, ach.mPC));
		}
	}
}

//*****************************************************************************
VuAchievementsEntity::~VuAchievementsEntity()
{
	for ( const Achievement *pAchievement : mAchievements )
		delete pAchievement;
}

//*****************************************************************************
void VuAchievementsEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

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
void VuAchievementsEntity::OnUITouch(const VuParams &params)
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
void VuAchievementsEntity::OnUIDraw(const VuParams &params)
{
	drawAchievements(mpTransitionComponent->alpha());
}

//*****************************************************************************
void VuAchievementsEntity::drawLayout(bool bSelected)
{
	// generate stats
	if ( mAchievements.empty() )
	{
		if ( VuSpreadsheetAsset *pSA = VuAssetFactory::IF()->createAsset<VuSpreadsheetAsset>("Achievements") )
		{
			for ( int iRow = 0; iRow < pSA->getRowCount(); iRow++ )
				mAchievements.push_back(new Achievement(pSA->getField(iRow, "ID").asInt(), pSA->getField(iRow, "Points").asInt()));

			VuAssetFactory::IF()->releaseAsset(pSA);
		}
	}

	if ( bSelected )
	{
		drawRect(mRect);
		drawRect(mClipRect);
		drawRect(mTitleRect);
		drawRect(mDescRect);
		drawRect(mProgressRect);
	}

	drawAchievements(1.0f);

	drawRect(mScrollBarRect);
}

//*****************************************************************************
void VuAchievementsEntity::drawAchievements(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mAlpha = alpha;
	VuUIAnchor textAnchor = calcTextAnchor();
	VuUIAnchor imageAnchor = calcImageAnchor();

	VuVector2 offset(0.0f, -mScrollPos);

	for ( const Achievement *pAchievement : mAchievements )
	{
		float progress = 0.5f;
		if ( VuAchievementManager::IF() )
			progress = VuAchievementManager::IF()->getProgress(pAchievement->mId);

		char str[128];

		// title
		VU_SPRINTF(str, sizeof(str), "Ach%02d", pAchievement->mId);
		drawText(VuStringDB::IF()->getString(str).c_str(), mTitleRect + offset, textAnchor, mTitleFont, mTitleStringFormat);

		// description
		VU_SPRINTF(str, sizeof(str), "Ach%02d_Desc", pAchievement->mId);
		drawText(VuStringDB::IF()->getString(str).c_str(), mDescRect + offset, textAnchor, mDescFont, mDescStringFormat);

		// PC
		VU_SPRINTF(str, sizeof(str), "%d", pAchievement->mPC);
		mText = VuStringDB::IF()->getString(mPCStringID);
		VuStringUtil::replace(mText, "[[PREMIUM_CURRENCY]]", str);
		drawText(mText.c_str(), mPCRect + offset, textAnchor, mPCFont, mPCStringFormat);

		// image
		if ( progress < 1.0f )
			drawImage(mLockedImage, mImageRect + offset, imageAnchor, VuColor(255,255,255), GFX_SORT_DEPTH_STEP);
		else
			drawImage(mUnlockedImage, mImageRect + offset, imageAnchor, VuColor(255,255,255), GFX_SORT_DEPTH_STEP);

		// progress
		if ( progress > 0.0f && progress < 1.0f )
		{
			VU_SPRINTF(str, sizeof(str), "%d%%", VuRound(progress*100));
			drawText(str, mProgressRect + offset, imageAnchor, mProgressFont, mProgressStringFormat);
		}

		offset.mY += mSpacing;
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

		drawFilledRect(scrollIndicatorRect, mAnchor, mScrollBarColor, -2*GFX_SORT_DEPTH_STEP);
		drawFilledRect(scrollBarRect, mAnchor, mScrollBarBackColor, -GFX_SORT_DEPTH_STEP);
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
void VuAchievementsEntity::drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags)
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
void VuAchievementsEntity::drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	drawImage(image.getTextureAsset(), rect, anchor, color, depth);
}

//*****************************************************************************
void VuAchievementsEntity::drawImage(const VuTextureAsset *pTextureAsset, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	if ( pTextureAsset )
	{
		if ( VuTexture *pTexture = pTextureAsset->getTexture() )
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
}

//*****************************************************************************
void VuAchievementsEntity::drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	color.mA = VuRound(color.mA*mAlpha);

	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);

	VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth + depth, color, dstRect);
}

//*****************************************************************************
void VuAchievementsEntity::drawRect(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect dstRect = uiDrawParams.transform(rect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), dstRect);
}

//*****************************************************************************
float VuAchievementsEntity::calcScrollSize()
{
	return mAchievements.size()*mSpacing + mExtraScrollMax;
}

//*****************************************************************************
float VuAchievementsEntity::calcScrollMax()
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
VuUIAnchor VuAchievementsEntity::calcTextAnchor()
{
	VuUIAnchor scrollAnchor = mAnchor;

	if ( scrollAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM )
		scrollAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;

	return scrollAnchor;
}

//*****************************************************************************
VuUIAnchor VuAchievementsEntity::calcImageAnchor()
{
	VuUIAnchor imageAnchor = mAnchor;

	if ( imageAnchor.mAnchorH == VuUIAnchor::ANCHOR_LEFT_RIGHT )
		imageAnchor.mAnchorH = VuUIAnchor::ANCHOR_LEFT;

	if ( imageAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM )
		imageAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;

	return imageAnchor;
}
