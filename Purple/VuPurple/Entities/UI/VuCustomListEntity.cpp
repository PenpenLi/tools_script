//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Custom List Entity
// 
//*****************************************************************************

#include "VuHListEntity.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Assets/VuTextureAsset.h"

class VuCustomListItemEntity;


class VuCustomListEntity : public VuHListEntity
{
	DECLARE_RTTI

public:
	VuCustomListEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	virtual int					getItemCount();
	virtual void				getItemName(int index, std::string &itemName);
	virtual void				getItemImage(int index, std::string &itemImage);
	virtual bool				isItemLocked(int index);
	virtual void				getUnlockText(int index, std::string &text) {}
	virtual void				onItemSelected(int index);
	virtual void				onItemChosen(int index);
	virtual void				onItemLocked(int inex) {}
	virtual void				onDrawItem(int index, const VuVector2 &offset, float scale) {}

	VuCustomListItemEntity		*getListItem(int index);

	// properties
	std::string					mDataName;

	VuScriptRef					*mpFirstItemRef;
};

IMPLEMENT_RTTI(VuCustomListEntity, VuHListEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCustomListEntity);


//*****************************************************************************
class VuCustomListItemEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCustomListItemEntity();

	VuRetVal			SetName(const VuParams &params);

	bool				isAvailable();
	static bool			isAvailable(const std::string &filter, const char *value);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mName;
	std::string			mImage;
	bool				mDefault;
	bool				mLockForDemo;
	std::string			mSkuFilter;
	std::string			mPlatformFilter;

	VuScriptRef			*mpNextItemRef;
};

IMPLEMENT_RTTI(VuCustomListItemEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCustomListItemEntity);


//*****************************************************************************
VuCustomListEntity::VuCustomListEntity()
{
	// properties
	addProperty(new VuStringProperty("Data Name", mDataName));

	// scripting
	mpFirstItemRef = ADD_SCRIPT_REF(mpScriptComponent, FirstItem, VuCustomListItemEntity::msRTTI);
}

//*****************************************************************************
void VuCustomListEntity::onGameInitialize()
{
	VuHListEntity::onGameInitialize();

	int index = 0;

	// set default item (if any)
	for ( int i = 0; i < getItemCount(); i++ )
		if ( getListItem(i)->mDefault )
			index = i;

	// profile data
	if ( !mDataName.empty() )
	{
		VuProfileManager::IF()->dataRead()["Lists"]["Custom"][mDataName].getValue(index);

		float scrollPos = VuProfileManager::IF()->dataRead()["Lists"]["Custom"][mDataName + "ScrollPos"].asFloat();
		scrollPos = VuClamp(scrollPos, calcScrollMin(), calcScrollMax());
		mScrollPos = mScrollTargetPos = scrollPos;
	}

	setSelectedItem(VuMin(index, getItemCount() - 1), true);
}

//*****************************************************************************
void VuCustomListEntity::onGameRelease()
{
	if ( !mDataName.empty() )
	{
		VuProfileManager::IF()->dataWrite()["Lists"]["Custom"][mDataName].putValue(getSelectedItem());
		VuProfileManager::IF()->dataWrite()["Lists"]["Custom"][mDataName + "ScrollPos"].putValue(mScrollTargetPos);
	}

	VuHListEntity::onGameRelease();
}

//*****************************************************************************
int VuCustomListEntity::getItemCount()
{
	int count = 0;

	VuCustomListItemEntity *pItem = mpFirstItemRef->getRefEntity<VuCustomListItemEntity>();
	while ( pItem )
	{
		if ( pItem->isAvailable() )
			count++;

		pItem = pItem->mpNextItemRef->getRefEntity<VuCustomListItemEntity>();
	}

	return count;
}

//*****************************************************************************
void VuCustomListEntity::getItemName(int index, std::string &itemName)
{
	if ( VuCustomListItemEntity *pItem = getListItem(index) )
		itemName = pItem->mName;
}

//*****************************************************************************
void VuCustomListEntity::getItemImage(int index, std::string &itemImage)
{
	if ( VuCustomListItemEntity *pItem = getListItem(index) )
		itemImage = pItem->mImage;
}

//*****************************************************************************
bool VuCustomListEntity::isItemLocked(int index)
{
	if ( VuCustomListItemEntity *pItem = getListItem(index) )
		if ( pItem->mLockForDemo && VuGameUtil::IF()->isDemoMode() )
			return true;

	return false;
}

//*****************************************************************************
void VuCustomListEntity::onItemSelected(int index)
{
	if ( VuCustomListItemEntity *pItem = getListItem(index) )
		pItem->mpScriptComponent->getPlug("ItemSelected")->execute();
}

//*****************************************************************************
void VuCustomListEntity::onItemChosen(int index)
{
	if ( VuCustomListItemEntity *pItem = getListItem(index) )
		pItem->mpScriptComponent->getPlug("ItemChosen")->execute();
}

//*****************************************************************************
VuCustomListItemEntity *VuCustomListEntity::getListItem(int index)
{
	int count = 0;

	VuCustomListItemEntity *pItem = mpFirstItemRef->getRefEntity<VuCustomListItemEntity>();
	while ( pItem )
	{
		if ( pItem->isAvailable() )
		{
			if ( count == index )
				break;

			count++;
		}

		pItem = pItem->mpNextItemRef->getRefEntity<VuCustomListItemEntity>();
	}

	return pItem;
}

//*****************************************************************************
VuCustomListItemEntity::VuCustomListItemEntity():
	mDefault(false),
	mLockForDemo(false)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 125));

	// properties
	addProperty(new VuStringProperty("Name", mName));
	addProperty(new VuAssetNameProperty(VuTextureAsset::msRTTI.mstrType, "Texture Asset", mImage));
	addProperty(new VuBoolProperty("Default", mDefault));
	addProperty(new VuBoolProperty("Lock for Demo", mLockForDemo));
	addProperty(new VuStringProperty("SKU Filter", mSkuFilter));
	addProperty(new VuStringProperty("Platform Filter", mPlatformFilter));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCustomListItemEntity, SetName, VuRetVal::Void, VuParamDecl(1, VuParams::String));
	mpNextItemRef = ADD_SCRIPT_REF(mpScriptComponent, NextItem, VuCustomListItemEntity::msRTTI);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ItemSelected);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ItemChosen);
}

//*****************************************************************************
VuRetVal VuCustomListItemEntity::SetName(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mName = accessor.getString();

	return VuRetVal();
}

//*****************************************************************************
bool VuCustomListItemEntity::isAvailable()
{
	if ( !isAvailable(mSkuFilter, VuAssetFactory::IF()->getSku().c_str()) )
		return false;

	if ( !isAvailable(mPlatformFilter, VUPLATFORM) )
		return false;

	return true;
}

//*****************************************************************************
bool VuCustomListItemEntity::isAvailable(const std::string &filter, const char *value)
{
	char str[256];
	VU_STRNCPY(str, sizeof(str), filter.c_str(), 255);
	str[255] = '\0';

	char *nextToken;
	const char *p = VU_STRTOK(str, ";", &nextToken);
	while ( p )
	{
		if ( strcmp(p, value) == 0 )
			return false;
		p = VU_STRTOK(VUNULL, ";", &nextToken);
	}

	return true;
}
