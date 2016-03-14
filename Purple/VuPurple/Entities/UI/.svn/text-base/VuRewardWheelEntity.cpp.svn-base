//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuRewardWheelEntity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Util/VuStringUtil.h"

#include "VuPurple/Entities/UI/VuRewardWheelEntity.h"

// constants
#define TARGET_ANGLE_REACHED_THRESHOLD 0.0125f // multiplied by wedge angle


IMPLEMENT_RTTI(VuRewardWheelEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRewardWheelEntity);

//*****************************************************************************
VuRewardWheelEntity::VuRewardWheelEntity() :
	mAngularOffset(0),
	mInnerRadius(0),
	mOuterRadius(0),
	mBackgroundRadius(0.0f),
	mWedgeAngle(15.0f),
	mSpaceAngle(0.0f),
	mSubdivisionAngle(1.0f),
	mRotationLag(4.0f),
	mMaxAngSpeed(0.0f),
	mTexCoord0(0, 0),
	mTexCoord1(1, 1),
	mSpinRevs(3),
	mTargetAngle(0.0f),
	mCurAngle(0.0f),
	mAngularVel(0.0f),
	mCurWedge(-1),
	mState(WAITING),
	mNeedleSize(0.0f, 0.0f),
	mNeedleRadius(0.0f),
	mNeedleRotation(0.0f),
	mNeedleAngularVelocity(0.0f),
	mNeedlePivotOffset(0.0f, 0.0f),
	mPegSize(0.0f, 0.0f),
	mPegAdjust(0.0f, 0.0f),
	mWheelTexCoord0(0.0f, 0.0f),
	mWheelTexCoord1(1.0f, 1.0f)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuRewardWheelEntity::drawLayout));

	// properties
	addProperty(new VuFloatProperty("Angular Offset", mAngularOffset));
	addProperty(new VuFloatProperty("Inner Radius", mInnerRadius));
	addProperty(new VuFloatProperty("Outer Radius", mOuterRadius));
	addProperty(new VuFloatProperty("Wheel Image Radius", mBackgroundRadius));
	addProperty(new VuFloatProperty("Needle Radius", mNeedleRadius));
	addProperty(new VuFloatProperty("Wedge Angle", mWedgeAngle));
	addProperty(new VuFloatProperty("Reward Icon Padding Angle", mSpaceAngle));
	addProperty(new VuFloatProperty("Subdivision Angle", mSubdivisionAngle));
	addProperty(new VuFloatProperty("Rotation Lag", mRotationLag));
	addProperty(new VuFloatProperty("Max Angular Speed", mMaxAngSpeed));
	
	mNeedleTexture.addProperties(getProperties(), "Needle Texture");
	addProperty(new VuVector2Property("Needle Pivot Offset", mNeedlePivotOffset));
	addProperty(new VuVector2Property("Needle Size", mNeedleSize));

	mPegTexture.addProperties(getProperties(), "Peg Texture");
	addProperty(new VuVector2Property("Peg Adjust", mPegAdjust));
	addProperty(new VuVector2Property("Peg Size", mPegSize));

	mWheelSlice1.addProperties(getProperties(), "Wheel Slice 1 Texture");
	mWheelSlice2.addProperties(getProperties(), "Wheel Slice 2 Texture");
	addProperty(new VuVector2Property("Wheel Texture Coord 0", mWheelTexCoord0));
	addProperty(new VuVector2Property("Wheel Texture Coord 1", mWheelTexCoord1));

	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	addProperty(new VuVector2Property("Tex Coord 0", mTexCoord0));
	addProperty(new VuVector2Property("Tex Coord 1", mTexCoord1));
	addProperty(new VuIntProperty("Spin Revs", mSpinRevs));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRewardWheelEntity, Spin);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuRewardWheelEntity, IsCompleted, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, SpinFinished);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnPegTick);

	// event handlers
	REG_EVENT_HANDLER(VuRewardWheelEntity, OnUITick);
	REG_EVENT_HANDLER(VuRewardWheelEntity, OnUIDraw);
}

