//
//  LevelBook.h
//  HiFly
//
//  Created by chris li on 14-5-15.
//
//

#ifndef __HiFly__LevelBook__
#define __HiFly__LevelBook__

#include "JsonConfigUtil.h"

class LevelBook
{
public:
    static LevelBook* getInstance();
    
    std::string getLevelConfig(int level);
    
private:
    LevelBook() {}
    
    virtual ~LevelBook() {}
    
    rapidjson::Document* m_levelConfig;
};

#endif /* defined(__HiFly__LevelBook__) */
