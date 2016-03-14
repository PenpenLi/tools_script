//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Lounge List Entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"

class VuLoungeListItemEntity;


class VuLoungeListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuLoungeListEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// scripting
	VuRetVal		Prev(const VuParams &params = VuParams());
	VuRetVal		Next(const VuParams &params = VuParams());

	void			setSelectedItem(int index);

	// components
	VuScriptComponent	*mpScriptComponent;

	VuScriptRef			*mpFirstItemRef;

	typedef std::vector<VuLoungeListItemEntity *> Items;
	Items			mItems;
	int				mCurItem;
};

IMPLEMENT_RTTI(VuLoungeListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLoungeListEntity);


//*****************************************************************************
class VuLoungeListItemEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuLoungeListItemEntity();

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mDriver;

	VuScriptRef			*mpNextItemRef;
};

IMPLEMENT_RTTI(VuLoungeListItemEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLoungeListItemEntity);


//*****************************************************************************
VuLoungeListEntity::VuLoungeListEntity():
	mCurItem(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	mpFirstItemRef = ADD_SCRIPT_REF(mpScriptComponent, FirstItem, VuLoungeListItemEntity::msRTTI);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuLoungeListEntity, Prev);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuLoungeListEntity, Next);
}

//*****************************************************************************
void VuLoungeListEntity::onGameInitialize()
{
	// gather items
	VuLoungeListItemEntity *pItem = mpFirstItemRef->getRefEntity<VuLoungeListItemEntity>();
	while ( pItem )
	{
		mItems.push_back(pItem);
		pItem = pItem->mpNextItemRef->getRefEntity<VuLoungeListItemEntity>();
	}

	// set default item
	int index = 0;
	for ( int i = 0; i < (int)mItems.size(); i++ )
		if ( mItems[i]->mDriver == VuGameManager::IF()->getCurDriverName() )
			index = i;

	// look for interesting driver
	for ( int i = 0; i < (int)mItems.size(); i++ )
	{
		if ( VuGameManager::IF()->isDriverInteresting(mItems[i]->mDriver) )
			index = i;
		VuGameManager::IF()->clearDriverInteresting(mItems[i]->mDriver);
	}

	setSelectedItem(index);
}

//*****************************************************************************
void VuLoungeListEntity::onGameRelease()
{
	const std::string &curDriver = mItems[mCurItem]->mDriver;

	if ( VuGameManager::IF()->getDriver(curDriver).isOnTeam() )
		VuGameManager::IF()->setCurDriver(curDriver);

	VuGameUtil::IF()->setLoungeDriver("");
}

//*****************************************************************************
VuRetVal VuLoungeListEntity::Prev(const VuParams &params)
{
	int newIndex = int((mCurItem - 1 + mItems.size())%mItems.size());
	setSelectedItem(newIndex);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuLoungeListEntity::Next(const VuParams &params)
{
	int newIndex = (mCurItem + 1)%mItems.size();
	setSelectedItem(newIndex);

	return VuRetVal();
}

//*****************************************************************************
void VuLoungeListEntity::setSelectedItem(int index)
{
	mCurItem = index;

	VuGameUtil::IF()->setLoungeDriver(mItems[mCurItem]->mDriver);

	mItems[index]->mpScriptComponent->getPlug("OnSelected")->execute();
}

//*****************************************************************************
VuLoungeListItemEntity::VuLoungeListItemEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuJsonStringArrayEnumProperty("Driver", mDriver, VuGameUtil::IF()->constantDB()["Names"]["Drivers"]));

	// scripting
	mpNextItemRef = ADD_SCRIPT_REF(mpScriptComponent, NextItem, VuLoungeListItemEntity::msRTTI);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSelected);
}