//*****************************************************************************
void VuRewardWheelEntity::onGameInitialize()
{
	mDaysSince2000 = VuTimeUtil::calcDaysSince2000();

	mCurAngle = 0.0f;
	mAngularVel = 0.0f;

	// today's car / power-up
	std::string todaysCar, todaysPowerUp;
	chooseTodaysCar(todaysCar);
	chooseTodaysPowerUp(todaysPowerUp);

	VuSpreadsheetAsset *pSA = VuAssetFactory::IF()->createAsset<VuSpreadsheetAsset>("DailyRewards");

	int stage = VuGameManager::IF()->getUnlockedStage();

	int chanceColumn = pSA->getColumnIndex("Chance");
	int typeColumn = pSA->getColumnIndex("Type");
	int imageColumn = pSA->getColumnIndex("Image");
	int paramsColumn = pSA->getColumnIndex("Params");

	VuSpreadsheetQuery::VuIntEqual expression("Stage", stage);
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, expression);
	while ( rowIndex < pSA->getRowCount() )
	{
		std::string type = pSA->getField(rowIndex, typeColumn).asCString();
		std::string image = pSA->getField(rowIndex, imageColumn).asCString();

		bool addReward = true;
		if ( type == "Car" && todaysCar.empty() )
			addReward = false;
		if ( type == "PowerUp" && todaysPowerUp.empty() )
			addReward = false;
		if ( type == "Tokens" && VuBillingManager::IF()->isUnlocked("DisableAds") )
			addReward = false;

		if ( addReward )
		{
			mRewards.resize(mRewards.size() + 1);
			Reward &reward = mRewards.back();

			reward.mType = type;
			reward.mChance = pSA->getField(rowIndex, chanceColumn).asInt();

			if ( type == "SC" || type == "PC" )
			{
				reward.mAmount = pSA->getField(rowIndex, paramsColumn).asInt();
				reward.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(image, VuAssetFactory::OPTIONAL_ASSET);
			}
			else if ( type == "Car" )
			{
				reward.mName = todaysCar;
				reward.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Rewards/Car_" + reward.mName, VuAssetFactory::OPTIONAL_ASSET);
			}
			else if ( type == "PowerUp" )
			{
				reward.mName = todaysPowerUp;
				reward.mpImage = VuPowerUpManager::IF()->getPowerUpByName(reward.mName.c_str())->mpIconTextureAsset;
				VuAssetFactory::IF()->addAssetRef(reward.mpImage);
			}
			else if ( type == "Upgrade" )
			{
				reward.mName = pSA->getField(rowIndex, paramsColumn).asCString();
				reward.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Rewards/Upgrade_" + reward.mName, VuAssetFactory::OPTIONAL_ASSET);

				if ( VuGameManager::IF()->getCurCar().getStat(reward.mName.c_str()) >= VuGameManager::IF()->getCurCar().getMaxLevel() )
					reward.mChance = 0;
			}
			else if ( type == "Tokens" )
			{
				reward.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(image, VuAssetFactory::OPTIONAL_ASSET);

				if ( VuGameManager::IF()->getTokens() >= VuCloudTuningManager::IF()->variables().mMaxTokens )
					reward.mChance = 0;
			}
		}

		rowIndex = VuSpreadsheetQuery::findNextRow(pSA, expression, rowIndex);
	}

	VuAssetFactory::IF()->releaseAsset(pSA);

	mTargetAngle = mCurAngle = mWedgeAngle * (float)VuRand::global().range(0, 10);
}

//*****************************************************************************
void VuRewardWheelEntity::onGameRelease()
{
	for ( const auto &reward : mRewards )
		VuAssetFactory::IF()->releaseAsset(reward.mpImage);
	mRewards.clear();
}

