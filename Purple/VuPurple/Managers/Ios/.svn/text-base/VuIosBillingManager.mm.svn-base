//*****************************************************************************
//
//  Copyright (c) 2012-2014 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Ios Billing manager
//
//*****************************************************************************

#import <StoreKit/StoreKit.h>
#import <FuseAPI.h>

#include "VuIosBillingManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Util/VuHash.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuBillingManager, VuIosBillingManager);

// obj-c interfaces
@interface VuBillingObserver : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
}
+ (VuBillingObserver *)instance;
@end



//*****************************************************************************
VuIosBillingManager::VuIosBillingManager():
	mVerifyHttpRequest(VUNULL)
{
}

//*****************************************************************************
bool VuIosBillingManager::init()
{
	if ( !VuBillingManager::init() )
		return false;
	
	// get product info
	const VuJsonContainer &store = VuGameUtil::IF()->storeDB();
	NSMutableSet *productIds = [[NSMutableSet alloc] initWithCapacity:store.size()];
	for ( int i = 0; i < store.size(); i++ )
	{
		const VuJsonContainer &itemData = store[i];
		const std::string &productId = itemData["AppStoreID"].asString();
		[productIds addObject: [NSString stringWithUTF8String:productId.c_str()]];
	}
	
	SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:productIds];
	if ( request )
	{
		request.delegate = [VuBillingObserver instance];
		[request start];
	}
	
	// start ticking
	VuTickManager::IF()->registerHandler(this, &VuIosBillingManager::tick, "Decision");
	
	return true;
}

//*****************************************************************************
void VuIosBillingManager::release()
{
	// stop ticking
	VuTickManager::IF()->unregisterHandlers(this);
	
	VuBillingManager::release();
}

//*****************************************************************************
void VuIosBillingManager::tick(float fdt)
{
	if ( mVerifyHttpRequest )
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mVerifyHttpRequest);
		if ( status != VuHttpClient::STATUS_WAITING_FOR_RESPONSE )
		{
			bool responseReceived = false;
			bool responseIsValid = false;
			
			if ( status == VuHttpClient::STATUS_RESPONSE_RECEIVED )
			{
				VuJsonContainer response;
				VuJsonReader jsonReader;
				if ( jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mVerifyHttpRequest)) )
				{
					if ( response.isNumber() )
					{
						responseReceived = true;
						if (response.asInt() == mVerifyKey )
							responseIsValid = true;
					}
				}
			}
			
			if ( responseReceived && !responseIsValid )
			{
				onPurchaseResult(mVerifyProductName, "RESULT_ERROR");
				VuGameManager::IF()->setFlaggedAsPirate();
			}
			else
			{
				onPurchaseResult(mVerifyProductName, "RESULT_OK"); // err on the side of caution
			}
			
			VuHttpClient::IF()->releaseRequest(mVerifyHttpRequest);
			mVerifyHttpRequest = VUNULL;
		}
	}
}

//*****************************************************************************
void VuIosBillingManager::setProductInfo(const char *id, const char *price)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "AppStoreID", id);
	const std::string &itemName = itemData["Name"].asString();
	
	setPrice(itemName.c_str(), price);
}

//*****************************************************************************
void VuIosBillingManager::verifyPurchase(const char *id, const char *receipt)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "AppStoreID", id);
	
	mVerifyProductName = itemData["Name"].asString();
	mVerifyKey = VuHash::crc32String(receipt);
	
	// create http request
	mVerifyHttpRequest = VuHttpClient::IF()->createRequest();
	
	// URL
	const char *url = "https://vectorunit-cloud.appspot.com/verify/itp";
	
	// data
	VuJsonContainer data;
	data["ReceiptData"].putValue(receipt);
	
	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);
	
	// header
	VuHttpClient::IF()->setContentHeader(mVerifyHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mVerifyHttpRequest, "Content-Length", strData.size());
	
	// timeout
	VuHttpClient::IF()->setRequestTimeoutMS(mVerifyHttpRequest, 5000);
	
	// send request
	VuHttpClient::IF()->postAsync(mVerifyHttpRequest, url, strData);
}

//*****************************************************************************
void VuIosBillingManager::onIosPurchaseResult(const char *id, const char *result)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "AppStoreID", id);
	const std::string &itemName = itemData["Name"].asString();
	
	onPurchaseResult(itemName, result);
}

//*****************************************************************************
void VuIosBillingManager::registerFuseTransaction(const char *transactionId, const char *id)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "AppStoreID", id);
	const std::string &itemName = itemData["Name"].asString();

	mFuseTransactions[transactionId] = itemName;
}

//*****************************************************************************
void VuIosBillingManager::purchaseVerifiedByFuse(const char *transactionId)
{
	FuseTransactions::iterator iter = mFuseTransactions.find(transactionId);
	if ( iter != mFuseTransactions.end() )
	{
		logPurchaseAnalyticsEvent(iter->second.c_str());
		mFuseTransactions.erase(iter);
	}
}

