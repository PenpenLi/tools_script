//*****************************************************************************
//
//  Copyright (c) 2011-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Siren
// 
//*****************************************************************************

#include "VuSiren.h"
#include "VuEngine/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
void VuSiren::loadData(const VuJsonContainer &data)
{
	VuVector3 relPos(0,0,0), relRot(0,0,0);
	VuDataUtil::getValue(data["RelPos"], relPos);
	VuDataUtil::getValue(data["RelRot"], relRot);
	VuDataUtil::getValue(data["DrawDist"], mCorona.mDrawDist);
	VuDataUtil::getValue(data["FadeDist"], mCorona.mFadeDist);
	VuDataUtil::getValue(data["QueryRadius"], mCorona.mQueryRadius);
	VuDataUtil::getValue(data["ConeAngle"], mCorona.mConeAngle);
	VuDataUtil::getValue(data["PenumbraAngle"], mCorona.mPenumbraAngle);
	VuDataUtil::getValue(data["TextureColor"], mCorona.mTextureColor);
	VuDataUtil::getValue(data["TextureSizeScreenSpace"], mCorona.mbTextureSizeScreenSpace);
	VuDataUtil::getValue(data["TextureSize"], mCorona.mTextureSize);
	VuDataUtil::getValue(data["RotationOffset"], mCorona.mRotationOffset);
	VuDataUtil::getValue(data["RotationAmount"], mCorona.mRotationAmount);

	mCorona.setTextureAsset(data["TextureName"].asString());

	mCorona.mConeAngle = VuDegreesToRadians(mCorona.mConeAngle);
	mCorona.mPenumbraAngle = VuDegreesToRadians(mCorona.mPenumbraAngle);
	mCorona.mRotationOffset = VuDegreesToRadians(mCorona.mRotationOffset);
	mCorona.mRotationAmount = VuDegreesToRadians(mCorona.mRotationAmount);

	mCorona.mCollisionMask = COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_ENGINE_CORONA_OCCLUDER;

	mTransform.setEulerAngles(VuDegreesToRadians(relRot));
	mTransform.setTrans(relPos);

	const VuJsonContainer &patternData = data["Pattern"];
	for ( int i = 0; i < patternData.size(); i++ )
	{
		VuSiren::PatternEntry entry;
		entry.mTime = patternData[i][0].asFloat();
		entry.mAmount = patternData[i][1].asFloat();
		mPattern.push_back(entry);
		mPatternDuration = VuMax(mPatternDuration, entry.mTime);
	}
}

//*****************************************************************************
void VuSiren::tick(float fdt)
{
	// pattern
	mPatternTimer += fdt;
	if ( mPatternTimer > mPatternDuration )
		mPatternTimer -= mPatternDuration;

	for ( int j = 1; j < (int)mPattern.size(); j++ )
	{
		if ( mPattern[j].mTime >= mPatternTimer )
		{
			const VuSiren::PatternEntry &entry0 = mPattern[j - 1];
			const VuSiren::PatternEntry &entry1 = mPattern[j];

			float ratio = (mPatternTimer - entry0.mTime)/(entry1.mTime - entry0.mTime);
			float amount = VuLerp(entry0.mAmount, entry1.mAmount, ratio);
			mCorona.mTextureColor.mA = (VUUINT8)VuRound(amount*255.0f);

			break;
		}
	}
}

//*****************************************************************************
void VuSiren::updateVisibility(const VuMatrix &transform)
{
	mCorona.updateVisibility(transform.transform(mTransform.getTrans()));
}

//*****************************************************************************
void VuSiren::draw(const VuMatrix &transform, const VuGfxDrawParams &params)
{
	mCorona.draw(mTransform*transform, params);
}