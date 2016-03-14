//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  
// 
//  by zouchao
//*****************************************************************************
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/UI/VuUIAnchor.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuPurple/Managers/VuRankManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"

class Vu15DayRewardListEntity : public VuEntity
{
	DECLARE_RTTI
protected:
	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	VuRetVal		Hide(const VuParams &params)		{ mVisible = false; return VuRetVal(); }
	VuRetVal		Show(const VuParams &params)		{ mVisible = true; return VuRetVal(); }
	// properties
	VuUIAnchor							mAnchor;
	VuUIRectProperties				mRect;
	VuUIRectProperties				mClipRect;
	float							mSpacingX;
	float							mSpacingY;

	VuUIRectProperties				mItemTextRect;
	VuUIRectProperties				mItemRect;
	VuUIRectProperties				mReceiveRect;

	std::string						mFont;

	VuUIImageProperties			mCanReceiveImage;

	//scroll view
	float										mScrollLag;
	float										mScrollResidualDamping;
	VuUIRectProperties				mScrollBarRect;
	VuColor								mScrollBarColor;
	VuColor								mScrollBarBackColor;
	float										mNextPrevScrollSpeed;

	float										mExtraScrollMax;


	struct RewardItem
	{
		RewardItem(){}
		int dayNo;
		std::string rewardName;
		int stats; // 奖励的状态，1 表示已经领取  2 表示可领取  3表示未领取（不能领取）
		VuTextureAsset *textureAsset;
	};
	typedef std::deque<RewardItem *> RewardItems;
	RewardItems							mRewardItems;

	enum eState { IDLE, SCROLLING };
	eState									mState;
	VuVector2							mInitialTouchDown;
	float										mScrollPos;
	float										mScrollTargetPos;
	float										mScrollVel;
	float										mScrollResidualVel;
	float										mScrollTouchPos;
	float										mAlpha;
	bool										mVisible;
protected:
	// scripting

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawRankLists(float alpha);

	void				drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags);
	void				drawMyInfoText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags);
	void				drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawImage(const VuTextureAsset *pTextureAsset, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth);
	void				drawRect(const VuRect &rect);

	float				calcScrollSize();
	float				calcScrollMax();


	VuUIAnchor			calcTextAnchor();
	VuUIAnchor			calcImageAnchor();
public:
	Vu15DayRewardListEntity();
	~Vu15DayRewardListEntity();
};

