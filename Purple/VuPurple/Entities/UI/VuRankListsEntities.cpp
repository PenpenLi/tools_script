//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAchievementsEntity
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

class VuRankListsEntity : public VuEntity
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
	float										mSpacing;
	VuUIRectProperties				mItemRect;

	//rank
	VuUIRectProperties				mRankRect;
	std::string								mRankFont;
	VuUIStringFormatProperties mRankStringFormat;
	//milestone
	VuUIRectProperties				mMilestoneRect;
	std::string								mMilestoneFont;
	VuUIStringFormatProperties mMilestoneStringFormat;
	//player name
	VuUIRectProperties				mPlayerNameRect;
	std::string								mPlayerNameFont;
	VuUIStringFormatProperties mPlayerNameStringFormat;
	//my info name
	VuUIRectProperties				mMyInfoNameRect;
	std::string								mMyInfoNameFont;
	VuUIStringFormatProperties mMyInfoNameStringFormat;
	//my info milestone
	VuUIRectProperties				mMyInfoMilestoneRect;
	std::string								mMyInfoMilestoneFont;
	VuUIStringFormatProperties mMyInfoMilestoneStringFormat;
	//my info rank
	VuUIRectProperties				mMyInfoRankRect;
	std::string								mMyInfoRankFont;
	VuUIStringFormatProperties mMyInfoRankStringFormat;

	//scroll view
	float										mScrollLag;
	float										mScrollResidualDamping;
	VuUIRectProperties				mScrollBarRect;
	VuColor								mScrollBarColor;
	VuColor								mScrollBarBackColor;
	float										mNextPrevScrollSpeed;
	VuUIImageProperties			mBackgroundTexture;
	float										mBackgroundSpeed;
	float										mExtraScrollMax;
	VuUIImageProperties			mItemBackgroundTexture;
	//myself infomation
	VuUIImageProperties			mMyInfoBackgroundTexture;
	VuUIRectProperties				mMyinfoRect;

	struct RankItem
	{
		RankItem(int rank, int milestone, std::string name) :m_rank(rank),m_milestone(milestone),m_name(name){}
		int m_rank;
		int m_milestone;
		std::string m_name;
	};
	typedef std::deque<RankItem *> RankItems;
	RankItems							mRankItems;

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
	VuRetVal			TabDataShow(const VuParams &params);
	VuRetVal			RequestPlayerName(const VuParams &params);
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

	void				updateData();

	VuUIAnchor			calcTextAnchor();
	VuUIAnchor			calcImageAnchor();
public:
	VuRankListsEntity();
	~VuRankListsEntity();
};

IMPLEMENT_RTTI(VuRankListsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRankListsEntity);

