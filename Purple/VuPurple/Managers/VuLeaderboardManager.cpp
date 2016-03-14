//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuLeaderboardManager
// 
//*****************************************************************************

#include "VuLeaderboardManager.h"
#include "VuGameServicesManager.h"
#include "VuEngine/Objects/VuRefObj.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"


#define BASE_URL "https://vectorunit-bbr.appspot.com/leaderboards"
//#define BASE_URL "http://localhost:8080/leaderboards"

#define CACHE_TIME 5*60.0f // seconds


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuLeaderboardManager, VuLeaderboardManager);


class VuLeaderboardManager::VuQuery : public VuRefObj
{
public:
	VuQuery() : mStatus(RETRIEVE_PENDING), mAge(0.0f) {}
	std::string	mLeaderboardName;
	eStatus		mStatus;
	float		mAge;
	Scores		mScores;
};


//*****************************************************************************
VuLeaderboardManager::VuLeaderboardManager():
	mpActiveQuery(VUNULL),
	mHttpRequest(VUNULL)
{
}

//*****************************************************************************
bool VuLeaderboardManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuLeaderboardManager::tickNetwork, "Network");

	return true;
}

//*****************************************************************************
void VuLeaderboardManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);

	VU_SAFE_RELEASE(mpActiveQuery);

	for ( auto pQuery: mQueries )
		pQuery->removeRef();
	mQueries.clear();
}

//*****************************************************************************
void VuLeaderboardManager::submitScore(const std::string &lbName, VUINT64 value)
{
	if ( lbName.length() && VuGameServicesManager::IF()->isSignedIn() )
	{
		// create http request
		VUHANDLE httpRequest = VuHttpClient::IF()->createRequest();

		// URL
		char url[256] = BASE_URL"/submit";

		// data
		VuJsonContainer data;
		data["AuthType"].putValue(VuGameServicesManager::IF()->getAuthType());
		data["AuthID"].putValue(VuGameServicesManager::IF()->getGamerID());
		data["Leaderboard"].putValue(lbName);
		data["Value"].putValue(value);

		std::string strData;
		VuJsonWriter writer;
		writer.configCompact(true);
		writer.saveToString(data, strData);

		// header
		VuHttpClient::IF()->setContentHeader(httpRequest, "Content-Type", "application/json");
		VuHttpClient::IF()->setContentHeader(httpRequest, "Content-Length", (int)strData.size());

		// send request
		VuHttpClient::IF()->postAsync(httpRequest, url, strData);
		VUPRINTF("Submitting leaderboard score: %s (%lld)\n", lbName.c_str(), value);

		VuHttpClient::IF()->releaseRequest(httpRequest);

		// expire any cached queries for this leaderboard
		clearCache(lbName);
	}
}

//*****************************************************************************
void VuLeaderboardManager::clearCache(const std::string &lbName)
{
	for ( auto pQuery: mQueries )
		if ( pQuery->mLeaderboardName == lbName )
			pQuery->mAge = CACHE_TIME;
}

//*****************************************************************************
VUHANDLE VuLeaderboardManager::createQuery(const std::string &lbName)
{
	// check existing queries to see if we can find a match
	for ( auto pQuery: mQueries )
	{
		if ( (pQuery->mStatus == RETRIEVE_SUCCESS || pQuery->mStatus == RETRIEVE_PENDING) && (pQuery->mAge < CACHE_TIME) )
		{
			if ( pQuery->mLeaderboardName == lbName )
			{
				pQuery->addRef();
				return pQuery;
			}
		}
	}

	// create new query
	VuQuery *pQuery = new VuQuery;

	pQuery->mLeaderboardName = lbName;
	pQuery->addRef();

	mQueries.push_back(pQuery);

	return pQuery;
}

//*****************************************************************************
void VuLeaderboardManager::releaseQuery(VUHANDLE hQuery)
{
	VuQuery *pQuery = static_cast<VuQuery *>(hQuery);
	pQuery->removeRef();
}

//*****************************************************************************
VuLeaderboardManager::eStatus VuLeaderboardManager::getStatus(VUHANDLE hQuery)
{
	VuQuery *pQuery = static_cast<VuQuery *>(hQuery);
	return pQuery->mStatus;
}

//*****************************************************************************
const VuLeaderboardManager::Scores &VuLeaderboardManager::getScores(VUHANDLE hQuery)
{
	VuQuery *pQuery = static_cast<VuQuery *>(hQuery);
	return pQuery->mScores;
}