IMPLEMENT_RTTI(Vu15DayRewardListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(Vu15DayRewardListEntity);

Vu15DayRewardListEntity::Vu15DayRewardListEntity() :
mSpacingX(0),
mSpacingY(0),
mScrollLag(0.1f),
mScrollResidualDamping(0.99f),
mScrollBarRect(0, 0, 0, 0),
mScrollBarColor(255, 255, 255),
mScrollBarBackColor(0, 0, 0),
mNextPrevScrollSpeed(360.0f),
mExtraScrollMax(0.0f),
mState(IDLE),
mScrollPos(0),
mScrollTargetPos(0),
mScrollVel(0),
mScrollResidualVel(0),
mAlpha(1.0f),
mVisible(true)
{
	//components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(new Vu2dLayoutComponent(this, &Vu15DayRewardListEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, Vu15DayRewardListEntity, Hide);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, Vu15DayRewardListEntity, Show);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ShowButtons);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, HideButtons);

	//properties
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuRectProperty("ItemRect", mItemRect));
	addProperty(new VuRectProperty("ItemTextRect", mItemTextRect));
	addProperty(new VuRectProperty("ReceiveRect", mReceiveRect));
	addProperty(new VuRectProperty("Clip Rect", mClipRect));
	addProperty(new VuFloatProperty("SpacingX", mSpacingX));
	addProperty(new VuFloatProperty("SpacingY", mSpacingY));
	addProperty(new VuFontEnumProperty("Font", mFont));

	addProperty(new VuFloatProperty("ScrollLag", mScrollLag));
	addProperty(new VuFloatProperty("ScrollResidualDamping", mScrollResidualDamping));
	addProperty(new VuRectProperty("ScrollBarRect", mScrollBarRect));
	addProperty(new VuColorProperty("ScrollBarColor", mScrollBarColor));
	addProperty(new VuColorProperty("ScrollBarBackColor", mScrollBarBackColor));

	addProperty(new VuFloatProperty("NextPrevScrollSpeed", mNextPrevScrollSpeed));

	addProperty(new VuFloatProperty("ExtraScrollMax", mExtraScrollMax));
	mCanReceiveImage.addProperties(getProperties(), "CanReceiveImage");

	REG_EVENT_HANDLER(Vu15DayRewardListEntity, OnUITick);
	REG_EVENT_HANDLER(Vu15DayRewardListEntity, OnUITouch);
	REG_EVENT_HANDLER(Vu15DayRewardListEntity, OnUIDraw);

	if (mRewardItems.empty())
	{
		for (int i = 1; i <= 15; i++)
		{
			RewardItem *item = new RewardItem();
			item->dayNo = i;
			item->stats = 3;
			char str[128];
			VU_SPRINTF(str, sizeof(str), "UI/Buttons/%dDayLogin",i);
			item->rewardName = str;
			
			VuTextureAsset *pTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>(str, VuAssetFactory::OPTIONAL_ASSET);
			item->textureAsset = pTexture;

			if (VuGameManager::IF())
				item->stats = VuGameManager::IF()->getCurrDayState(i);
			else
			{
				item->stats = 3;
				if (i == 1)
				{
					item->stats = 1;
				}
				if (i == 3)
				{
					item->stats = 2;
				}
			}
				

			mRewardItems.push_back(item);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
Vu15DayRewardListEntity::~Vu15DayRewardListEntity()
{
	for (const RewardItem * pRewardItem : mRewardItems)
	{
		VuAssetFactory::IF()->releaseAsset(pRewardItem->textureAsset);
		delete pRewardItem;
		pRewardItem = NULL;
	}
	mRewardItems.clear();

	/*if (mCanReceiveImage.getTexture())
	{
		VuAssetFactory::IF()->releaseAsset(mCanReceiveImage.getTextureAsset());
	}*/
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	int padIndex = 0;
	if (VuInputManager::IF()->getButtonValue(padIndex, "Up"))
		mScrollTargetPos -= mNextPrevScrollSpeed*fdt;
	if (VuInputManager::IF()->getButtonValue(padIndex, "Down"))
		mScrollTargetPos += mNextPrevScrollSpeed*fdt;

	if (mState != SCROLLING)
	{
		mScrollResidualVel *= VuPow(1.0f - mScrollResidualDamping, fdt);
		mScrollTargetPos += mScrollResidualVel* fdt;
		mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
	}
	mScrollPos = VuMathUtil::smoothCD(mScrollPos, mScrollTargetPos, mScrollVel, mScrollLag, fdt);
}


//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();

	if (action == VuUIInputUtil::TOUCH_DOWN)
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);
		VuRect screenRect = uiDrawParams.transform(mRect);
		mAnchor.apply(screenRect, screenRect);

		if (screenRect.contains(touch))
		{
			mState = SCROLLING;
			mInitialTouchDown = touch;
			mScrollTouchPos = mScrollTargetPos;
		}
	}
	else if (action == VuUIInputUtil::TOUCH_UP)
	{
		mState = IDLE;
		mScrollResidualVel = mScrollVel;
	}
	else if (action == VuUIInputUtil::TOUCH_MOVE)
	{
		if (mState == SCROLLING)
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuVector2 touchDelta = uiDrawParams.transformInv(touch) - uiDrawParams.transformInv(mInitialTouchDown);

			mScrollTargetPos = mScrollTouchPos - touchDelta.mY;
			if (mScrollTargetPos > 0)
			{
				int i = 0;
				int j = i;
			}
			mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::OnUIDraw(const VuParams &params)
{
	drawRankLists(mpTransitionComponent->alpha());
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawLayout(bool bSelected)
{
	if (mRewardItems.empty())
	{
		for (int i = 1; i <= 15; i++)
		{
			RewardItem *item = new RewardItem();
			item->dayNo = i;
			item->stats = 3;
			char str[128];
			VU_SPRINTF(str, sizeof(str), "UI/Buttons/%dDayLogin",i);
			item->rewardName = str;

			VuTextureAsset *pTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>(str, VuAssetFactory::OPTIONAL_ASSET);
			item->textureAsset = pTexture;

			if (i == 1)
			{
				item->stats = 1;
			}
			if (i == 3)
			{
				item->stats = 2;
			}
			mRewardItems.push_back(item);
		}
	}

	if (bSelected)
	{
		drawRect(mRect);
		drawRect(mClipRect);
	}

	drawRankLists(1.0f);
	drawRect(mScrollBarRect);
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawRankLists(float alpha)
{
	if (mVisible == false)
		return;

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mAlpha = alpha;
	VuUIAnchor textAnchor = calcTextAnchor();
	VuUIAnchor imageAnchor = calcImageAnchor();

	VuVector2 offset(0.0f, -mScrollPos);

	if (false)
	{
		;
	}
	else
	{
		int i = 0;
		for (const RewardItem *tempItem : mRewardItems)
		{
			int ix = i % 3;
			int iy = i / 3;
			i++;

			VuVector2 tmpOffset;
			tmpOffset.mX = mSpacingX * ix;
			tmpOffset.mY = offset.mY + mSpacingY * iy;
			
			char str[128];

			VuTexture *pTexture = tempItem->textureAsset->getTexture();

			if (pTexture)
			{
				VuColor color(255, 255, 255);
				color.mA = VuRound(color.mA*mAlpha);
				if (color.mA)
				{
					VuUIDrawParams uiDrawParams;
					VuUIDrawUtil::getParams(this, uiDrawParams);

					VuRect clipRect = uiDrawParams.transform(mClipRect);
					mAnchor.apply(clipRect, clipRect);

					VuRect dstRect = uiDrawParams.transform(mItemRect + tmpOffset);
					mAnchor.apply(dstRect, dstRect);

					VuRect dstTextRect = uiDrawParams.transform(mItemTextRect + tmpOffset);
					mAnchor.apply(dstTextRect, dstTextRect);

					VuRect receiveRect = uiDrawParams.transform(mReceiveRect + tmpOffset);
					mAnchor.apply(mReceiveRect, mReceiveRect);



					if (dstRect.intersects(clipRect))
					{
						VuRect srcRect(0, 0, 1, 1);
						VuRect dstRectOrig = dstRect;
						dstRect = VuRect::intersection(clipRect, dstRect);
						srcRect.mWidth = dstRect.mWidth / dstRectOrig.mWidth;
						srcRect.mHeight = dstRect.mHeight / dstRectOrig.mHeight;

						if (dstRectOrig.getLeft() < clipRect.getLeft())
							srcRect.mX = 1.0f - srcRect.mWidth;
						if (dstRectOrig.getTop() < clipRect.getTop())
							srcRect.mY = 1.0f - srcRect.mHeight;



						//VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);

						if (tempItem->stats == 1)
						{
							VuGameGfxUtil::IF()->drawGrayScaleTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
							std::string bodyText = VuStringDB::IF()->getString("Received_Login_Text");
							const char *stringId = bodyText.c_str();

							if (stringId[0])
							{
								const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
								const char *strText = VuStringDB::IF()->getStringSelf(stringId);

								VuFontDrawParams fdParams = fontEntry.params();

								VuRect dRect = dstTextRect;
								//fdParams.mSize *= 0.5f*(mpTransformComponent->getWorldScale().mX + mpTransformComponent->getWorldScale().mY);

								/*VuUIDrawParams uiDrawParams;
								VuUIDrawUtil::getParams(this, uiDrawParams);
								dRect = uiDrawParams.transform(dRect);
								mAnchor.apply(dRect, dstRect);

								VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER;

								VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth + 0.7 * GFX_SORT_DEPTH_STEP, fontEntry.font(),
								strText, fdParams, (mItemTextRect + tmpOffset)*uiDrawParams.mInvAuthScale, 1.0f);*/
								/*dRect = uiDrawParams.transform(dRect);
								mAnchor.apply(dRect, dRect);*/

								VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER;
								VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth + 0.7 * GFX_SORT_DEPTH_STEP, fontEntry.font(), strText, fdParams, dRect, flags);
							}
						}
						else if(tempItem->stats == 2)
						{
							VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
							VuTexture *pTmpTexture = mCanReceiveImage.getTexture();
							VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTmpTexture, color, receiveRect, srcRect);
						}
						else
						{
							VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
						}
					}
				}
			}
			
		}
	}
}

void Vu15DayRewardListEntity::drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags)
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

//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	drawImage(image.getTextureAsset(), rect, anchor, color, depth);
}