//*****************************************************************************
void VuRewardWheelEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	// Simulate a needle returning to rest
	mNeedleRotation = VuMathUtil::smoothCD(mNeedleRotation, 0.0f, mNeedleAngularVelocity, 0.25f, fdt);

	float rotationLag = mRotationLag;
	if (mState == SPINNING)
	{
		float remainingAngle = mTargetAngle - mCurAngle;
		float hitTheBrakesAngle = mWedgeAngle * 3.0f;
		if (remainingAngle < hitTheBrakesAngle)
		{
			// Clamp it at mRotationLag/2.0f at the low end, but scale down for the
			// last little bit
			rotationLag *= VuMin(0.5f, remainingAngle / hitTheBrakesAngle);
		}
	}

	// Do this simulation whether we're spinning or not
	mCurAngle = VuMathUtil::smoothCD(mCurAngle, mTargetAngle, mAngularVel, rotationLag, fdt);
	if (mMaxAngSpeed > FLT_EPSILON && VuAbs(mAngularVel) > mMaxAngSpeed)
		mAngularVel *= mMaxAngSpeed / VuAbs(mAngularVel);

	if ( mState == SPINNING )
	{
		// Detect passing a peg
		int curWedge = (int)VuFloor((mCurAngle - mWedgeAngle * 0.5f) / mWedgeAngle);
		if (curWedge > mCurWedge && mNeedleRotation > -0.7f)
		{
			mCurWedge = curWedge;
			mNeedleAngularVelocity -= 15.0f;

			mpScriptComponent->getPlug("OnPegTick")->execute();
		}

		// Check to see if spin is finished
		if ( VuAbs(mTargetAngle - mCurAngle) < TARGET_ANGLE_REACHED_THRESHOLD*mWedgeAngle )
		{
			mState = DONE;

			int rewardIndex = getNormalizedSlice(VuRound(mTargetAngle/mWedgeAngle));
			giveReward(rewardIndex);

			VuGameManager::IF()->setCalendarDay(VuGameManager::DAILY_REWARD_CALENDAR, mDaysSince2000);
			VuProfileManager::IF()->save();
			VuCloudSaveManager::IF()->save();

			mpScriptComponent->getPlug("SpinFinished")->execute();
		}
	}
}

//*****************************************************************************
void VuRewardWheelEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawWheel(alpha);
	drawNeedle(alpha);
}

