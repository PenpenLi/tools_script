//
//  JsonUtil.cpp
//  HiFly
//
//  Created by chris li on 14-4-23.
//
//

#include "JsonConfigUtil.h"

rapidjson::Document* JsonConfigUtil::loadConfig(std::string jsonFile)
{
    std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile(jsonFile);
    
    rapidjson::Document d;
    
    if ("" != json)
    {
        d.Parse<0>(json.c_str());
    }
    
    CCASSERT(d.IsObject() == true, "JsonUtil load config file err");

	rapidjson::Document* p = &d;
    
    return p;
}
