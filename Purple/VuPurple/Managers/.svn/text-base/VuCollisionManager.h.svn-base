//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CollisionManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Dynamics/VuDynamicsCallbacks.h"
#include "VuEngine/Containers/VuArray.h"

class VuDBAsset;
namespace FMOD { class EventGroup; class Event; }


class VuCollisionManager : public VuSystemComponent, public VuGlobalContactCallback
{
	DECLARE_SYSTEM_COMPONENT(VuCollisionManager)

protected:
	// created/destroyed by barracuda
	friend class VuPurple;
	bool	init();
	void	postInit();
	void	release();

public:
	VuCollisionManager();
	~VuCollisionManager();

	void			reset();

	const char		*getWheelDrivePfxName(VUUINT8 surfaceType)	{ return mWheelDrivePfxTable[surfaceType]; }
	const char		*getWheelSlidePfxName(VUUINT8 surfaceType)	{ return mWheelSlidePfxTable[surfaceType]; }

	static float	calculateImpactIntensity(const VuRigidBody *pRB0, const VuRigidBody *pRB1, const VuVector3 &pos, const VuVector3 &nor1);

private:
	// VuGlobalContactCallback
	virtual void	onGlobalContactAdded(VuContactPoint &cp);

	struct VuImpactEvent
	{
		int			mSurfaceType;
		VuVector3	mPosition;
		float		mIntensity;
	};
	struct VuManifold
	{
		const VuRigidBody	*mpBody0;
		const VuRigidBody	*mpBody1;
		int					mSurfaceType0;
		int					mSurfaceType1;
		bool				mbActive;
		FMOD::Event			*mpEvent;
		VUUINT32			mhPfx;
	};
	enum { MAX_NAME_LEN = 64 };
	typedef char TableEntry[MAX_NAME_LEN];
	typedef VuArray<TableEntry> EventNameTable;
	typedef VuArray<VuImpactEvent> ImpactEvents;
	typedef VuArray<VuManifold> Manifolds;

	void				loadEventNameTable(const VuDBAsset *pDBAsset, const char *strTableName, EventNameTable &table, const char *strPrefix = "");
	void				tickDecision(float fdt);
	void				playImpactEvents();
	void				updateManifolds();
	void				updateDevStats();

	FMOD::EventGroup	*mpImpactEventGroup;
	FMOD::EventGroup	*mpScrapeEventGroup;
	int					mSurfaceTypeCount;
	EventNameTable		mImpactSfxTable;
	EventNameTable		mScrapeSfxTable;
	EventNameTable		mImpactPfxTable;
	EventNameTable		mScrapePfxTable;
	EventNameTable		mWheelDrivePfxTable;
	EventNameTable		mWheelSlidePfxTable;
	ImpactEvents		mImpactEvents;
	Manifolds			mManifolds;
};