//*****************************************************************************
VuRetVal VuRewardWheelEntity::Spin(const VuParams &params)
{
	if ( mState != SPINNING )
	{
		// determine reward
		int rewardIndex = 0;
		{
			int totalChance = 0;
			for ( const auto &reward : mRewards )
				totalChance += reward.mChance;

			int chance = VuRand::global().range(0, totalChance);
			for ( int i = 0; i < (int)mRewards.size(); i++ )
			{
				if ( chance < mRewards[i].mChance )
				{
					rewardIndex = i;
					break;
				}
				chance -= mRewards[i].mChance;
			}
		}

		mCurAngle = mTargetAngle;
		int curRevs = VuTruncate(mCurAngle/(mWedgeAngle*mRewards.size()));
		mTargetAngle = mWedgeAngle*((mSpinRevs + curRevs)*mRewards.size() + rewardIndex);
		mState = SPINNING;
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuRewardWheelEntity::IsCompleted(const VuParams &params)
{
	bool completed = VuGameManager::IF()->getCalendarDay(VuGameManager::DAILY_REWARD_CALENDAR, mDaysSince2000);
	return VuRetVal(completed);
}

//*****************************************************************************
void VuRewardWheelEntity::drawLayout(bool bSelected)
{
	if ( mRewards.empty() )
		mRewards.resize(1);

	drawWheel(1.0f);
	drawNeedle(mCurAngle);
}

//*****************************************************************************
void VuRewardWheelEntity::drawWheel(float alpha)
{
	int curSlice = VuRound(mCurAngle/mWedgeAngle);

	int sliceCount = VuTruncate(360.0f/mWedgeAngle)/2 - 1;
	for ( int i = -sliceCount; i <= sliceCount; i++ )
	{
		int itemIndex = getNormalizedSlice(curSlice - i);
		const Reward &reward = mRewards[itemIndex];

		VuTexture *pTexture = reward.mpImage ? reward.mpImage->getTexture() : VuGfxUtil::IF()->whiteTexture();

		float angle = (i - curSlice)*mWedgeAngle + mCurAngle + mAngularOffset;
		
		drawBackgroundSlice(angle, curSlice+i);
		drawPegs(angle);
		drawRewardSlice(angle, pTexture);
	}
}

//*****************************************************************************
int VuRewardWheelEntity::getNormalizedSlice(int slice)
{
	if ( slice < 0 )
		slice += (1 - slice/(int)mRewards.size()) * (int)mRewards.size();

	return slice%mRewards.size();
}

//*****************************************************************************
bool VuRewardWheelEntity::isSliceVisible(const VuVector2 &center, float innerRadius, float outerRadius, float minAngle, float maxAngle)
{
	float minAngleSin, minAngleCos;
	VuSinCos(VuDegreesToRadians(minAngle), minAngleSin, minAngleCos);

	float maxAngleSin, maxAngleCos;
	VuSinCos(VuDegreesToRadians(maxAngle), maxAngleSin, maxAngleCos);

	VuVector2 v[4];
	v[0] = VuVector2(innerRadius*minAngleCos, innerRadius*minAngleSin);
	v[1] = VuVector2(outerRadius*minAngleCos, outerRadius*minAngleSin);
	v[2] = VuVector2(innerRadius*maxAngleCos, innerRadius*maxAngleSin);
	v[3] = VuVector2(outerRadius*maxAngleCos, outerRadius*maxAngleSin);

	float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
	for ( int i = 0; i < 4; i++ )
	{
		VuVector2 vert = (center + v[i])/VuUI::IF()->getAuthoringScreenScale();
		vert= VuGfxUtil::IF()->getMatrix().transform(vert);
		minX = VuMin(minX, vert.mX);
		minY = VuMin(minY, vert.mY);
		maxX = VuMax(maxX, vert.mX);
		maxY = VuMax(maxY, vert.mY);
	}

	if ( minX > 1.0f ) return false;
	if ( maxX < 0.0f ) return false;
	if ( minY > 1.0f ) return false;
	if ( maxY < 0.0f ) return false;

	return true;
}

//*****************************************************************************
void VuRewardWheelEntity::drawRewardSlice(float angle, VuTexture *pTexture)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(pData->mTransform);
			VuGfxUtil::IF()->basicShaders()->set2dXyzUvTexture(pData->mpTexture);

			// use scratch pad to build verts
			VuVertex2dXyzUv *verts = (VuVertex2dXyzUv *)VuScratchPad::get(VuScratchPad::GRAPHICS);
			int vertCount = 0;

			VuVertex2dXyzUv *pv = verts;
			float angleStep = (pData->mMaxAngle - pData->mMinAngle)/pData->mSubdivCount;
			for ( int i = 0; i <= pData->mSubdivCount; i++ )
			{
				float angle = VuDegreesToRadians(pData->mMinAngle + (i+0)*angleStep);

				float s, c;
				VuSinCos(angle, s, c);

				float texCoordX = VuLerp(pData->mTexCoord0.mX, pData->mTexCoord1.mX, (float)i/pData->mSubdivCount);

				pv[0].mXyz[0] = (pData->mCenter.mX + pData->mOuterRadius*c)/pData->mScreenScale.mX;
				pv[0].mXyz[1] = (pData->mCenter.mY + pData->mOuterRadius*s)/pData->mScreenScale.mY;
				pv[0].mXyz[2] = 0.0f;
				pv[0].mUv[0] = texCoordX;
				pv[0].mUv[1] = pData->mTexCoord0.mY;

				pv[1].mXyz[0] = (pData->mCenter.mX + pData->mInnerRadius*c)/pData->mScreenScale.mX;
				pv[1].mXyz[1] = (pData->mCenter.mY + pData->mInnerRadius*s)/pData->mScreenScale.mY;
				pv[1].mXyz[2] = 0.0f;
				pv[1].mUv[0] = texCoordX;
				pv[1].mUv[1] = pData->mTexCoord1.mY;

				pv += 2;
				vertCount += 2;
			}

			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, vertCount - 2, verts);
		}

		VuMatrix	mTransform;
		VuVector2	mScreenScale;
		VuVector2	mCenter;
		float		mInnerRadius;
		float		mOuterRadius;
		float		mMinAngle;
		float		mMaxAngle;
		int			mSubdivCount;
		VuTexture	*mpTexture;
		VuVector2	mTexCoord0;
		VuVector2	mTexCoord1;
	};

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 center = uiDrawParams.mPosition*uiDrawParams.mInvAuthScale;
	mAnchor.apply(center, center);
	center /= uiDrawParams.mInvAuthScale;

	float minAngle = angle - 0.5f*(mWedgeAngle - mSpaceAngle);
	float maxAngle = angle + 0.5f*(mWedgeAngle - mSpaceAngle);

	if ( isSliceVisible(center, mInnerRadius, mOuterRadius, minAngle, maxAngle) )
	{
		DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
		pData->mTransform = VuGfxUtil::IF()->getMatrix();
		pData->mScreenScale = VuUI::IF()->getAuthoringScreenScale();
		pData->mCenter = center;
		pData->mInnerRadius = mInnerRadius;
		pData->mOuterRadius = mOuterRadius;
		pData->mMinAngle = minAngle;
		pData->mMaxAngle = maxAngle;
		pData->mSubdivCount = VuRound((pData->mMaxAngle - pData->mMinAngle)/mSubdivisionAngle);
		pData->mpTexture = pTexture;
		pData->mTexCoord0 = mTexCoord0;
		pData->mTexCoord1 = mTexCoord1;

		VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, VuGfxUtil::IF()->basicShaders()->get2dXyzUvMaterial(VuBasicShaders::FLV_MODULATED), VUNULL, &DrawData::callback, uiDrawParams.mDepth);
	}
}

