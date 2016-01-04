//
//  BattleData.cpp
//  HiFly
//
//  Created by chris li on 14-6-16.
//
//

#include "BattleData.h"

static BattleData* g_BattleData = nullptr;

BattleData* BattleData::getInstance()
{
    if (nullptr == g_BattleData)
    {
        g_BattleData = new BattleData();
        
        g_BattleData->reset();
    }
    
    return g_BattleData;
}

void BattleData::reset()
{
    setGold(0);
    
    setScore(0);
    
    setHp(0);
    
    setStar(0);
    
    setProp1(0);
    
    setProp2(0);
    
    setProp3(0);
    
    setProp4(0);
    
    setHelperPlayerId(0);

	setMonsterScore(0);

	setEffectRandomProp(0);
	setEffectRole(0);

	setGoldAddActivity(0);
	setGoldAddRandomProp(0);
	setGoldAddRole(0);

	setPropRandom(0);
	setRoleScore(0);
	setRoleLevel(0);

	setScoreAddPercent(0);
	setGoldAddPercent(0);

	setMonsterKilledNum(0);
	setIsBreak(false);

	setIsRealDie(false);
    
    setReviveTimes(0);
}

void BattleData::addGold(int gold)
{
    m_gold += gold;
}

void BattleData::addScore(int score)
{
    m_score += score;
}

void BattleData::addHp(int hp)
{
    m_hp += hp;
}

void BattleData::addStar(int star)
{
    m_star += star;
}

void BattleData::addProp1(int prop1)
{
    m_prop1 += prop1;
}

void BattleData::addProp2(int prop2)
{
    m_prop2 += prop2;
}

void BattleData::addProp3(int prop3)
{
    m_prop3 += prop3;
}

void BattleData::addProp4(int prop4)
{
    m_prop4 += prop4;
}

void BattleData::addMonsterScore(int score)
{
	m_monsterScore += score;
}

void BattleData::addMonsterKilledNum(int num)
{
	m_monsterKilledNum += num;
}