VuRankListsEntity::VuRankListsEntity() :
mSpacing(0),
mScrollLag(0.1f),
mScrollResidualDamping(0.99f),
mScrollBarRect(0, 0, 0, 0),
mScrollBarColor(255, 255, 255),
mScrollBarBackColor(0, 0, 0),
mNextPrevScrollSpeed(360.0f),
mBackgroundSpeed(1.0f),
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
	addComponent(new Vu2dLayoutComponent(this, &VuRankListsEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRankListsEntity, TabDataShow);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRankListsEntity, RequestPlayerName);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRankListsEntity, Hide);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRankListsEntity, Show);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ShowButtons);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, HideButtons);

	//properties
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuRectProperty("MyInfoRect",mMyinfoRect));
	addProperty(new VuRectProperty("ItemRect", mItemRect));
	addProperty(new VuRectProperty("Clip Rect", mClipRect));
	addProperty(new VuFloatProperty("Spacing", mSpacing));

	//rank
	addProperty(new VuRectProperty("RankRect", mRankRect));
	addProperty(new VuFontEnumProperty("RankFont", mRankFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mRankStringFormat, "Rank");

	addProperty(new VuRectProperty("MyInfoRankRect",mMyInfoRankRect));
	addProperty(new VuFontEnumProperty("MyInfoRankFont", mMyInfoRankFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(),mMyInfoRankStringFormat,"MyInfoRank");
	
	//milestone
	addProperty(new VuRectProperty("MilestoneRect", mMilestoneRect));
	addProperty(new VuFontEnumProperty("MilestoneFont", mMilestoneFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mMilestoneStringFormat, "Milestone");
	addProperty(new VuRectProperty("MyInfoMilestoneRect", mMyInfoMilestoneRect));
	addProperty(new VuFontEnumProperty("MyInfoMilestoneFont", mMyInfoMilestoneFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mMyInfoMilestoneStringFormat, "MyInfoMilestone");
	
	//name
	addProperty(new VuRectProperty("PlayerNameRect", mPlayerNameRect));
	addProperty(new VuFontEnumProperty("PlayNameFont", mPlayerNameFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mPlayerNameStringFormat, "PlayerName");
	addProperty(new VuRectProperty("MyInfoPlayerNameRect", mMyInfoNameRect));
	addProperty(new VuFontEnumProperty("MyInfoPlayNameFont", mMyInfoNameFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mMyInfoNameStringFormat, "MyInfoPlayerName");

	addProperty(new VuFloatProperty("ScrollLag", mScrollLag));
	addProperty(new VuFloatProperty("ScrollResidualDamping", mScrollResidualDamping));
	addProperty(new VuRectProperty("ScrollBarRect", mScrollBarRect));
	addProperty(new VuColorProperty("ScrollBarColor", mScrollBarColor));
	addProperty(new VuColorProperty("ScrollBarBackColor", mScrollBarBackColor));

	addProperty(new VuFloatProperty("NextPrevScrollSpeed", mNextPrevScrollSpeed));

	mBackgroundTexture.addProperties(getProperties(), "BackgroundTexture");
	mItemBackgroundTexture.addProperties(getProperties(), "ItemBackgroundTexture");
	mMyInfoBackgroundTexture.addProperties(getProperties(), "MyInoBackgroundTexture");
	addProperty(new VuFloatProperty("BackgroundSpeed", mBackgroundSpeed));
	addProperty(new VuFloatProperty("ExtraScrollMax", mExtraScrollMax));

	REG_EVENT_HANDLER(VuRankListsEntity, OnUITick);
	REG_EVENT_HANDLER(VuRankListsEntity, OnUITouch);
	REG_EVENT_HANDLER(VuRankListsEntity, OnUIDraw);

	if (mRankItems.empty())
	{
		for (int i = 0; i < 10; i++)
		{
			mRankItems.push_back(new RankItem(1, 100, "test"));
		}
	}
	/*if (VuRankManager::IF()->getRankDatas())
	{
	for (int i = 0; i < VuRankManager::IF()->getRankDatas()->mRankDatas.size(); i++)
	{
	int idx = VuRankManager::IF()->getRankDatas()->mRankDatas[i].mRank;
	int  num = VuRankManager::IF()->getRankDatas()->mRankDatas[i].mValue;
	std::string name = VuRankManager::IF()->getRankDatas()->mRankDatas[i].mGamePlayer;
	mRankItems.push_back(new RankItem(idx, num, name));
	}
	}*/
	

	if (VuGameFontMacros::IF())
	{
		VuGameFontMacros::IF()->setMacro("TEMP_USER_NAME", VuGameManager::IF()->getTempPlayerName().c_str());
		char str[128];
		VU_SPRINTF(str, sizeof(str), "(%d/%d)", VuGameManager::IF()->getNameTimes(), 3);
		VuGameFontMacros::IF()->setMacro("GET_NAME_TIMES", str);
	}
	if (VuRankManager::IF())
	{
		VuRankManager::IF()->setRankDataType(VuRankManager::CUR_WEEK);
		VuRankManager::IF()->startGetRank();
	}
}
//////////////////////////////////////////////////////////////////////////
VuRankListsEntity::~VuRankListsEntity()
{
	for (const RankItem * prankItem : mRankItems)
	{
		delete prankItem;
		prankItem = NULL;
	}
	mRankItems.clear();
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::OnUITick(const VuParams &params)
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
VuRetVal VuRankListsEntity::TabDataShow(const VuParams &params)
{
	//update data
	updateData();
	return VuRetVal();
}
//////////////////////////////////////////////////////////////////////////
VuRetVal VuRankListsEntity::RequestPlayerName(const VuParams &params)
{
	//update data
	VuRankManager::IF()->startGetPlayerName();
	return VuRetVal();
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::OnUITouch(const VuParams &params)
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
void VuRankListsEntity::OnUIDraw(const VuParams &params)
{ 
	drawRankLists(mpTransitionComponent->alpha());
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::drawLayout(bool bSelected)
{
	if (mRankItems.empty())
	{
		for (int i = 0; i < 10; i++)
		{
			mRankItems.push_back(new RankItem(1, 100, "test"));
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
void VuRankListsEntity::drawRankLists(float alpha)
{
	if (mVisible == false)
		return;

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mAlpha = alpha;
	VuUIAnchor textAnchor = calcTextAnchor();
	VuUIAnchor imageAnchor = calcImageAnchor();

	VuVector2 offset(0.0f, -mScrollPos);

	if (VuRankManager::IF() && VuRankManager::IF()->getRankDatas())
	{
		//draw other players
		for (int i = 0; i < VuRankManager::IF()->getRankDatas()->mRankDatas.size(); i++)
		{
			int idx = VuRankManager::IF()->getRankDatas()->mRankDatas[i].mRank;
			float  num = (float)VuRankManager::IF()->getRankDatas()->mRankDatas[i].mValue / 1000.0;
			std::string name = VuRankManager::IF()->getRankDatas()->mRankDatas[i].mGamePlayer;
			
			char str[128];
			VU_SPRINTF(str, sizeof(str), "%d", idx);
			drawText(str, mRankRect + offset, textAnchor, mRankFont, mRankStringFormat);

			
			VU_SPRINTF(str, sizeof(str), "%.1f %s", num, VuStringDB::IF()->getString("Kilometre").c_str());
			drawText(str, mMilestoneRect + offset, textAnchor, mMilestoneFont, mMilestoneStringFormat);

			drawText(name.c_str(), mPlayerNameRect + offset, textAnchor, mPlayerNameFont, mPlayerNameStringFormat);

			VuTexture *pTexture = mItemBackgroundTexture.getTexture();

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

					VuRect dstRect = uiDrawParams.transform(mItemRect + offset);
					mAnchor.apply(dstRect, dstRect);

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

						VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.5 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
					}
				}
			}
			offset.mY += mSpacing;
		}
	}
	else
	{
		for ( const RankItem *tempItem : mRankItems)
		{
			char str[128];
			VU_SPRINTF(str, sizeof(str), "%d", tempItem->m_rank);
			drawText(str, mRankRect + offset, textAnchor, mRankFont, mRankStringFormat);

			VU_SPRINTF(str, sizeof(str), "%d", tempItem->m_milestone);
			drawText(str, mMilestoneRect + offset, textAnchor, mMilestoneFont, mMilestoneStringFormat);

			drawText(tempItem->m_name.c_str(), mPlayerNameRect + offset, textAnchor, mPlayerNameFont, mPlayerNameStringFormat);

			VuTexture *pTexture = mItemBackgroundTexture.getTexture();

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

					VuRect dstRect = uiDrawParams.transform(mItemRect + offset);
					mAnchor.apply(dstRect, dstRect);

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

						VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.6 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
					}
				}
			}
			offset.mY += mSpacing;
		}
	}

	if (VuGameManager::IF() && VuGameManager::IF()->IsHavePlayerName())
	{
		mpScriptComponent->getPlug("HideButtons")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("ShowButtons")->execute();
	}

	//draw my info
	if (VuGameManager::IF()&&VuGameManager::IF()->IsHavePlayerName())
	{
		char strMyInfo[128];
		int rank = VuRankManager::IF()->getCurRank();
		float milestone = (float)VuRankManager::IF()->getCurMilestone() / 1000.0;
		char str[128];
		if (rank > 0 && rank < 99999)
			VU_SPRINTF(str, sizeof(str), "%d", rank);
		else
			VU_SPRINTF(str, sizeof(str), "%s", VuStringDB::IF()->getString("NoRank").c_str());
		drawMyInfoText(str, mMyInfoRankRect, textAnchor, mMyInfoRankFont, mMyInfoRankStringFormat);
		if (milestone > 0)
			VU_SPRINTF(str, sizeof(str), "%.1f %s", milestone, VuStringDB::IF()->getString("Kilometre").c_str());
		else
			VU_SPRINTF(str, sizeof(str), "%s",  VuStringDB::IF()->getString("NO_RANK_DATA").c_str()); 
		drawMyInfoText(str, mMyInfoMilestoneRect, textAnchor, mMyInfoMilestoneFont, mMyInfoMilestoneStringFormat);
		drawMyInfoText(VuGameManager::IF()->getPlayerName().c_str(), mMyInfoNameRect, textAnchor, mMyInfoNameFont, mMyInfoNameStringFormat);
	}
	VuTexture* myInfoBackgroundTex = mMyInfoBackgroundTexture.getTexture();
	if (myInfoBackgroundTex)
	{
		VuColor color(205, 155, 29);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mMyinfoRect);
		mAnchor.apply(dstRect, dstRect);
		VuRect srcRect(0, 0, 1, 1);
		//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.5 * GFX_SORT_DEPTH_STEP, myInfoBackgroundTex, color, dstRect, srcRect);
	}


	if (VuTexture *pTexture = mBackgroundTexture.getTexture())
	{
		VuColor color(255, 255, 255);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mRect);
		mAnchor.apply(dstRect, dstRect);
		VuRect srcRect(0, 0, 1, 1);
		//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	}
}
//////////////////////////////////////////////////////////////////////////
void	VuRankListsEntity::drawMyInfoText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(font);

	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);

	VuFontDrawParams params = fontEntry.params();
	params.mClip = true;
	params.mClipRect = uiDrawParams.transform(mMyinfoRect);
	mAnchor.apply(params.mClipRect, params.mClipRect);

	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), text, params, dstRect, flags, mAlpha);
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::drawText(const char *text, const VuRect &rect, const VuUIAnchor &anchor, const std::string &font, int flags)
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
void VuRankListsEntity::drawImage(const VuUIImageProperties &image, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	drawImage(image.getTextureAsset(), rect, anchor, color, depth);
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::drawImage(const VuTextureAsset *pTextureAsset, const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
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
void VuRankListsEntity::drawFilledRect(const VuRect &rect, const VuUIAnchor &anchor, VuColor color, float depth)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	color.mA = VuRound(color.mA*mAlpha);
	VuRect dstRect = uiDrawParams.transform(rect);
	anchor.apply(dstRect, dstRect);
	VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth + depth, color, dstRect);
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::drawRect(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	VuRect dstRect = uiDrawParams.transform(rect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255, 255, 255), dstRect);
}
//////////////////////////////////////////////////////////////////////////
float VuRankListsEntity::calcScrollSize()
{
	if (VuRankManager::IF()->getRankDatas())
		return VuRankManager::IF()->getRankDatas()->mRankDatas.size()*mSpacing + mExtraScrollMax;
	else
		return 0;
}
//////////////////////////////////////////////////////////////////////////
float VuRankListsEntity::calcScrollMax()
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
VuUIAnchor VuRankListsEntity::calcTextAnchor()
{
	VuUIAnchor scrollAnchor = mAnchor;
	if (scrollAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM)
		scrollAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;
	return scrollAnchor;
}
//////////////////////////////////////////////////////////////////////////
VuUIAnchor VuRankListsEntity::calcImageAnchor()
{
	VuUIAnchor imageAnchor = mAnchor;
	if (imageAnchor.mAnchorH == VuUIAnchor::ANCHOR_LEFT_RIGHT)
		imageAnchor.mAnchorH = VuUIAnchor::ANCHOR_LEFT;
	if (imageAnchor.mAnchorV == VuUIAnchor::ANCHOR_TOP_BOTTOM)
		imageAnchor.mAnchorV = VuUIAnchor::ANCHOR_TOP;
	return imageAnchor;
}
//////////////////////////////////////////////////////////////////////////
void VuRankListsEntity::updateData()
{
	VuRankManager::IF()->tabRankDataType();
}
