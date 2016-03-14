//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RaceGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"


class VuSplitScreenRaceGame : public VuGame
{
	DECLARE_RTTI

public:
	VuSplitScreenRaceGame(VuProject *pProject);
	~VuSplitScreenRaceGame();

private:
	virtual int		getNumOpponents() { return 0; }
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();

	void			checkPauseInput();

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameTick(float fdt);

	void			onPostGameEnter();
	void			onPostGameExit();
	void			onPostGameTick(float fdt);

	void			updatePlacing();

	float			mTimeRemaining;
	float			mPostGameDelay;

	// placing/ranking
	class VuPlacingComp
	{
	public:
		VuPlacingComp(const Cars &cars) : mCars(cars) {}
		bool operator()(int i0, int i1);
		const Cars &mCars;
	};
	class VuChampRankComp
	{
	public:
		VuChampRankComp(const Cars &cars) : mCars(cars) {}
		bool operator()(int i0, int i1);
		const Cars &mCars;
	};
	typedef std::vector<int> Order;
	Order		mPlacing;
	Order		mChampRanks;
};