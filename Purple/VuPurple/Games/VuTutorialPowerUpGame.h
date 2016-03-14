//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TutorialPowerUpGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"
#include "VuCarSpawner.h"


class VuTutorialPowerUpGame : public VuGame
{
	DECLARE_RTTI

public:
	VuTutorialPowerUpGame(VuProject *pProject);
	~VuTutorialPowerUpGame();

private:
	// event handlers
	void			OnStartCarEffect(const VuParams &params);

	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();
	virtual void	onEnd();
	virtual void	onTick(float fdt);

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
	void			onPostGameTick(float fdt);

	VuCarSpawner	mCarSpawner;
};