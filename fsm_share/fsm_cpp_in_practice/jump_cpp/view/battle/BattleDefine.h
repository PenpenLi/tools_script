//
//  BattleDefine.h
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#ifndef HiFly_BattleDefine_h
#define HiFly_BattleDefine_h

#define EVENT_HERO_NTF              "event_hero_ntf"
#define EVENT_JUMP_READY_OVER       "event_jump_ready_over"
#define EVENT_JUMP_SUSPEND_OVER     "event_jump_suspend_over"
#define EVENT_JUMP_AGAIN1_OVER      "event_jump_again1_over"
#define EVENT_JUMP_DEATH1_OVER      "event_jump_death1_over"
#define EVENT_REWARD_LEVELS_START   "event_reward_levels_start"
#define EVENT_REWARD_LEVELS_END     "event_reward_levels_end"
#define EVENT_BATTLE_LEVELS_END     "event_battle_levels_end"
#define EVENT_ATTACK_START          "event_attack_start"
#define EVENT_ATTACK_END            "event_attack_end"
#define EVENT_INVINCIBLE_START      "event_inbincible_start"
#define EVENT_INVINCIBLE_END        "event_inbincible_end"
#define EVENT_WARNING_END           "event_warning_end"
#define EVENT_MONSTER_TOUCHED       "event_monster_touched"
#define EVENT_VORTEX_TOUCHED       "event_vortex_touched"
#define EVENT_MONSTER_ENABLE_TOUCHED       "event_monster_enable_touched"
#define EVENT_REWARD_MAGNET_START   "event_reward_magnet_start"
#define EVENT_ATTACK_READY   "event_attack_ready"
#define EVENT_LOAD_RES              "event_load_res"

typedef struct
{
    int tag; // 0 is invalid, 1 is valid
    
    int hp;
    
    int score;
    
    int gold;
    //prop1~8 are an array of prop's CD
	//when it is 0, it means dismiss the prop's icon

    int prop1; //PROP_SPRINT_TAG 

    int prop2; //PROP_BUBBLE_TAG 
    
    int prop3; //PROP_JET_TAG  
    
    int prop4; //PROP_DIESPRINT_TAG

	int prop5; //PROP_HELICOPTER_TAG

	int prop6; //PROP_MAGNET_TAG

	int prop7; //PROP_SPUERBUBBLE_TAG

	int prop8; //PROP_MUSHROOM_TAG
    
    int star; //progress bar startNum / enterRewardStar
    
} HeroUIDataNtf;

typedef struct
{
    HeroUIDataNtf heroUIDataNtf;
    
    float scrollDistance;
    
    int magnetState; //0-disable, 1-start, 2-stop
    
    float x; // x position
    
    float y; // y position
    
    int levelType; // 0 - normal, 1 - start reward level, 2 - stop reward level
    
} HeroNtf;



#define LADDER_SET_HEIGHT   1136

#define BOUNCE_NORMAL_SPEED   1050

typedef enum
{
    // ladder tag define
    LADDER_BEGIN_TAG = 999,
    LADDER_NORMAL_LONG_TAG = 1000,
    LADDER_NORMAL_SHORT_TAG = 1001,
    LADDER_HIDE_LONG_TAG = 1010,
    LADDER_HIDE_SHORT_TAG,
    LADDER_MOVE_LANDSCAPE_LONG_TAG = 1020,
    LADDER_MOVE_LANDSCAPE_SHORT_TAG,
    LADDER_MOVE_PORTRAIT_LONG_TAG = 1030,
    LADDER_MOVE_PORTRAIT_SHORT_TAG,
    LADDER_ONCE_LONG_TAG = 1040,
    LADDER_ONCE_SHORT_TAG,
    LADDER_BROKEN_LONG_TAG = 1050,
    LADDER_BROKEN_SHORT_TAG,
    LADDER_BOMB_TAG = 1060,
	LADDER_BOMB_OFF_TAG = 1061,
	LADDER_RANDOM_MOVE_TAG = 1070,
	LADDER_CONTROL_TAG = 1080,
    LADDER_END_TAG = 1999,
    
    PROP_START_TAG = 2000,
    PROP_SPRING_TAG = 2001,
    PROP_TRAMPOLINE_TAG = 2002,
    PROP_STAR_TAG = 2003,
    PROP_HELICOPTER_TAG = 2004,
    PROP_MEDICINE_TAG = 2005,
    PROP_MAGNET_TAG = 2006,
    PROP_SHIELD_TAG = 2007,
    PROP_GOLD_TAG = 2008,
	PROP_GOLD_SMALL_TAG = 2018,
	PROP_GOLD_ABSORB_TAG = 2108,
    PROP_TORPEDO_TAG = 2009,
	PROP_SHIP_TAG = 2010,
	PROP_VORTEX_TAG = 2011,
	PROP_RANDOM_TAG = 2012,
	PROP_SPUERBUBBLE_TAG = 2013,
	PROP_DIESPRINT_TAG = 2014,
	PROP_MUSHROOM_TAG = 2015,
	PROP_JET_TAG = 2016,
	PROP_BUBBLE_TAG = 2017,
	PROP_SPRINT_TAG = 2018,
    PROP_LEVEL_OVER_TAG = 2019,
    PROP_END_TAG =2999,

	// monster tag define
	MONSTER_BEGIN_TAG = 3000,
	MONSTER_JELLYFISH_BIG_TAG = 3001,
	MONSTER_JELLYFISH_SMALL_TAG = 3002,
	MONSTER_PIRANHA_BIG_TAG = 3003,
	MONSTER_PIRANHA_NORMAL_TAG = 3004,
	MONSTER_PIRANHA_SMALL_TAG = 3005,
	MONSTER_SEASNAKE_BIG_TAG = 3006,
	MONSTER_SEASNAKE_SMALL_TAG = 3007,
	MONSTER_END_TAG = 3999,

	// Animation tag define
	ANIMATION_BEGIN_TAG = 4000,
	ANIMATION_MAGNET_TAG = 4001,

	ANIMATION_END_TAG = 4999,

    // hero tag define
    HERO_BEGIN_TAG = 9999,
    HERO_1_TAG = 10000,
    HERO_2_TAG = 10001,
    HERO_END_TAG = 10009
    
} GAME_OBJ_TAG;

#endif