//*****************************************************************************
void VuRewardWheelEntity::drawBackgroundSlice(float angle, int curSlice)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(pData->mTransform);
			VuGfxUtil::IF()->basicShaders()->set2dXyzUvTexture(pData->mpTexture);

			// use scratch pad to build verts
			VuVertex2dXyzUv *verts = (VuVertex2dXyzUv *)VuScratchPad::get(VuScratchPad::GRAPHICS);

			VuVertex2dXyzUv *pv = verts;

			float s, c;

			// Upper left
			VuVector2 upperLeft;
			float angle = VuDegreesToRadians(pData->mMinAngle);
			VuSinCos(angle, s, c);
			upperLeft.set((pData->mCenter.mX + pData->mBackgroundRadius*c),
				(pData->mCenter.mY + pData->mBackgroundRadius*s));

			// Upper right
			VuVector2 upperRight;
			angle = VuDegreesToRadians(pData->mMaxAngle);
			VuSinCos(angle, s, c);
			upperRight.set((pData->mCenter.mX + pData->mBackgroundRadius*c),
				(pData->mCenter.mY + pData->mBackgroundRadius*s));

			VuVector2 leftToRight = upperRight - upperLeft;
			float distLeftToRight = leftToRight.mag();
			leftToRight.normalize();
			leftToRight *= (distLeftToRight / 2.0f);

			VuVector2 lowerLeft = pData->mCenter - leftToRight;
			VuVector2 lowerRight = pData->mCenter + leftToRight;

			pv[0].mXyz[0] = upperLeft.mX / pData->mScreenScale.mX;
			pv[0].mXyz[1] = upperLeft.mY / pData->mScreenScale.mY;
			pv[0].mXyz[2] = 0.0f;
			pv[0].mUv[0] = pData->mWheelTexCoord0.mX;
			pv[0].mUv[1] = pData->mWheelTexCoord0.mY;

			pv[1].mXyz[0] = lowerLeft.mX / pData->mScreenScale.mX;
			pv[1].mXyz[1] = lowerLeft.mY / pData->mScreenScale.mY;
			pv[1].mXyz[2] = 0.0f;
			pv[1].mUv[0] = pData->mWheelTexCoord0.mX;
			pv[1].mUv[1] = pData->mWheelTexCoord1.mY;

			pv[2].mXyz[0] = upperRight.mX / pData->mScreenScale.mX;
			pv[2].mXyz[1] = upperRight.mY / pData->mScreenScale.mY;
			pv[2].mXyz[2] = 0.0f;
			pv[2].mUv[0] = pData->mWheelTexCoord1.mX;
			pv[2].mUv[1] = pData->mWheelTexCoord0.mY;

			pv[3].mXyz[0] = lowerRight.mX / pData->mScreenScale.mX;
			pv[3].mXyz[1] = lowerRight.mY / pData->mScreenScale.mY;
			pv[3].mXyz[2] = 0.0f;
			pv[3].mUv[0] = pData->mWheelTexCoord1.mX;
			pv[3].mUv[1] = pData->mWheelTexCoord1.mY;

			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 2, verts);
		}

		VuMatrix	mTransform;
		VuVector2	mScreenScale;
		VuVector2	mCenter;
		VuVector2	mWheelTexCoord0;
		VuVector2	mWheelTexCoord1;
		float		mBackgroundRadius;
		float		mMinAngle;
		float		mMaxAngle;
		VuTexture	*mpTexture;
	};

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 center = uiDrawParams.mPosition*uiDrawParams.mInvAuthScale;
	mAnchor.apply(center, center);
	center /= uiDrawParams.mInvAuthScale;

	float minAngle = angle - 0.5f*mWedgeAngle;
	float maxAngle = angle + 0.5f*mWedgeAngle;

	VuTexture* pTexture = VUNULL;
	if (curSlice % 2 == 0)
	{
		pTexture = mWheelSlice1.getTexture();
	}
	else
	{
		pTexture = mWheelSlice2.getTexture();
	}

	if (isSliceVisible(center, mInnerRadius, mOuterRadius, minAngle, maxAngle))
	{
		DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
		pData->mTransform = VuGfxUtil::IF()->getMatrix();
		pData->mScreenScale = VuUI::IF()->getAuthoringScreenScale();
		pData->mCenter = center;
		pData->mBackgroundRadius = mBackgroundRadius;
		pData->mMinAngle = minAngle;
		pData->mMaxAngle = maxAngle;
		pData->mpTexture = pTexture;
		pData->mWheelTexCoord0 = mWheelTexCoord0;
		pData->mWheelTexCoord1 = mWheelTexCoord1;

		VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, VuGfxUtil::IF()->basicShaders()->get2dXyzUvMaterial(VuBasicShaders::FLV_MODULATED), VUNULL, &DrawData::callback, uiDrawParams.mDepth);
	}
}


