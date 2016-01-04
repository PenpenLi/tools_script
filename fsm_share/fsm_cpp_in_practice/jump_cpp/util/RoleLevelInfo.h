//
//  RoleLevelInfo.h
//  HiFly
//
//  Created by chris li on 14-4-22.
//
//

#ifndef __HiFly__RoleLevelInfo__
#define __HiFly__RoleLevelInfo__
#include "cocos2d.h"
#include "JsonConfigUtil.h"

class RoleLevelInfo
{
public:
    static RoleLevelInfo* getInstance();

	int getLvUpCost(std::string level);

	int getUpgradeCost(std::string level);

	float getCoinExtra(std::string level);

	float getScoreExtra(std::string level);

	std::string getEffect(std::string level);

	float getMagnetTime(std::string level);

	float getShipTime(std::string level);

	float getHelicopterTime(std::string level);

	float getShieldTime(std::string level);

	std::string getRoleIntroduce(int roleId);
    
private:
    RoleLevelInfo() {}
    virtual ~RoleLevelInfo() {}
    void init();

//
	rapidjson::Document m_document;
};

#endif /* defined(__HiFly__RoleLevelInfo__) */
