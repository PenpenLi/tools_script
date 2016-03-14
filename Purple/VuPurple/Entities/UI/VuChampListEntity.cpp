//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Champ List Entity
// 
//*****************************************************************************

#include "VuHListEntity.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/DB/VuStringDB.h"


class VuChampListEntity : public VuHListEntity
{
	DECLARE_RTTI

public:
	VuChampListEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	virtual int					getItemCount();
	virtual void				getItemName(int index, std::string &itemName);
	virtual void				getItemImage(int index, std::string &itemImage);
	virtual bool				isItemLocked(int index) { return false; }
	virtual void				getUnlockText(int index, std::string &text) {}
	virtual void				onItemSelected(int index) {}
	virtual void				onItemChosen(int index);
	virtual void				onItemLocked(int inex) {}
	virtual void				onDrawItem(int index, const VuVector2 &offset, float scale) {}

	void						setCurrentChamp(int index);

	std::vector<std::string>	mChamps;
};


IMPLEMENT_RTTI(VuChampListEntity, VuHListEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuChampListEntity);


//*****************************************************************************
VuChampListEntity::VuChampListEntity()
{
	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ChampChosen);

	// build champ list
	const char *champNames = VuGameUtil::IF()->isDemoMode() ? "DemoChamps" : "Champs";
	const VuJsonContainer &champs = VuGameUtil::IF()->constantDB()["Names"][champNames];
	for ( int i = 0; i < champs.size(); i++ )
		mChamps.push_back(champs[i].asString());
}

//*****************************************************************************
void VuChampListEntity::onGameInitialize()
{
	VuHListEntity::onGameInitialize();

	float scrollPos = VuProfileManager::IF()->dataRead()["Lists"]["ChampScrollPos"].asFloat();
	scrollPos = VuClamp(scrollPos, calcScrollMin(), calcScrollMax());
	mScrollPos = mScrollTargetPos = scrollPos;

	const std::string &champName = VuProfileManager::IF()->dataRead()["Lists"]["Champ"].asString();
	for ( int i = 0; i < (int)mChamps.size(); i++ )
		if ( champName == mChamps[i] )
			setSelectedItem(i, true);
}

//*****************************************************************************
void VuChampListEntity::onGameRelease()
{
	const std::string &champName = mChamps[getSelectedItem()];
	VuProfileManager::IF()->dataWrite()["Lists"]["Champ"].putValue(champName);
	VuProfileManager::IF()->dataWrite()["Lists"]["ChampScrollPos"].putValue(mScrollTargetPos);

	VuHListEntity::onGameRelease();
}

//*****************************************************************************
int VuChampListEntity::getItemCount()
{
	return (int)mChamps.size();
}

//*****************************************************************************
void VuChampListEntity::getItemName(int index, std::string &itemName)
{
	itemName = mChamps[index];
}

//*****************************************************************************
void VuChampListEntity::getItemImage(int index, std::string &itemImage)
{
	itemImage = VuGameUtil::IF()->champDB()[mChamps[index]]["Image"].asString();
}

//*****************************************************************************
void VuChampListEntity::onItemChosen(int index)
{
	setCurrentChamp(index);

	mpScriptComponent->getPlug("ChampChosen")->execute();
}

//*****************************************************************************
void VuChampListEntity::setCurrentChamp(int index)
{
	VuJsonContainer &eventData = VuGameUtil::IF()->dataWrite()["EventData"];
	eventData["ChampName"].putValue(mChamps[index]);
}
