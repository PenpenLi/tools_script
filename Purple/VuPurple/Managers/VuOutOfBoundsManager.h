//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Out of Bounds Manager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Math/VuVector3.h"


class VuOutOfBoundsManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuOutOfBoundsManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init() { return true; }
	virtual void release();

public:
	bool	isOutOfBounds(const VuVector3 &position, float &recoverTime);

	class VuOutOfBoundsIF { public: virtual bool isOutOfBounds(const VuVector3 &pos) const = 0; };
	void	addZone(const VuOutOfBoundsIF *pIF, const VuVector3 &position, float radius, float recoverTime);
	void	removeZone(const VuOutOfBoundsIF *pIF);

private:
	struct Zone
	{
		VuVector3				mPosition;
		float					mSquaredRadius;
		float					mRecoverTime;
		const VuOutOfBoundsIF	*mpIF;
	};
	typedef std::vector<Zone> Zones;
	Zones	mZones;
};