//*****************************************************************************
void VuIosBillingManager::onRestoreCompleted(bool success)
{
	if ( !success )
	{
		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mHeading = "RestorePurchases_MB_Fail_Header";
		messageBoxParams.mBody = "RestorePurchases_MB_Fail_Body";
		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();
	}

	mBusy = false;
}

//*****************************************************************************
void VuIosBillingManager::restoreManagedItems()
{
	if ( !mBusy )
	{
		mBusy = true;

		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mHeading = "RestorePurchases_MB_Start_Header";
		messageBoxParams.mBody = "RestorePurchases_MB_Start_Body";
		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();
		
		[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
	}
}

//*****************************************************************************
void VuIosBillingManager::startPurchaseInternal(const std::string &itemName)
{
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
	const std::string &appStoreID = itemData["AppStoreID"].asString();
	
	if ( [SKPaymentQueue canMakePayments] )
	{
		SKMutablePayment *payment = [[SKMutablePayment alloc] init];
		payment.productIdentifier = [NSString stringWithUTF8String:appStoreID.c_str()];
		payment.quantity = 1;
		[[SKPaymentQueue defaultQueue] addPayment:payment];
	}
	else
	{
		onPurchaseResult(appStoreID.c_str(), "RESULT_BILLING_UNAVAILABLE");
	}
}


@implementation VuBillingObserver

+ (VuBillingObserver *)instance
{
	static VuBillingObserver *manager = NULL;
	@synchronized(self)
	{
		if (manager == NULL)
		{
			manager = [[VuBillingObserver alloc] init];
			[[SKPaymentQueue defaultQueue] addTransactionObserver:manager];
		}
	}
	
	return(manager);
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	for ( SKProduct *product in response.products )
	{
		NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
		[numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
		[numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
		[numberFormatter setLocale:product.priceLocale];
		NSString *formattedString = [numberFormatter stringFromNumber:product.price];
		
		VuIosBillingManager::IF()->setProductInfo([product.productIdentifier UTF8String], [formattedString UTF8String]);
	}
	
	[FuseAPI registerInAppPurchaseList:response];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction *transaction in transactions)
	{
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchased:
			{
				NSString* receipt = [self createEncodedString:transaction.transactionReceipt];
				VuIosBillingManager::IF()->verifyPurchase([transaction.payment.productIdentifier UTF8String], [receipt UTF8String]);
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				
				VuIosBillingManager::IF()->registerFuseTransaction([transaction.transactionIdentifier UTF8String], [transaction.payment.productIdentifier UTF8String]);
				[FuseAPI registerInAppPurchase:transaction];
				break;
			}
			case SKPaymentTransactionStateFailed:
			{
				NSLog(@"%@",[transaction.error localizedDescription]);
				if ( transaction.error.code == SKErrorPaymentCancelled )
					VuIosBillingManager::IF()->onIosPurchaseResult([transaction.payment.productIdentifier UTF8String], "RESULT_USER_CANCELED");
				else
					VuIosBillingManager::IF()->onIosPurchaseResult([transaction.payment.productIdentifier UTF8String], "RESULT_ERROR");
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				break;
			}
			case SKPaymentTransactionStateRestored:
			{
				NSString* receipt = [self createEncodedString:transaction.transactionReceipt];
				VuIosBillingManager::IF()->verifyPurchase([transaction.payment.productIdentifier UTF8String], [receipt UTF8String]);
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				break;
			}
			case SKPaymentTransactionStateDeferred:
			{
				VuIosBillingManager::IF()->onIosPurchaseResult([transaction.payment.productIdentifier UTF8String], "RESULT_DEFERRED");
				break;
			}
			default:
				break;
		}
	}
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
	VuIosBillingManager::IF()->onRestoreCompleted(true);
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
	NSLog(@"%@",[error localizedDescription]);
	VuIosBillingManager::IF()->onRestoreCompleted(false);
}

- (NSString*) createEncodedString:(NSData*)data
{
	static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	const int size = ((data.length + 2)/3)*4;
	uint8_t output[size];
	
	const uint8_t* input = (const uint8_t*)[data bytes];
	for (int i = 0; i < data.length; i += 3)
	{
		int value = 0;
		for (int j = i; j < (i + 3); j++)
		{
			value <<= 8;
			if (j < data.length)
				value |= (0xFF & input[j]);
		}
		
		const int index = (i / 3) * 4;
		output[index + 0] =  table[(value >> 18) & 0x3F];
		output[index + 1] =  table[(value >> 12) & 0x3F];
		output[index + 2] = (i + 1) < data.length ? table[(value >> 6)  & 0x3F] : '=';
		output[index + 3] = (i + 2) < data.length ? table[(value >> 0)  & 0x3F] : '=';
	}
	
	return  [[NSString alloc] initWithBytes:output length:size encoding:NSASCIIStringEncoding];
}

@end

