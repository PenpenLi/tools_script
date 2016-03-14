//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Table entities for game
// 
//*****************************************************************************


#include "VuTableEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Util/VuStringUtil.h"


// utility
static const char *GetCarName(const std::string &car)
{
	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Car_%s", car.c_str());
	return VuStringDB::IF()->getString(stringId).c_str();
}

static const char *GetDriverName(const std::string &driver)
{
	char stringId[64];
	VU_SPRINTF(stringId, sizeof(stringId), "Driver_%s", driver.c_str());
	return VuStringDB::IF()->getString(stringId).c_str();
}

//*****************************************************************************
// VuRaceResultsTableEntity
//*****************************************************************************

class VuRaceResultsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuRaceResultsTableEntity();

private:
	virtual int				getRowCount();
	virtual const char		*getItemText(int iRow, int iCol);
	virtual bool			getRowHighlight(int iRow);

	VuCarEntity				*getCarForRow(int iRow);

	enum { COL_PLACE, COL_DRIVER, COL_CAR, NUM_COLUMNS };

	char					mText[128];
};


IMPLEMENT_RTTI(VuRaceResultsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRaceResultsTableEntity);


//*****************************************************************************
VuRaceResultsTableEntity::VuRaceResultsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Driver", true);
	ADD_COLUMN("Car", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
int VuRaceResultsTableEntity::getRowCount()
{
	return VuCarManager::IF()->getCarCount();
}

//*****************************************************************************
const char *VuRaceResultsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	if ( VuCarEntity *pCar = getCarForRow(iRow) )
	{
		if ( iCol == COL_PLACE )
		{
			VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
		}
		else if ( iCol == COL_DRIVER )
		{
			VU_STRCPY(mText, sizeof(mText), GetDriverName(pCar->getDriverName()));
		}
		else if ( iCol == COL_CAR )
		{
			VU_STRCPY(mText, sizeof(mText), GetCarName(pCar->getCarName()));
		}
	}

	return mText;
}

//*****************************************************************************
bool VuRaceResultsTableEntity::getRowHighlight(int iRow)
{
	if ( VuCarEntity *pCar = getCarForRow(iRow) )
		return pCar->getDriver()->isHuman() && pCar->getDriver()->isLocal();

	return false;
}

//*****************************************************************************
VuCarEntity *VuRaceResultsTableEntity::getCarForRow(int iRow)
{
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
		if ( pCar->getStats().mPlace == iRow + 1 )
			return pCar;
	}

	return VUNULL;
}


//*****************************************************************************
// VuBoostBlitzResultsTableEntity
//*****************************************************************************

class VuBoostBlitzResultsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuBoostBlitzResultsTableEntity();

	virtual void			onGameInitialize();

private:
	virtual int				getRowCount() { return (int)mRows.size(); }
	virtual const char		*getItemText(int iRow, int iCol);
	virtual bool			getRowHighlight(int iRow) { return mRows[iRow].mPlayer; }

	enum { COL_PLACE, COL_NAME, COL_TIME, NUM_COLUMNS };

	struct Row
	{
		std::string	mName;
		float		mTime;
		bool		mPlayer;
	};
	typedef std::vector<Row> Rows;
	Rows			mRows;
	char			mText[128];
};


IMPLEMENT_RTTI(VuBoostBlitzResultsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBoostBlitzResultsTableEntity);


//*****************************************************************************
VuBoostBlitzResultsTableEntity::VuBoostBlitzResultsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Name", true);
	ADD_COLUMN("Time", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
void VuBoostBlitzResultsTableEntity::onGameInitialize()
{
	VuTableEntity::onGameInitialize();

	const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

	for ( int i = 0; i < criteria.size(); i++ )
	{
		mRows.resize(mRows.size() + 1);
		Row &row = mRows.back();

		char str[128];
		VU_SPRINTF(str, sizeof(str), "GameResult_%d", i+1);
		row.mName = VuStringDB::IF()->getString(str);
		row.mTime = criteria[i].asFloat();
		row.mPlayer = false;
	}

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCar(0) )
	{
		mRows.resize(mRows.size() + 1);
		Row &row = mRows.back();

		row.mName = VuStringDB::IF()->getString("Driver_" + pCar->getDriverName());
		row.mTime = (float)pCar->getStats().mTotalTime;
		row.mPlayer = true;
	}

	std::sort(mRows.begin(), mRows.end(), [](const Row &row0, const Row &row1)
	{
		return row0.mTime < row1.mTime;
	});
}

