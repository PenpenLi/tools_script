//
//  LevelBook.cpp
//  HiFly
//
//  Created by chris li on 14-5-15.
//
//

#include "LevelBook.h"


USING_NS_CC;

static LevelBook* g_LevelBook = nullptr;

LevelBook* LevelBook::getInstance()
{
    if (nullptr == g_LevelBook)
    {
        g_LevelBook = new LevelBook();
    }
    
    return g_LevelBook;
}

std::string LevelBook::getLevelConfig(int level)
{
    std::string levelStr = "";
    
    std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("LEVEL_BOOK.json");
    
    rapidjson::Document d;
    
    if ("" != json)
    {
        d.Parse<0>(json.c_str());
    }
    
    //CCASSERT(d.IsObject() == true, "JsonUtil load config file err");
    
    if (false == d.IsObject())
    {
        return levelStr;
    }
    
    //const char *key = (std::to_string(level)).c_str();
	std::string keyStr = StringUtils::format("%d", level);

	const char *key = keyStr.c_str();

    levelStr = d[key].GetString();
    
    // add "-" at end for parseing easily
    levelStr += "-";
    
    return levelStr;
}

