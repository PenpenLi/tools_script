//
//  BattleData.h
//  HiFly
//
//  Created by chris li on 14-6-16.
//
//

#ifndef __HiFly__BattleData__
#define __HiFly__BattleData__

#include "cocos2d.h"
/**
 * shared by all layers in battle scene
 */

class BattleData
{
public:
    static BattleData* getInstance();
    
    void reset();
    
    CC_SYNTHESIZE(int, m_battleLevel, BattleLevel);
    
    CC_SYNTHESIZE(int, m_battleMode, BattleMode); // 0 infinite, 1 finite
    
    // config
    CC_SYNTHESIZE(int, m_score, Score);
    void addScore(int score);
    
    CC_SYNTHESIZE(int, m_gold, Gold);
    void addGold(int gold);
    
    CC_SYNTHESIZE(int, m_hp, Hp);
    void addHp(int hp);
    
    CC_SYNTHESIZE(int, m_star, Star);
    void addStar(int star);
    
    CC_SYNTHESIZE(int, m_prop1, Prop1);
    void addProp1(int prop1);
    
    CC_SYNTHESIZE(int, m_prop2, Prop2);
    void addProp2(int prop2);
    
    CC_SYNTHESIZE(int, m_prop3, Prop3);
    void addProp3(int prop3);
    
    CC_SYNTHESIZE(int, m_prop4, Prop4);
    void addProp4(int prop4);

	CC_SYNTHESIZE(bool, m_isbreak, IsBreak);

	CC_SYNTHESIZE(float, m_roleScore, RoleScore);

	CC_SYNTHESIZE(float, m_effectRandomProp, EffectRandomProp);

	CC_SYNTHESIZE(float, m_effectRole, EffectRole);

	CC_SYNTHESIZE(float, m_goldAddRandomProp, GoldAddRandomProp);

	CC_SYNTHESIZE(float, m_goldAddRole, GoldAddRole);

    CC_SYNTHESIZE(float, m_goldAddActivity, GoldAddActivity);

	CC_SYNTHESIZE(float, m_goldAddPercent, GoldAddPercent);

	CC_SYNTHESIZE(float, m_scoreAddPercent, ScoreAddPercent);

	CC_SYNTHESIZE(int, m_PropRandom, PropRandom);

    CC_SYNTHESIZE(int, m_helperPlayerId, HelperPlayerId);

	CC_SYNTHESIZE(int, m_roleLevel, RoleLevel);
    
    CC_SYNTHESIZE(int, m_roleType, RoleType);

	CC_SYNTHESIZE(int, m_monsterScore, MonsterScore);
    void addMonsterScore(int score);

	CC_SYNTHESIZE(int, m_monsterKilledNum, MonsterKilledNum);
    void addMonsterKilledNum(int num);

	CC_SYNTHESIZE(int, m_playTime, PlayTime);

	// 1 : win 2 : loss
	CC_SYNTHESIZE(int, m_gameWin, GameWin);

	//Real Die
	CC_SYNTHESIZE(bool, m_isRealDie, IsRealDie);
    
    //revivie times
    CC_SYNTHESIZE(int, m_reviveTimes, ReviveTimes);

};

#endif /* defined(__HiFly__BattleData__) */