//*****************************************************************************
const char *VuBoostBlitzResultsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	if ( iCol == COL_PLACE )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
	}
	else if ( iCol == COL_NAME )
	{
		VU_STRCPY(mText, sizeof(mText), mRows[iRow].mName.c_str());
	}
	else if ( iCol == COL_TIME )
	{
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, mRows[iRow].mTime, mText, sizeof(mText));
	}

	return mText;
}


//*****************************************************************************
// VuGameResultsTableEntity
//*****************************************************************************

class VuGameResultsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuGameResultsTableEntity();

	virtual void			onGameInitialize();

private:
	virtual int				getRowCount() { return (int)mRows.size(); }
	virtual const char		*getItemText(int iRow, int iCol);
	virtual bool			getRowHighlight(int iRow) { return mRows[iRow].mPlayer; }

	enum { COL_PLACE, COL_NAME, COL_SCORE, NUM_COLUMNS };

	struct Row
	{
		std::string	mName;
		int			mScore;
		bool		mPlayer;
	};
	typedef std::vector<Row> Rows;
	Rows			mRows;
	char			mText[128];
};


IMPLEMENT_RTTI(VuGameResultsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameResultsTableEntity);


//*****************************************************************************
VuGameResultsTableEntity::VuGameResultsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Name", true);
	ADD_COLUMN("Score", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
void VuGameResultsTableEntity::onGameInitialize()
{
	VuTableEntity::onGameInitialize();

	const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

	for ( int i = 0; i < criteria.size(); i++ )
	{
		mRows.resize(mRows.size() + 1);
		Row &row = mRows.back();

		char str[128];
		VU_SPRINTF(str, sizeof(str), "GameResult_%d", i+1);
		row.mName = VuStringDB::IF()->getString(str);
		row.mScore = criteria[i].asInt();
		row.mPlayer = false;
	}

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCar(0) )
	{
		mRows.resize(mRows.size() + 1);
		Row &row = mRows.back();

		row.mName = VuStringDB::IF()->getString("Driver_" + pCar->getDriverName());
		row.mScore = pCar->getStats().mScore;
		row.mPlayer = true;
	}

	std::sort(mRows.begin(), mRows.end(), [](const Row &row0, const Row &row1)
	{
		if ( row0.mScore != row1.mScore )
			return row0.mScore > row1.mScore;

		return row0.mPlayer;
	});
}

//*****************************************************************************
const char *VuGameResultsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	if ( iCol == COL_PLACE )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
	}
	else if ( iCol == COL_NAME )
	{
		VU_STRCPY(mText, sizeof(mText), mRows[iRow].mName.c_str());
	}
	else if ( iCol == COL_SCORE )
	{
		VuStringUtil::integerFormat(mRows[iRow].mScore, mText, sizeof(mText));
	}

	return mText;
}


//*****************************************************************************
// VuSplitScreenResultsTableEntity
//*****************************************************************************

class VuSplitScreenResultsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuSplitScreenResultsTableEntity();

private:
	virtual int				getRowCount();
	virtual const char		*getItemText(int iRow, int iCol);

	VuCarEntity				*getCarForRow(int iRow);

	enum { COL_PLACE, COL_DRIVER, COL_CAR, COL_POINTS, NUM_COLUMNS };

	char					mText[128];
};


IMPLEMENT_RTTI(VuSplitScreenResultsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSplitScreenResultsTableEntity);


//*****************************************************************************
VuSplitScreenResultsTableEntity::VuSplitScreenResultsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Driver", true);
	ADD_COLUMN("Car", true);
	ADD_COLUMN("Points", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
int VuSplitScreenResultsTableEntity::getRowCount()
{
	return VuCarManager::IF()->getCarCount();
}

//*****************************************************************************
const char *VuSplitScreenResultsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	if ( VuCarEntity *pCar = getCarForRow(iRow) )
	{
		VuCarStats &stats = pCar->getStats();

		if ( iCol == COL_PLACE )
		{
			VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
		}
		else if ( iCol == COL_DRIVER )
		{
			VU_STRCPY(mText, sizeof(mText), GetDriverName(pCar->getDriverName()));
		}
		else if ( iCol == COL_CAR )
		{
			VU_STRCPY(mText, sizeof(mText), GetCarName(pCar->getCarName()));
		}
		else if ( iCol == COL_POINTS )
		{
			if ( stats.mDNF )
				VU_STRCPY(mText, sizeof(mText), VuStringDB::IF()->getString("Common_DNF").c_str());
			else if ( stats.mHasFinished )
				VU_SPRINTF(mText, sizeof(mText), "%d", VuGameUtil::IF()->constantDB()["PointsTable"][iRow].asInt());
		}
	}

	return mText;
}

