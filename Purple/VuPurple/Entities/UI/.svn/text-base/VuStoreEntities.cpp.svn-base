//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  store entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuBillingManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuStringProperty.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuRequestOrderManager.h"
#include "VuPurple/Util/CommonUtility.h"
/*
#if defined VUANDROID
#include "VuPurple/Managers/Android/VuAndroidBillingManager.h"
#endif*/


class VuStoreItemProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);
	void setItemName(std::string itemName) { m_itemName = itemName; }
protected:
	std::string m_itemName;
};

void VuStoreItemProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	if (pMessageBox->getResult() != "OnA")
	{
		if (VuBillingManager::IF()->getCurrentBillingType() == VuBillingManager::WEIXIN)
		{
			VuRequestOrderManager::IF()->setDelegate(VuBillingManager::IF());
			VuRequestOrderManager::IF()->startGetOrderId(m_itemName);
		}
		else
		{
			VuBillingManager::IF()->startPurchase(m_itemName);
		}
	}
	
}


//*****************************************************************************
// store item
//*****************************************************************************

class VuStoreItemEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuStoreItemEntity();
	~VuStoreItemEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// scripting
	VuRetVal			StartPurchase(const VuParams &params);
	VuRetVal			IsOwned(const VuParams &params);

	// event handlers
	void				OnPurchaseMade(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mItemName;
	VuStoreItemProcessor* m_processor;
};

IMPLEMENT_RTTI(VuStoreItemEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuStoreItemEntity);


//*****************************************************************************
VuStoreItemEntity::VuStoreItemEntity()
{
	m_processor = new VuStoreItemProcessor;
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuStringProperty("Item Name", mItemName));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuStoreItemEntity, StartPurchase);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuStoreItemEntity, IsOwned, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}
//*****************************************************************************
VuStoreItemEntity::~VuStoreItemEntity()
{
	if (m_processor)
	{
		delete m_processor;
		m_processor = NULL;
	}
}
//*****************************************************************************
void VuStoreItemEntity::onGameInitialize()
{
	// event handlers
	REG_EVENT_HANDLER(VuStoreItemEntity, OnPurchaseMade);
}

//*****************************************************************************
void VuStoreItemEntity::onGameRelease()
{
	mEventMap.unregisterHandlers();
}

//*****************************************************************************
VuRetVal VuStoreItemEntity::StartPurchase(const VuParams &params)
{
	//zouchao
	if (!VuBillingManager::IF()->isBusy())
	{
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mItemName);
		std::string strNeedMoney = itemData["StubPrice"].asString();
		float needMoney = VuStringUtil::readFloat(strNeedMoney.c_str());
		VuBillingManager::IF()->startPurchaseInternal(mItemName, needMoney);
	}
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuStoreItemEntity::IsOwned(const VuParams &params)
{
	bool isOwned = VuBillingManager::IF()->isOwned(mItemName.c_str());

	return VuRetVal(isOwned);
}

//*****************************************************************************
void VuStoreItemEntity::OnPurchaseMade(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	if ( mItemName.compare(itemName) == 0 )
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
	}
}





