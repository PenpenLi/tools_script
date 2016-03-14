//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuLeaderboardEntity
// 
//*****************************************************************************

#include "VuTableEntity.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


class VuLeaderboardEntity : public VuTableEntity, VuGameServicesManager::VuListener
{
	DECLARE_RTTI

public:
	VuLeaderboardEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// VuGameServicesManager::VuListener
	virtual void		onSignIn() { refresh(); }
	virtual void		onSignOut() { refresh(); }
	virtual void		onFriendsRetrieved() { refresh(); }

	// scripting
	VuRetVal			Refresh(const VuParams &params) { refresh(); return VuRetVal(); }

	void				refresh();

	virtual int			getRowCount();
	virtual const char	*getItemText(int row, int col);
	virtual bool		getRowHighlight(int row);

	virtual void		drawLayout(bool bSelected);
	virtual void		drawTable(float alpha);

	void				drawMessage(const std::string &stringID, float alpha);

	// properties
	int							mLeaderboardType;
	std::string					mSignedOutStringID;
	std::string					mPendingStringID;
	std::string					mFailureStringID;
	std::string					mNoScoresStringID;
	VuUIRectProperties			mMessageRect;
	std::string					mMessageFont;
	VuUIStringFormatProperties	mMessageStringFormat;

	enum { LBTYPE_EVENT, LBTYPE_WEEKLY };
	enum { COL_RANK, COL_NAME, COL_SCORE, NUM_COLUMNS };
	char				mText[128];

	std::string			mLeaderboardName;
	VUHANDLE			mLeaderboardQuery;
};

IMPLEMENT_RTTI(VuLeaderboardEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLeaderboardEntity);

//*****************************************************************************
VuLeaderboardEntity::VuLeaderboardEntity():
	mLeaderboardType(LBTYPE_EVENT),
	mLeaderboardQuery(VUNULL)
{
	// properties
	static VuStaticIntEnumProperty::Choice sLBTypeChoices[] =
	{
		{ "Event", LBTYPE_EVENT},
		{ "Weekly", LBTYPE_WEEKLY},
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Leaderboard Type", mLeaderboardType, sLBTypeChoices));
	addProperty(new VuStringProperty("SignedOut String ID", mSignedOutStringID));
	addProperty(new VuStringProperty("Pending String ID", mPendingStringID));
	addProperty(new VuStringProperty("Failure String ID", mFailureStringID));
	addProperty(new VuStringProperty("NoScores String ID", mNoScoresStringID));
	addProperty(new VuRectProperty("Message Rect", mMessageRect));
	addProperty(new VuFontEnumProperty("Message Font", mMessageFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mMessageStringFormat, "Message ");

	ADD_COLUMN("Rank", true);
	ADD_COLUMN("Name", true);
	ADD_COLUMN("Score", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuLeaderboardEntity, Refresh);
}

//*****************************************************************************
void VuLeaderboardEntity::onGameInitialize()
{
	if ( mLeaderboardType == LBTYPE_EVENT )
		mLeaderboardName = VuGameUtil::IF()->getEventName();
	else
		mLeaderboardName = "Weekly";

	mLeaderboardQuery = VuLeaderboardManager::IF()->createQuery(mLeaderboardName);

	VuGameServicesManager::IF()->addListener(this);
}

//*****************************************************************************
void VuLeaderboardEntity::onGameRelease()
{
	VuLeaderboardManager::IF()->releaseQuery(mLeaderboardQuery);

	VuGameServicesManager::IF()->removeListener(this);
}

//*****************************************************************************
void VuLeaderboardEntity::refresh()
{
	VUPRINTF("VuLeaderboardEntity::refresh()\n");
	VuLeaderboardManager::IF()->releaseQuery(mLeaderboardQuery);
	VuLeaderboardManager::IF()->clearCache(mLeaderboardName);
	mLeaderboardQuery = VuLeaderboardManager::IF()->createQuery(mLeaderboardName);
}

//*****************************************************************************
int VuLeaderboardEntity::getRowCount()
{
	return VuMin((int)VuLeaderboardManager::IF()->getScores(mLeaderboardQuery).size(), mMaxRows);
}

//*****************************************************************************
const char *VuLeaderboardEntity::getItemText(int row, int col)
{
	mText[0] = '\0';

	const VuLeaderboardManager::VuScore &score = VuLeaderboardManager::IF()->getScores(mLeaderboardQuery)[row];

	if ( col == COL_RANK )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", score.mRank);
	}
	else if ( col == COL_NAME )
	{
		VU_STRCPY(mText, sizeof(mText), score.mGamerTag.c_str());

		if ( mColumns[col]->mStringFormat.mClip )
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mColumns[col]->mFont);
			float fieldWidth = mColumns[col]->mRect.mWidth/VuUI::IF()->getAuthoringScreenScale().mX;

			if ( VuGfxUtil::IF()->fontDraw()->measureStringWidth(fontEntry.font(), mText, fontEntry.params(), VuUI::IF()->getAuthoringAspectRatio()) > fieldWidth )
			{
				for ( int numChars = (int)score.mGamerTag.length() - 1; numChars >= 0; numChars-- )
				{
					if ( score.mGamerTag[numChars] == ' ' )
						continue;

					VU_STRNCPY(mText, sizeof(mText), score.mGamerTag.c_str(), numChars);
					VU_STRCPY(mText + numChars, sizeof(mText) - numChars, "...");

					if ( VuGfxUtil::IF()->fontDraw()->measureStringWidth(fontEntry.font(), mText, fontEntry.params(), VuUI::IF()->getAuthoringAspectRatio()) <= fieldWidth )
						break;
				}
			}
		}
	}
	else if ( col == COL_SCORE )
	{
		if ( mLeaderboardType == LBTYPE_EVENT )
		{
			const std::string &gameType = VuGameUtil::IF()->getGameType();
			if ( gameType == "Race" || gameType == "BoostBlitz" )
			{
				float totalTime = (-score.mValue)*0.001f;
				VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, totalTime, mText, sizeof(mText));
			}
			else if ( gameType == "Elimination" )
			{
				VuStringUtil::integerFormat((int)score.mValue, mText, sizeof(mText));
				VU_STRCAT(mText, sizeof(mText), " m");
			}
			else if ( gameType == "ShootingGallery" || gameType == "DemolitionDerby" || gameType == "FollowLeader" )
			{
				VuStringUtil::integerFormat((int)score.mValue, mText, sizeof(mText));
			}
		}
		else if ( mLeaderboardType == LBTYPE_WEEKLY )
		{
			int distanceKm = (int)(score.mValue/1000.0);
			VuStringUtil::integerFormat(distanceKm, mText, sizeof(mText));
			VU_STRCAT(mText, sizeof(mText), " km");
		}
	}
	
	return mText;
}