//*****************************************************************************
VuCarEntity *VuSplitScreenResultsTableEntity::getCarForRow(int iRow)
{
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
		if ( pCar->getStats().mPlace == iRow + 1 )
			return pCar;
	}

	return VUNULL;
}


//*****************************************************************************
// VuSplitScreenStandingsTableEntity
//*****************************************************************************

class VuSplitScreenStandingsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuSplitScreenStandingsTableEntity();

private:
	virtual int				getRowCount();
	virtual const char		*getItemText(int iRow, int iCol);

	const VuJsonContainer	&getDataForRow(int iRow);

	enum { COL_PLACE, COL_DRIVER, COL_CAR, COL_POINTS, NUM_COLUMNS };

	char					mText[128];
};


IMPLEMENT_RTTI(VuSplitScreenStandingsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSplitScreenStandingsTableEntity);


//*****************************************************************************
VuSplitScreenStandingsTableEntity::VuSplitScreenStandingsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Driver", true);
	ADD_COLUMN("Car", true);
	ADD_COLUMN("Points", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
int VuSplitScreenStandingsTableEntity::getRowCount()
{
	return VuGameUtil::IF()->dataRead()["ChampTable"].size();
}

//*****************************************************************************
const char *VuSplitScreenStandingsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	const VuJsonContainer &data = getDataForRow(iRow);

	if ( iCol == COL_PLACE )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
	}
	else if ( iCol == COL_DRIVER )
	{
		VU_STRCPY(mText, sizeof(mText), GetDriverName(data["Driver"].asString()));
	}
	else if ( iCol == COL_CAR )
	{
		VU_STRCPY(mText, sizeof(mText), GetCarName(data["Car"].asString()));
	}
	else if ( iCol == COL_POINTS )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", data["Points"].asInt());
	}

	return mText;
}

//*****************************************************************************
const VuJsonContainer &VuSplitScreenStandingsTableEntity::getDataForRow(int iRow)
{
	const VuJsonContainer &champTable = VuGameUtil::IF()->dataRead()["ChampTable"];

	for ( int i = 0; i < champTable.size(); i++ )
		if ( champTable[i]["Rank"].asInt() == iRow + 1 )
			return champTable[i];

	return VuJsonContainer::null;
}


//*****************************************************************************
// VuCarChampStandingsTableEntity
//*****************************************************************************

class VuCarChampStandingsTableEntity : public VuTableEntity
{
	DECLARE_RTTI
	
public:
	VuCarChampStandingsTableEntity();

	virtual void			onGameInitialize();

private:
	virtual int				getRowCount() { return (int)mEntries.size(); }
	virtual const char		*getItemText(int iRow, int iCol);
	virtual bool			getRowHighlight(int iRow) { return mEntries[iRow].mPlayer; }

	enum { COL_PLACE, COL_DRIVER, COL_CAR, COL_POINTS, NUM_COLUMNS };

	VuGameUtil::CarChampTable	mEntries;
	char					mText[128];
};


IMPLEMENT_RTTI(VuCarChampStandingsTableEntity, VuTableEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampStandingsTableEntity);


//*****************************************************************************
VuCarChampStandingsTableEntity::VuCarChampStandingsTableEntity()
{
	// properties
	ADD_COLUMN("Place", true);
	ADD_COLUMN("Driver", true);
	ADD_COLUMN("Car", true);
	ADD_COLUMN("Points", true);

	VUASSERT(mColumns.size() == NUM_COLUMNS, "column mismatch");
}

//*****************************************************************************
void VuCarChampStandingsTableEntity::onGameInitialize()
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	VuGameUtil::IF()->createCarChampTable(carName, mEntries);
}

//*****************************************************************************
const char *VuCarChampStandingsTableEntity::getItemText(int iRow, int iCol)
{
	mText[0] = '\0';

	const VuGameUtil::CarChampTableEntry &entry = mEntries[iRow];

	if ( iCol == COL_PLACE )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", iRow + 1);
	}
	else if ( iCol == COL_DRIVER )
	{
		VU_STRCPY(mText, sizeof(mText), GetDriverName(entry.mDriver));
	}
	else if ( iCol == COL_CAR )
	{
		VU_STRCPY(mText, sizeof(mText), GetCarName(entry.mCar));
	}
	else if ( iCol == COL_POINTS )
	{
		VU_SPRINTF(mText, sizeof(mText), "%d", entry.mPoints);
	}

	return mText;
}
