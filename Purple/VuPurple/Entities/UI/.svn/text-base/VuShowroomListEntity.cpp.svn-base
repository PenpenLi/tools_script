//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Showroom List Entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"

class VuShowroomListItemEntity;


class VuShowroomListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuShowroomListEntity();

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

	typedef std::vector<VuShowroomListItemEntity *> Items;
	Items			mItems;
	int				mCurItem;
};

IMPLEMENT_RTTI(VuShowroomListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuShowroomListEntity);


//*****************************************************************************
class VuShowroomListItemEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuShowroomListItemEntity();

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mCar;

	VuScriptRef			*mpNextItemRef;
};

IMPLEMENT_RTTI(VuShowroomListItemEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuShowroomListItemEntity);


//*****************************************************************************
VuShowroomListEntity::VuShowroomListEntity():
	mCurItem(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	mpFirstItemRef = ADD_SCRIPT_REF(mpScriptComponent, FirstItem, VuShowroomListItemEntity::msRTTI);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuShowroomListEntity, Prev);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuShowroomListEntity, Next);
}

//*****************************************************************************
void VuShowroomListEntity::onGameInitialize()
{
	// gather items
	VuShowroomListItemEntity *pItem = mpFirstItemRef->getRefEntity<VuShowroomListItemEntity>();
	while ( pItem )
	{
		mItems.push_back(pItem);
		pItem = pItem->mpNextItemRef->getRefEntity<VuShowroomListItemEntity>();
	}

	// set default item
	int index = 0;
	for ( int i = 0; i < (int)mItems.size(); i++ )
		if ( mItems[i]->mCar == VuGameManager::IF()->getCurCarName() )
			index = i;

	setSelectedItem(index);
}

//*****************************************************************************
void VuShowroomListEntity::onGameRelease()
{
	VuGameManager::IF()->setCurCar(mItems[mCurItem]->mCar);

	VuGameUtil::IF()->setShowroomCar("");
}

//*****************************************************************************
VuRetVal VuShowroomListEntity::Prev(const VuParams &params)
{
	int newIndex = int((mCurItem - 1 + mItems.size())%mItems.size());
	setSelectedItem(newIndex);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuShowroomListEntity::Next(const VuParams &params)
{
	int newIndex = (mCurItem + 1)%mItems.size();
	setSelectedItem(newIndex);

	return VuRetVal();
}

//*****************************************************************************
void VuShowroomListEntity::setSelectedItem(int index)
{
	mCurItem = index;

	VuGameUtil::IF()->setShowroomCar(mItems[mCurItem]->mCar);

	mItems[index]->mpScriptComponent->getPlug("OnSelected")->execute();
}

//*****************************************************************************
VuShowroomListItemEntity::VuShowroomListItemEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuJsonStringArrayEnumProperty("Car", mCar, VuGameUtil::IF()->constantDB()["Names"]["Cars"]));

	// scripting
	mpNextItemRef = ADD_SCRIPT_REF(mpScriptComponent, NextItem, VuShowroomListItemEntity::msRTTI);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSelected);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnPurchased);
}
