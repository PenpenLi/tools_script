//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Windows Billing manager
//
//*****************************************************************************

#include <ppltasks.h>
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Util/VuUtf8.h"


#define STORE_ITEM_KEY "WindowsID"


using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Store;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;


class VuWindowsBillingManager : public VuBillingManager
{
public:
	virtual bool		init();

	virtual void		startPurchaseInternal(const std::string &itemName);

protected:
	void			fulfillProduct(Platform::String ^productId, Platform::Guid transactionId);
	static bool		getItemName(const std::string &productID, std::string &itemName);
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuBillingManager, VuWindowsBillingManager);


//*****************************************************************************
bool VuWindowsBillingManager::init()
{
	if ( !VuBillingManager::init() )
		return false;

#ifdef VURETAIL
	create_task(CurrentApp::LoadListingInformationAsync()).then([this](task<ListingInformation^> currentTask)
	{
#else
    create_task(Package::Current->InstalledLocation->GetFileAsync("WindowsStoreProxy.xml")).then([this](StorageFile^ proxyFile)
    {
        create_task(CurrentAppSimulator::ReloadSimulatorAsync(proxyFile)).then([this]()
        {
			create_task(CurrentAppSimulator::LoadListingInformationAsync()).then([this](task<ListingInformation^> currentTask)
			{
#endif
				try
				{
					ListingInformation^ listing = currentTask.get();
					for (IIterator<IKeyValuePair<Platform::String^, ProductListing^>^> ^iter = listing->ProductListings->First(); iter->HasCurrent; iter->MoveNext())
					{
						std::string productId, price;
						VuUtf8::convertWCharStringToUtf8String(iter->Current->Value->ProductId->Data(), productId);
						VuUtf8::convertWCharStringToUtf8String(iter->Current->Value->FormattedPrice->Data(), price);

						std::string itemName;
						getItemName(productId, itemName);

						mPrices[itemName] = price;
					}
				}
				catch(Platform::Exception^ exception)
				{
					VUPRINTF("LoadListingInformationAsync API call failed!\n");
				}
#ifdef VURETAIL
	});
#else
			});
		});
	});
#endif

	return true;
}

//*****************************************************************************
void VuWindowsBillingManager::startPurchaseInternal(const std::string &itemName)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
	std::wstring strProductId;
	VuUtf8::convertUtf8StringToWCharString(itemData[STORE_ITEM_KEY].asCString(), strProductId);
	Platform::String ^productId = ref new Platform::String(strProductId.c_str());

#ifdef VURETAIL
	create_task(CurrentApp::RequestProductPurchaseAsync(productId)).then([this, itemName, productId](task<PurchaseResults^> currentTask)
#else
	create_task(CurrentAppSimulator::RequestProductPurchaseAsync(productId)).then([this, itemName, productId](task<PurchaseResults^> currentTask)
#endif
	{
		try
		{
			PurchaseResults^ results = currentTask.get();
			if (results->Status == ProductPurchaseStatus::Succeeded)
			{
				onPurchaseResult(itemName, "RESULT_OK");
				fulfillProduct(productId, results->TransactionId);
			}
			else if (results->Status == ProductPurchaseStatus::NotFulfilled)
			{
				fulfillProduct(productId, results->TransactionId);
			}
			else
			{
				onPurchaseResult(itemName, "RESULT_USER_CANCELED");
			}
		}
		catch (Platform::Exception^ exception)
		{
			onPurchaseResult(itemName, "RESULT_ERROR");
		}
	});
}

//*****************************************************************************
void VuWindowsBillingManager::fulfillProduct(Platform::String ^productId, Platform::Guid transactionId)
{
#ifdef VURETAIL
	create_task(CurrentApp::ReportConsumableFulfillmentAsync(productId, transactionId)).then([this](task<FulfillmentResult> currentTask)
#else
	create_task(CurrentAppSimulator::ReportConsumableFulfillmentAsync(productId, transactionId)).then([this](task<FulfillmentResult> currentTask)
#endif
	{
		try
		{
			FulfillmentResult result = currentTask.get();
			if (result == FulfillmentResult::Succeeded)
			{
				VUPRINTF("Windows Billing: FulfillmentResult::Succeeded\n");
			}
			else if (result == FulfillmentResult::NothingToFulfill)
			{
				VUPRINTF("Windows Billing: FulfillmentResult::NothingToFulfill\n");
			}
			else if (result == FulfillmentResult::PurchasePending)
			{
				VUPRINTF("Windows Billing: FulfillmentResult::PurchasePending\n");
			}
			else if (result == FulfillmentResult::PurchaseReverted)
			{
				VUPRINTF("Windows Billing: FulfillmentResult::PurchaseReverted\n");
			}
			else if (result == FulfillmentResult::ServerError)
			{
				VUPRINTF("Windows Billing: FulfillmentResult::ServerError\n");
			}
		}
		catch (Platform::Exception^ exception)
		{
			VUPRINTF("Windows Billing: Fulfillment exception\n");
		}
	});
}

//*****************************************************************************
bool VuWindowsBillingManager::getItemName(const std::string &productID, std::string &itemName)
{
	const VuJsonContainer &items = VuGameUtil::IF()->storeDB();
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i][STORE_ITEM_KEY].asString() == productID)
		{
			itemName = items[i]["Name"].asString();
			return true;
		}
	}

	return false;
}