//*****************************************************************************
void VuLeaderboardManager::tickNetwork(float fdt)
{
	// use actual real time
	fdt = VuTickManager::IF()->getUnclampedRealDeltaTime();

	// update queries
	for ( auto pQuery : mQueries )
		pQuery->mAge += fdt;

	// destroy dangling queries
	for ( Queries::iterator iter = mQueries.begin(); iter != mQueries.end(); iter++ )
	{
		VuQuery *pQuery = *iter;
		if ( (pQuery->refCount() == 1) && (pQuery->mAge >= CACHE_TIME) )
		{
			pQuery->removeRef();
			mQueries.erase(iter);
			break;
		}
	}

	// update active query
	if ( mpActiveQuery )
	{
		// read http response
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if ( status != VuHttpClient::STATUS_WAITING_FOR_RESPONSE )
		{
			if ( status == VuHttpClient::STATUS_RESPONSE_RECEIVED )
			{
				VuJsonContainer response;
				VuJsonReader jsonReader;
				if ( jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)) )
				{
					mpActiveQuery->mScores.reserve(response.size());
					for ( int iRow = 0; iRow < response.size(); iRow++ )
					{
						const std::string &gamerID = response[iRow]["AuthID"].asString();

						// determine gamer tag
						const char *gamerTag = VUNULL;
						if ( gamerID == VuGameServicesManager::IF()->getGamerID() )
							gamerTag = VuGameServicesManager::IF()->getGamerTag().c_str();
						else if ( const VuGameServicesManager::FriendInfo *pFriendInfo = VuGameServicesManager::IF()->getFriendInfo(gamerID.c_str()) )
							gamerTag = pFriendInfo->mGamerTag.c_str();

						if ( gamerTag )
						{
							mpActiveQuery->mScores.resize(mpActiveQuery->mScores.size() + 1);
							VuScore &score = mpActiveQuery->mScores.back();

							score.mRank = response[iRow]["Rank"].asInt();
							score.mValue = response[iRow]["Value"].asInt64();
							score.mGamerID = gamerID;
							score.mGamerTag = gamerTag;
						}
					}

					mpActiveQuery->mStatus = RETRIEVE_SUCCESS;
					VUPRINTF("Received leaderboard scores: %s\n", mpActiveQuery->mLeaderboardName.c_str());
				}
				else
				{
					mpActiveQuery->mStatus = RETRIEVE_FAILURE;
					VUPRINTF("Leaderboard read error: %s\n", mpActiveQuery->mLeaderboardName.c_str());
				}
			}
			else
			{
				mpActiveQuery->mStatus = RETRIEVE_FAILURE;
				VUPRINTF("Leaderboard retrieve failure: %s\n", mpActiveQuery->mLeaderboardName.c_str());
			}

			mpActiveQuery->removeRef();
			mpActiveQuery = VUNULL;

			VuHttpClient::IF()->releaseRequest(mHttpRequest);
		}
	}

	// activate next query
	if ( mpActiveQuery == VUNULL )
	{
		auto result = std::find_if(mQueries.begin(), mQueries.end(), [](VuQuery *pQuery) { return pQuery->mStatus == RETRIEVE_PENDING; } );
		if ( result != mQueries.end() )
		{
			VuQuery *pQuery = *result;

			if ( VuGameServicesManager::IF()->isSignedIn() )
			{
				mpActiveQuery = pQuery;
				mpActiveQuery->addRef();

				// create http request
				mHttpRequest = VuHttpClient::IF()->createRequest();

				// URL
				char url[256] = BASE_URL"/retrieve";

				// data
				VuJsonContainer data;
				data["Leaderboard"].putValue(mpActiveQuery->mLeaderboardName);
				data["AuthType"].putValue(VuGameServicesManager::IF()->getAuthType());

				data["AuthIDs"].append().putValue(VuGameServicesManager::IF()->getGamerID());
				for ( int iFriend = 0; iFriend < VuGameServicesManager::IF()->getFriendCount(); iFriend++ )
					data["AuthIDs"].append().putValue(VuGameServicesManager::IF()->getFriendInfo(iFriend).mGamerID);

				std::string strData;
				VuJsonWriter writer;
				writer.configCompact(true);
				writer.saveToString(data, strData);

				// header
				VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
				VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());

				// send request
				VuHttpClient::IF()->postAsync(mHttpRequest, url, strData);
				VUPRINTF("Retrieving leaderboard scores: %s\n", mpActiveQuery->mLeaderboardName.c_str());
			}
			else
			{
				pQuery->mStatus = RETRIEVE_FAILURE;
			}
		}
	}
}