//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawImage(const VuTextureAsset *pTextureAsset, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	if (pTextureAsset)
	{
		if (VuTexture *pTexture = pTextureAsset->getTexture())
		{
			color.mA = VuRound(color.mA*mAlpha);
			if (color.mA)
			{
				VuUIDrawParams uiDrawParams;
				VuUIDrawUtil::getParams(this, uiDrawParams);

				VuRect clipRect = uiDrawParams.transform(mClipRect);
				mAnchor.apply(clipRect, clipRect);

				VuRect dstRect = uiDrawParams.transform(rect);
				anchor.apply(dstRect, dstRect);

				VuRect srcRect(0, 0, 1, 1);
				if (dstRect.intersects(clipRect))
				{
					VuRect dstRectOrig = dstRect;
					dstRect = VuRect::intersection(clipRect, dstRect);
					srcRect.mWidth = dstRect.mWidth / dstRectOrig.mWidth;
					srcRect.mHeight = dstRect.mHeight / dstRectOrig.mHeight;

					if (dstRectOrig.getLeft() < clipRect.getLeft())
						srcRect.mX = 1.0f - srcRect.mWidth;
					if (dstRectOrig.getTop() < clipRect.getTop())
						srcRect.mY = 1.0f - srcRect.mHeight;

					VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + depth, pTexture, color, dstRect, srcRect);
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	color.mA = VuRound(color.mA*mAlpha);
	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);
	VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth + depth, color, dstRect);
}
//////////////////////////////////////////////////////////////////////////
void Vu15DayRewardListEntity::drawRect(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	VuRect dstRect = uiDrawParams.transform(rect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255, 255, 255), dstRect);
}
//////////////////////////////////////////////////////////////////////////
float Vu15DayRewardListEntity::calcScrollSize()
{
	if (mRewardItems.size() > 0)
		return mRewardItems.size()*mSpacingX /3  + mExtraScrollMax;
	else
		return 0;
}
//////////////////////////////////////////////////////////////////////////
float Vu15DayRewardListEntity::calcScrollMax()
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
//////////////////////////////////////////////////////////////////////////
VuUIAnchor Vu15DayRewardListEntity::calcTextAnchor()
{
	VuUIAnchor scrollAnchor = mAnchor;
	if (scrollAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM)
		scrollAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;
	return scrollAnchor;
}
//////////////////////////////////////////////////////////////////////////
VuUIAnchor Vu15DayRewardListEntity::calcImageAnchor()
{
	VuUIAnchor imageAnchor = mAnchor;
	if (imageAnchor.mAnchorH == VuUIAnchor::ANCHOR_LEFT_RIGHT)
		imageAnchor.mAnchorH = VuUIAnchor::ANCHOR_LEFT;
	if (imageAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM)
		imageAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;
	return imageAnchor;
}

