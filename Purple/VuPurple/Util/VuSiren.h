//*****************************************************************************
//
//  Copyright (c) 2011-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Siren
// 
//*****************************************************************************

#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Gfx/Corona/VuCorona.h"

class VuJsonContainer;
class VuGfxDrawParams;


class VuSiren
{
public:
	VuSiren() : mTransform(VuMatrix::identity()), mPatternDuration(0), mPatternTimer(0) {}

	void		loadData(const VuJsonContainer &data);

	void		tick(float fdt);

	void		updateVisibility(const VuMatrix &transform);
	void		draw(const VuMatrix &transform, const VuGfxDrawParams &params);

	struct PatternEntry
	{
		float	mTime;
		float	mAmount;
	};
	typedef std::vector<PatternEntry> Pattern;

	VuMatrix	mTransform;
	VuCorona	mCorona;
	Pattern		mPattern;
	float		mPatternDuration;
	float		mPatternTimer;
};