//*****************************************************************************
void VuRewardWheelEntity::drawPegs(float angle)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 center = uiDrawParams.mPosition*uiDrawParams.mInvAuthScale;
	mAnchor.apply(center, center);
	center /= uiDrawParams.mInvAuthScale;

	VuTexture* pTexture = mPegTexture.getTexture();
	if (!pTexture)
	{
		pTexture = VuGfxUtil::IF()->whiteTexture();
	}

	float pegCenterAngle = angle - 0.5f*mWedgeAngle;

	float s, c;
	float sinCosAngle = VuDegreesToRadians(pegCenterAngle);
	VuSinCos(sinCosAngle, s, c);

	VuVector2 screenScale = VuUI::IF()->getAuthoringScreenScale();
	
	VuVector2 pegLocation;
	pegLocation.set(
		(center.mX + (mBackgroundRadius + mPegAdjust.mY)*c + mPegAdjust.mX) / screenScale.mX,
		(center.mY + (mBackgroundRadius + mPegAdjust.mY)*s) / screenScale.mY);

	float pegWidthX = mPegSize.mX / screenScale.mX;
	float pegWidthY = mPegSize.mY / screenScale.mY;

	VuRect destRect;
	float depth = 0.0f;
	destRect.set(	pegLocation.mX - pegWidthX / 2.0f, 
					pegLocation.mY - pegWidthY / 2.0f, 
					pegWidthX,
					pegWidthY);

	VuGfxUtil::IF()->drawTexture2d(depth, pTexture, destRect);
}

void VuRewardWheelEntity::drawNeedle(float alpha)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(pData->mTransform);
			VuGfxUtil::IF()->basicShaders()->set2dXyzUvTexture(pData->mpTexture);

			// use scratch pad to build verts
			VuVertex2dXyzUv *verts = (VuVertex2dXyzUv *)VuScratchPad::get(VuScratchPad::GRAPHICS);

			VuVertex2dXyzUv *pv = verts;

			VuVector3 needleVerts[4] = {
				{ 0.0f, 0.0f, 0.0f },
				{ 0.0f, pData->mNeedleDimensions.mY, 0.0f },
				{ pData->mNeedleDimensions.mX, 0.0f, 0.0f },
				{ pData->mNeedleDimensions.mX, pData->mNeedleDimensions.mY, 0.0f } };

			VuMatrix mat = VuMatrix::identity();

			mat.translate(VuVector3(-pData->mNeedlePivotOffset.mX, -pData->mNeedlePivotOffset.mY, 0.0f));
			mat.rotateZ(pData->mNeedleRotation);
			mat.translate(VuVector3(pData->mNeedleLocation.mX, pData->mNeedleLocation.mY, 0.0f));

			for (int i = 0; i < 4; i++)
			{
				needleVerts[i] = mat.transform(needleVerts[i]);
				pv[i].mXyz[0] = needleVerts[i].mX / pData->mScreenScale.mX;
				pv[i].mXyz[1] = needleVerts[i].mY / pData->mScreenScale.mY;
				pv[i].mXyz[2] = 0.0f;
			}
			
			pv[0].mUv[0] = 0.0f;
			pv[0].mUv[1] = 0.0f;
			pv[1].mUv[0] = 0.0f;
			pv[1].mUv[1] = 1.0f;
			pv[2].mUv[0] = 1.0f;
			pv[2].mUv[1] = 0.0f;
			pv[3].mUv[0] = 1.0f;
			pv[3].mUv[1] = 1.0f;

			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 2, verts);
		}

		VuMatrix	mTransform;
		VuVector2	mScreenScale;
		VuVector2	mCenter;
		VuVector2	mNeedleDimensions;
		VuVector2	mNeedleLocation;
		VuVector2	mNeedlePivotOffset;
		float		mNeedleRotation;
		VuTexture	*mpTexture;
	};

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 center = uiDrawParams.mPosition*uiDrawParams.mInvAuthScale;
	mAnchor.apply(center, center);
	center /= uiDrawParams.mInvAuthScale;

	VuTexture* pTexture = mNeedleTexture.getTexture();
	if (!pTexture)
	{
		pTexture = VuGfxUtil::IF()->whiteTexture();
	}

	VuVector2 needleLocation = center + VuVector2(0.0f, -mNeedleRadius);

	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
	pData->mTransform = VuGfxUtil::IF()->getMatrix();
	pData->mScreenScale = VuUI::IF()->getAuthoringScreenScale();
	pData->mCenter = center;
	pData->mNeedleDimensions = mNeedleSize;
	pData->mNeedleLocation = needleLocation;
	pData->mNeedlePivotOffset = mNeedlePivotOffset;
	pData->mNeedleRotation = mNeedleRotation;
	pData->mpTexture = pTexture;

	VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, VuGfxUtil::IF()->basicShaders()->get2dXyzUvMaterial(VuBasicShaders::FLV_MODULATED), VUNULL, &DrawData::callback, uiDrawParams.mDepth);
}

