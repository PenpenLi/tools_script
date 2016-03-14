//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Boing util
// 
//*****************************************************************************

#include "VuBoing.h"
#include "VuEngine/Math/VuMath.h"
#include "VuEngine/Json/VuJsonContainer.h"


//*****************************************************************************
void VuBoing::load(const VuJsonContainer &data)
{
	data["Rate"].getValue(mRate);
	data["Scale"][0].getValue(mScaleMin);
	data["Scale"][1].getValue(mScaleMax);
	data["Duration"].getValue(mDuration);
}

//*****************************************************************************
void VuBoing::tick(float fdt)
{
	mCurTime += fdt;

	mScale =  0.5f + 0.5f*VuSin(mRate*mCurTime*VU_2PI);
	mScale = VuLerp(mScaleMin, mScaleMax, mScale);
	mScale = VuLerp(1.0f, mScale, VuMax(0.0f, (mDuration - mCurTime)/mDuration));
}
