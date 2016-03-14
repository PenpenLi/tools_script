//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Boing util
// 
//*****************************************************************************

#pragma once

class VuJsonContainer;


class VuBoing
{
public:
	VuBoing() : mRate(4.0f), mScaleMin(0.8f), mScaleMax(1.2f), mDuration(1.0f), mScale(1.0f), mCurTime(0.0f) {}

	void	load(const VuJsonContainer &data);
	void	tick(float fdt);

	void	reset() { mScale = 1.0f; mCurTime = 0.0f; }
	float	getCurTime() const { return mCurTime; }
	bool	isDone() const { return mCurTime >= mDuration; }

	// parameters
	float	mRate;
	float	mScaleMin;
	float	mScaleMax;
	float	mDuration;

	// output
	float	mScale;

private:
	float	mCurTime;
};