//*****************************************************************************
bool VuRewardWheelEntity::chooseTodaysCar(std::string &name)
{
	// make list of cars not owned
	std::deque<std::string> cars;
	for ( const auto &car : VuGameManager::IF()->getCars() )
		if ( !car.second.mIsOwned )
			cars.push_back(car.second.mName);

	if ( cars.size() )
	{
		name = cars[mDaysSince2000%cars.size()];
		return true;
	}

	return false;
}

//*****************************************************************************
bool VuRewardWheelEntity::chooseTodaysPowerUp(std::string &name)
{
	// make list of power-ups not owned
	std::deque<std::string> powerUps;
	for ( const auto pPowerUp : VuPowerUpManager::IF()->getPowerUps() )
		if ( !VuGameManager::IF()->isPowerUpOwned(pPowerUp->mName) )
			powerUps.push_back(pPowerUp->mName);

	if ( powerUps.size() )
	{
		name = powerUps[mDaysSince2000%powerUps.size()];
		return true;
	}

	return false;
}

//*****************************************************************************
bool VuRewardWheelEntity::isStatMaxed(const std::string &stat)
{
	int level = VuGameManager::IF()->getCurCar().getStat(stat.c_str());
	return level >= VuGameManager::IF()->getCurCar().getMaxLevel();
}

//*****************************************************************************
void VuRewardWheelEntity::giveReward(int rewardIndex)
{
	const Reward &reward = mRewards[rewardIndex];
	char str[256] = "";

	if ( reward.mType == "SC" )
	{
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_STANDARD, reward.mAmount);
		VuStringUtil::integerFormat(reward.mAmount, str, sizeof(str));
	}
	else if ( reward.mType == "PC" )
	{
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_PREMIUM, reward.mAmount);
		VuStringUtil::integerFormat(reward.mAmount, str, sizeof(str));
	}
	else if ( reward.mType == "Car" )
	{
		VuGameManager::IF()->giveCar(reward.mName);
		VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("Car_" + reward.mName).c_str());
	}
	else if ( reward.mType == "PowerUp" )
	{
		VuGameManager::IF()->givePowerUp(reward.mName);
		VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("PowerUp_" + reward.mName).c_str());
	}
	else if ( reward.mType == "Upgrade" )
	{
		VuGameManager::IF()->giveCarUpgrade(reward.mName);
		VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("Stat_" + reward.mName).c_str());
	}
	else if ( reward.mType == "Tokens" )
	{
		VuGameManager::IF()->refillTokens();
	}

	VuGameFontMacros::IF()->setMacro("DAILY_REWARD", str);

	VU_SPRINTF(str, sizeof(str), "Daily_Reward_Text_%s", reward.mType.c_str());
	VuGameFontMacros::IF()->setMacro("DAILY_REWARD_TEXT", VuStringDB::IF()->getString(str).c_str());

	// make sure we don't hit the same thing twice in one session
	mRewards[rewardIndex].mChance = 0;
}