//*****************************************************************************
bool VuLeaderboardEntity::getRowHighlight(int row)
{
	return VuLeaderboardManager::IF()->getScores(mLeaderboardQuery)[row].mGamerID == VuGameServicesManager::IF()->getGamerID();
}

//*****************************************************************************
void VuLeaderboardEntity::drawLayout(bool bSelected)
{
	VuTableEntity::drawLayout(bSelected);

	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect rect = uiDrawParams.transform(mMessageRect);

		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth - GFX_SORT_DEPTH_STEP, mHighlightColor, rect);
	}
}

//*****************************************************************************
void VuLeaderboardEntity::drawTable(float alpha)
{
	VuLeaderboardManager::eStatus status = VuLeaderboardManager::IF()->getStatus(mLeaderboardQuery);

	if ( !VuGameServicesManager::IF()->isSignedIn() )
	{
		drawMessage(mSignedOutStringID, alpha);
	}
	else if ( status == VuLeaderboardManager::RETRIEVE_PENDING )
	{
		drawMessage(mPendingStringID, alpha);
	}
	else if ( status == VuLeaderboardManager::RETRIEVE_FAILURE )
	{
		drawMessage(mFailureStringID, alpha);
	}
	else if ( status == VuLeaderboardManager::RETRIEVE_SUCCESS )
	{
		if ( getRowCount() == 0 )
			drawMessage(mNoScoresStringID, alpha);
		else
			VuTableEntity::drawTable(alpha);
	}
}

//*****************************************************************************
void VuLeaderboardEntity::drawMessage(const std::string &stringID, float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mMessageFont);

	VuRect rect = uiDrawParams.transform(mMessageRect);
	mAnchor.apply(rect, rect);

	VuFontDrawParams fdParams = fontEntry.params();
	fdParams.mSize *= 0.5f*(uiDrawParams.mLocalScale.mX + uiDrawParams.mLocalScale.mY);

	const char *text = VuStringDB::IF()->getString(stringID).c_str();
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), text, fdParams, rect, mMessageStringFormat, alpha);
}
