//
//  VIPBook.cpp
//  HiFly
//
//  Created by chris li on 14-4-22.
//
//

#include "VIPBook.h"
#include "JsonConfigUtil.h"

USING_NS_CC;

static VIPBook* g_VIPBook = nullptr;

VIPBook* VIPBook::getInstance()
{
    if (nullptr == g_VIPBook)
    {
        g_VIPBook = new VIPBook();
        g_VIPBook->init();
    }
    
    return g_VIPBook;
}

void VIPBook::init()
{
	rapidjson::Document* p = JsonConfigUtil::loadConfig("VIP_BOOK.json");
	if ((*p).IsObject()) {
        log("1:%s", (*p)["1"].GetString());
        log("2:%s", (*p)["2"].GetString());
    }
}

////**************** josn code ref ******************////
/*
 //assert(d.IsObject());
 if (!d.IsObject()) {return;}
 
 //printf("result: %d\n", d["result"].GetInt());
 int result = d["result"].GetInt();
 // result ok
 if (result == 0) {
 Value &data = d["data"];
 assert(data.IsObject());
 
 string nickname = data["nickname"].GetString();
 printf("nickname: %s\n", nickname.c_str());
 GameData::getInstance()->nickname2 = nickname;
 
 string sex = data["sex"].GetString();
 printf("sex: %s\n", sex.c_str());
 GameData::getInstance()->sex2 = sex;
 
 int score = data["score"].GetInt();
 printf("score: %d\n", score);
 GameData::getInstance()->score2 = score;
 
 int diamond = data["diamond"].GetInt();
 printf("diamond: %d\n", diamond);
 GameData::getInstance()->diamond2 = diamond;
 
 int coin = data["coin"].GetInt();
 printf("coin: %d\n", coin);
 GameData::getInstance()->coin2 = coin;
 
 int heart = data["heart"].GetInt();
 printf("heart: %d\n", heart);
 GameData::getInstance()->heart2 = heart;
 
 int rank = data["rank"].GetInt();
 printf("rank: %d\n", rank);
 GameData::getInstance()->rank2 = rank;
 
 int level = data["level"].GetInt();
 printf("level:%d", level);
 GameData::getInstance()->level2 = level;
 
 int time = data["time"].GetInt();
 printf("time: %d\n", time);
 GameData::getInstance()->time2 = time;
 
 int getRewardDayCount2 = data["login_day_count"].GetInt();
 printf("getRewardDayCount2: %d\n", getRewardDayCount2);
 GameData::getInstance()->getRewardDayCount2 = getRewardDayCount2;
 
 if (data["notice"].IsString()) {
 std::string notice = data["notice"].GetString();
 printf("notice: %s\n", notice.c_str());
 GameData::getInstance()->notice2 = data["notice"].GetString();
 } else {
 GameData::getInstance()->notice2 = "";
 }
 GameData::getInstance()->_loginOk = true;
 }
 log("jsonStr:%s", jsonStr.c_str());
 */