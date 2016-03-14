//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ShootingGalleryGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"
#include "VuCarSpawner.h"


class VuShootingGalleryGame : public VuGame
{
	DECLARE_RTTI

public:
	VuShootingGalleryGame(VuProject *pProject);
	~VuShootingGalleryGame();

private:
	virtual int		getNumOpponents() { return 0; }
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();
	virtual void	onEnd();
	virtual void	onTick(float fdt);
	virtual void	onCarFinished(VuCarEntity *pCar);

	void			onIntroEnter();
	void			onIntroExit();
	void			onIntroTick(float fdt);

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameTick(float fdt);

	void			onPostGameEnter();
	void			onPostGameExit();

	VuCarSpawner	mCarSpawner;
	int				mTimeLimit;
};