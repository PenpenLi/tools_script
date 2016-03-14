//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  DuelGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"


class VuDuelGame : public VuGame
{
	DECLARE_RTTI

public:
	VuDuelGame(VuProject *pProject);
	~VuDuelGame();

private:
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();
	virtual void	onEnd();
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
	void			onPostGameTick(float fdt);

	void			updatePlacing();

	// placing
	class VuPlacingComp
	{
	public:
		VuPlacingComp(const Cars &cars) : mCars(cars) {}
		bool operator()(int i0, int i1);
		const Cars &mCars;
	};
	typedef std::vector<int> Placing;
	Placing		mPlacing;
};