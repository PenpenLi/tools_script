//
//  GameState.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 18/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#include "GameState.h"
#include "GameScene.h"
#include "GameManager.h"
#include "Mission.h"
#include "DailyReward.h"
#include "ParamManager.h"
#include "FacebookManager.h"
#include "GCManager.h"
#include "Achievement.h"





class TimeEntity : public NSObject {public:
    GameEntityType type;
    float distance;


@implementation TimeEntity







Statistics







class Level : public NSObject {public:
    ParallaxLayerType type;
    int number;
    float minDifficult;
    float maxDifficult;
    float targetDistance;
    float targetTime;
    float currentDistance;
    float currentTime;
    bool allowEntities;
    bool allowEnemyParachutist;
    bool allowEnemyJetpack;
    bool allowEnemyBoat;
    bool allowEnemyMine;
    bool allowEnemyHelicopter;
    bool allowEnemyBomber;
    bool allowEnemySubmarine;
    bool allowShamrock;
@property (retain) Level *nextLevel;

GameState* GameState::init() 
{
    this = super->init();
    if (this){
        this->number = 0;
        this->targetDistance = 0.0f;
        this->targetTime = 0.0f;
        this->currentDistance = 0.0f;
        this->currentTime = 0.0f;
        this->allowEntities = false;
        this->allowEnemyParachutist = false;
        this->allowEnemyJetpack = false;
        this->allowEnemyBoat = false;
        this->allowEnemyMine = false;
        this->allowEnemyHelicopter = false;
        this->allowEnemyBomber = false;
        this->allowEnemySubmarine = false;
        this->allowShamrock = false;
    }
    return this;
}

bool GameState::canCreateEntity() 
{
    // Deja al menos un par de segundos para crear entidades para que dé tiempo a esconderse el texto de la GUI
    if (this->currentTime > 2.0f){
        if (this->targetTime > 0.0f)
            return this->allowEntities && (this->currentTime < this->targetTime || this->nextLevel == NULL);
        else
            return this->allowEntities && (this->currentDistance < this->targetDistance || this->nextLevel == NULL);
    }
    return false;
}

float GameState::difficultyFactor() 
{
    if (this->targetTime > 0.0f)
        return fminf(this->maxDifficult,this->minDifficult + (this->maxDifficult-this->minDifficult)*(this->currentTime/this->targetTime));
    else
        return fminf(this->maxDifficult,this->minDifficult + (this->maxDifficult-this->minDifficult)*(this->currentDistance/this->targetDistance));
}

void GameState::dealloc() 
{
    this->nextLevel = NULL;
    super->dealloc();
}







// Velocidad mínima y máxima
#define MIN_SPEED 15.0f
#define MAX_SPEED 45.0f

// Segundos para volver a la dificultad normal después de recibir un toque
#define RELAX_TIME_AFTER_HIT 10

// En metros
#define MIN_INTERVAL_ENTITY 50
#define MAX_INTERVAL_ENTITY 80

// En segundos
#define MIN_INTERVAL_CARD 20
#define MAX_INTERVAL_CARD 35

// En metros
#define MIN_INTERVAL_WAVE 100
#define MAX_INTERVAL_WAVE 300

// En número de enemigos matados
#define MIN_INTERVAL_MONEY_PLANE 30
#define MAX_INTERVAL_MONEY_PLANE 50

// En segundos
#define MIN_INTERVAL_SAMBA 20
#define MAX_INTERVAL_SAMBA 50

@implementation GameState{
    bool isDailyRewardAvailable;
    bool isPlayer1Died;
    bool isPlayer2Died;
    bool isSecondChanceRefusedForPlayer1;
    bool isSecondChanceRefusedForPlayer2;
    bool isSambaAvailable;
    bool isCrazyModeEnabled;
    float timeWithoutHit;
    float timeAccelerating;
    float speed;
    float targetSpeed;
    float nextEnemyDistance;
    float nextPowerUpWeaponTime;
    float nextBonusLetterTime;
    float nextCardTime;
    float nextSpecialCoinDistance;
    float nextShamrockDistance;
    float nextPokerChipTime;
    float nextHealthPackTime;
    float nextWaveDistance;
    float nextSambaTime;
    
    // En segundos
    int minIntervalPowerUpWeapon;
    int maxIntervalPowerUpWeapon;
    
    // En segundos
    int minIntervalBonusLetter;
    int maxIntervalBonusLetter;
    
    // En metros
    int minIntervalSpecialCoin;
    int maxIntervalSpecialCoin;
    
    // En segundos
    int minIntervalPokerChip;
    int maxIntervalPokerChip;
    
    // En segundos
    int minIntervalHealthPack;
    int maxIntervalHealthPack;
    
    ccTime comboTime;
    int combo;
    int maxCombo;
    int nextMoneyPlaneKills;
    int statisticsLevelLastNumWeaponsBuyed;
    int statisticsLevelLastNumPowerUpsUsed;
    int statisticsLevelLastNumLivesLost;
    int statisticsLevelLastNumKills;
    int statisticsLevelLastNumCoinsCollected;
    int statisticsLevelLastNumCoinsSpent;
    int statisticsLevelLastNumSpecialCoinsCollected;
    int statisticsLevelLastNumSpecialCoinsSpent;
    int statisticsLevelLastNumPokerChipsCollected;
    int statisticsLevelLastDistance;
    int statisticsLevelLastTime;
    AvatarConfig *avatarPlayer1;
    AvatarConfig *avatarPlayer2;
    VehicleConfig *vehiclePlayer1;
    VehicleConfig *vehiclePlayer2;
    WeaponConfig *weaponPlayer1;
    WeaponConfig *weaponPlayer2;
    PokerCardHand *cardHand;
    Level *level;
    UIImage *videoSnapshot;
    VideoSnapshotType videoSnapshotType;
    NSMutableArray *timeEntities;
    NSMutableArray *kills;
    NSMutableArray *dodges;
    NSMutableArray *powerUpsBuyed;
    NSMutableArray *cards;
    NSMutableArray *scoresToBeat;
    NSMutableArray *levelStatistics;
    NSMutableArray *weaponsUsedForKilling;
    NSMutableArray *hitSources;
    NSMutableArray *loadedWeapons;
   Vector<T>* missions;
   Vector<T>* levels;
}
GameState* GameState::initWithNumPlayers(int numPlayers) 
{
    this = super->init();
    if (this){
        GameManager *manager = GameManager::sharedManager();
        
        minIntervalPowerUpWeapon = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("weapon")->objectForKey("min")->intValue();
        maxIntervalPowerUpWeapon = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("weapon")->objectForKey("max")->intValue();
        minIntervalBonusLetter = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("bonus")->objectForKey("min")->intValue();
        maxIntervalBonusLetter = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("bonus")->objectForKey("max")->intValue();
        minIntervalSpecialCoin = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("gem")->objectForKey("min")->intValue();
        maxIntervalSpecialCoin = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("gem")->objectForKey("max")->intValue();
        minIntervalPokerChip = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("poker")->objectForKey("min")->intValue();
        maxIntervalPokerChip = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("poker")->objectForKey("max")->intValue();
        minIntervalHealthPack = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("heal")->objectForKey("min")->intValue();
        maxIntervalHealthPack = ParamManager::sharedManager()->objectForKey("Box")->objectForKey("heal")->objectForKey("max")->intValue();
        
        isDailyRewardAvailable = DailyReward::nextBonusLetter() != GAME_ENTITY_TYPE_NONE;
        isPlayer1Died = false;
        isPlayer2Died = numPlayers == 1 ? true : false;
        isSecondChanceRefusedForPlayer1 = false;
        isSecondChanceRefusedForPlayer2 = numPlayers == 1 ? true : false;
        isCrazyModeEnabled = false;
        avatarPlayer1 =::(->())->();
        avatarPlayer2 = numPlayers == 2 ?::(->())->() : NULL;
        vehiclePlayer1 =::(->())->();
        vehiclePlayer2 = numPlayers == 2 ?::(->())->() : NULL;
        isSambaAvailable = vehiclePlayer1->type() == VEHICLE_TYPE_CARRIAGE || vehiclePlayer2->type() == VEHICLE_TYPE_CARRIAGE;
        weaponPlayer1 = NULL;
        weaponPlayer2 = NULL;
        cardHand = NULL;
        speed = MIN_SPEED;
        targetSpeed = MIN_SPEED;
        comboTime = 100.0f;
        combo = 0;
        maxCombo = 0;
        timeWithoutHit = 0.0f;
        timeAccelerating = 0.0f;
        statisticsLevelLastNumWeaponsBuyed = 0;
        statisticsLevelLastNumPowerUpsUsed = 0;
        statisticsLevelLastNumLivesLost = 0;
        statisticsLevelLastNumKills = 0;
        statisticsLevelLastNumCoinsCollected = 0;
        statisticsLevelLastNumCoinsSpent = 0;
        statisticsLevelLastNumSpecialCoinsCollected = 0;
        statisticsLevelLastNumSpecialCoinsSpent = 0;
        statisticsLevelLastNumPokerChipsCollected = 0;
        statisticsLevelLastDistance = 0;
        statisticsLevelLastTime = 0;
        this->delegate = NULL;
        _numPlayers = numPlayers;
        _distance = 0.0f;
        this->time = 0.0f;
        this->maxLife = 3;
        this->lifePlayer1 = this->maxLife;
        this->lifePlayer2 = numPlayers == 2 ? this->maxLife : 0;
        this->livesLost = 0;
        this->coins = 0;
        this->coinsSpent = 0;
        this->coinsWhileDead = 0;
        this->specialCoins = 0;
        this->specialCoinsSpent = 0;
        this->pokerChipsTotal = 0;
        this->pokerChipsRemaining = 0;
        this->points = 0;
        this->jumps = 0;
        this->dives = 0;
        this->rings = 0;
        this->fishes = 0;
        this->minesJumped = 0;
        this->minesDived = 0;
        this->playerShotHits = 0;
        this->playerShotMisses = 0;
        this->enemyShotHits = 0;
        this->enemyShotMisses = 0;
        this->cardsDodged = 0;
        this->reborns = 0;
        this->pokerReborns = 0;
        this->friendBuoysDestroyed = 0;
        this->powerUpsUsed = 0;
        this->itemBoxesDestroyed = 0;
        this->minesDestroyedWithGadget = 0;
        this->weaponPowerBonus = 0.0f;
        this->weaponSpeedBonus = 0.0f;
        this->weaponAmmoBonus = 0.0f;
        switch (DailyReward::nextBonusLetter()) {
            case GAME_ENTITY_TYPE_BONUS_LETTER_B: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_NONE; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_O: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_B; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_N: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_O; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_U: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_N; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_S: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_U; break;
            default: this->lastBonusLetterPicked = GAME_ENTITY_TYPE_NONE; break;
        }
        this->gadget1 = numPlayers == 1 ? manager.gadget1 : GADGET_TYPE_NONE;
        this->gadget2 = numPlayers == 1 ? manager.gadget2 : GADGET_TYPE_NONE;
        this->gadget3 = numPlayers == 1 ? manager.gadget3 : GADGET_TYPE_NONE;
        this->helicopterTouched = false;
        this->submarineTouched = false;
        this->boatTouched = false;
        this->isGameStarted = false;
        this->isGameOver = false;
        this->isKrakenReleased = false;
        this->isEnemyBitten = false;
        this->isEnemyElectrocuted = false;
        this->isTutorial = false;
        this->isVideoRewarded = false;
        videoSnapshot = NULL;
        videoSnapshotType = VIDEO_SNAPSHOT_TYPE_NONE;
        timeEntities = NSMutableArray::initWithCapacity(10);
        kills = NSMutableArray::initWithCapacity(100);
        dodges = NSMutableArray::initWithCapacity(100);
        powerUpsBuyed = NSMutableArray::initWithCapacity(20);
        cards = NSMutableArray::initWithCapacity(5);
        scoresToBeat = NSMutableArray::initWithCapacity(20);
        levelStatistics = NSMutableArray::initWithCapacity(10);
        weaponsUsedForKilling = NSMutableArray::initWithCapacity(10);
        hitSources = NSMutableArray::initWithCapacity(10);
        loadedWeapons = NSMutableArray::initWithCapacity(10);
        MilitarRange *range = MilitarRange::currentRange();
        missions = NSArray::initWithArray(Mission::missionsWithRange(range->type()));
        
        // Bonus
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_POWER_1))    this->weaponPowerBonus += 0.1f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_POWER_2))    this->weaponPowerBonus += 0.15f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_POWER_3))    this->weaponPowerBonus += 0.2f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_SPEED_1))    this->weaponSpeedBonus += 0.1f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_SPEED_2))    this->weaponSpeedBonus += 0.15f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_SPEED_3))    this->weaponSpeedBonus += 0.2f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_AMMO_1))     this->weaponAmmoBonus += 0.1f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_AMMO_2))     this->weaponAmmoBonus += 0.15f;
        if (this->isGadgetEnabled(GADGET_TYPE_MORE_AMMO_3))     this->weaponAmmoBonus += 0.2f;
        
        // Las primeras 5 partidas aumenta la probabilidad de salir ítems
        float beginnerFactor = 0.5f + fminf(manager.gamesPlayed,5) * 0.1f;
        
        nextEnemyDistance = this->randomEntityInterval();
        nextPowerUpWeaponTime = this->randomPowerUpWeaponInterval() * beginnerFactor;
        nextBonusLetterTime = this->randomBonusLetterInterval() * beginnerFactor;
        nextSpecialCoinDistance = this->randomSpecialCoinInterval() * beginnerFactor;
        nextPokerChipTime = this->randomPokerChipInterval() * beginnerFactor;
        nextShamrockDistance = 99999.0f;
        nextMoneyPlaneKills = this->randomMoneyPlaneInterval();
        nextSambaTime = this->randomSambaInterval();
        nextHealthPackTime = this->randomHealthPackInterval();
        nextWaveDistance = this->randomWaveInterval();
        nextCardTime = this->randomCardInterval();
        this->loadWeapon(WEAPON_TYPE_PISTOL, 1);
        this->loadWeapon(WEAPON_TYPE_PISTOL, 2);
        
        Level *entry = Level::create();
        Level *jungle = Level::create();
        Level *jungleTransition = Level::create();
        Level *asian = Level::create();
        Level *asianTransition = Level::create();
        Level *snow = Level::create();
        Level *snowTransition = Level::create();
        Level *sunset = Level::create();
        Level *sunsetTransition = Level::create();
        Level *volcano = Level::create();
        Level *volcanoTransition = Level::create();
        Level *city = Level::create();
        Level *cityTransition = Level::create();
        Level *tempest = Level::create();
        // Para tener hecho un retain de ellos se pone el array
        levels = NSArray::initWithObjects(entry,jungle,jungle,jungleTransition,asian,asianTransition,snow,snowTransition,sunset,sunsetTransition,volcano,volcanoTransition,city,cityTransition,tempest,NULL);
        
        entry.type = PARALLAX_LAYER_TYPE_ENTRY;
        entry.minDifficult = 0.0f;
        entry.maxDifficult = 0.0f;
        entry.targetDistance = 200.0f;
        entry.targetTime = 6.0f;
        entry.allowEntities = false;
        entry.nextLevel = jungle;

        jungle.type = PARALLAX_LAYER_TYPE_JUNGLE;
        jungle.number = 1;
        jungle.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("1")->objectForKey("minDifficulty")->floatValue();
        // Para el primer nivel incrementa la dificultad en usuarios avanzados
        jungle.minDifficult = fmaxf(jungle.minDifficult*0.1f,fminf(jungle.minDifficult,jungle.minDifficult*0.1f*manager.gamesPlayed));
        jungle.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("1")->objectForKey("maxDifficulty")->floatValue();
        jungle.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("1")->objectForKey("length")->floatValue();
        jungle.allowEntities = true;
        jungle.allowEnemyParachutist = true;
        jungle.allowEnemyJetpack = true;
        jungle.allowEnemyBoat = true;
        jungle.allowEnemyMine = false;
        jungle.allowEnemyHelicopter = false;
        jungle.allowEnemyBomber = false;
        jungle.allowEnemySubmarine = false;
        jungle.allowShamrock = arc4random() % 2 == 0; // Shamrock aleatorio en el primer nivel al 50% probabilidad
        jungle.nextLevel = jungleTransition;
        
        jungleTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        jungleTransition.minDifficult = 0.0f;
        jungleTransition.maxDifficult = 0.0f;
        jungleTransition.targetTime = 6.0f;
        jungleTransition.allowEntities = false;
        jungleTransition.nextLevel = asian;
        
        asian.type = PARALLAX_LAYER_TYPE_ASIAN;
        asian.number = 2;
        asian.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("2")->objectForKey("minDifficulty")->floatValue();
        asian.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("2")->objectForKey("maxDifficulty")->floatValue();
        asian.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("2")->objectForKey("length")->floatValue();
        asian.allowEntities = true;
        asian.allowEnemyParachutist = true;
        asian.allowEnemyJetpack = true;
        asian.allowEnemyBoat = true;
        asian.allowEnemyMine = true;
        asian.allowEnemyHelicopter = false;
        asian.allowEnemyBomber = true;
        asian.allowEnemySubmarine = false;
        asian.nextLevel = asianTransition;
        
        asianTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        asianTransition.minDifficult = 0.0f;
        asianTransition.maxDifficult = 0.0f;
        asianTransition.targetTime = 6.0f;
        asianTransition.allowEntities = false;
        asianTransition.nextLevel = sunset;
        
        sunset.type = PARALLAX_LAYER_TYPE_SUNSET;
        sunset.number = 3;
        sunset.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("3")->objectForKey("minDifficulty")->floatValue();
        sunset.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("3")->objectForKey("maxDifficulty")->floatValue();
        sunset.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("3")->objectForKey("length")->floatValue();
        sunset.allowEntities = true;
        sunset.allowEnemyParachutist = true;
        sunset.allowEnemyJetpack = true;
        sunset.allowEnemyBoat = true;
        sunset.allowEnemyMine = true;
        sunset.allowEnemyHelicopter = true;
        sunset.allowEnemyBomber = true;
        sunset.allowEnemySubmarine = true;
        sunset.nextLevel = sunsetTransition;
        
        sunsetTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        sunsetTransition.minDifficult = 0.0f;
        sunsetTransition.maxDifficult = 0.0f;
        sunsetTransition.targetTime = 6.0f;
        sunsetTransition.allowEntities = false;
        sunsetTransition.nextLevel = snow;
        
        snow.type = PARALLAX_LAYER_TYPE_SNOW;
        snow.number = 4;
        snow.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("4")->objectForKey("minDifficulty")->floatValue();
        snow.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("4")->objectForKey("maxDifficulty")->floatValue();
        snow.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("4")->objectForKey("length")->floatValue();
        snow.allowEntities = true;
        snow.allowEnemyParachutist = true;
        snow.allowEnemyJetpack = true;
        snow.allowEnemyBoat = true;
        snow.allowEnemyMine = true;
        snow.allowEnemyHelicopter = true;
        snow.allowEnemyBomber = true;
        snow.allowEnemySubmarine = true;
        snow.nextLevel = snowTransition;
        
        snowTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        snowTransition.minDifficult = 0.0f;
        snowTransition.maxDifficult = 0.0f;
        snowTransition.targetTime = 6.0f;
        snowTransition.allowEntities = false;
        snowTransition.nextLevel = volcano;
        
        volcano.type = PARALLAX_LAYER_TYPE_VOLCANO;
        volcano.number = 5;
        volcano.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("5")->objectForKey("minDifficulty")->floatValue();
        volcano.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("5")->objectForKey("maxDifficulty")->floatValue();
        volcano.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("5")->objectForKey("length")->floatValue();
        volcano.allowEntities = true;
        volcano.allowEnemyParachutist = true;
        volcano.allowEnemyJetpack = true;
        volcano.allowEnemyBoat = true;
        volcano.allowEnemyMine = true;
        volcano.allowEnemyHelicopter = true;
        volcano.allowEnemyBomber = true;
        volcano.allowEnemySubmarine = true;
        volcano.nextLevel = volcanoTransition;
        
        volcanoTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        volcanoTransition.minDifficult = 0.0f;
        volcanoTransition.maxDifficult = 0.0f;
        volcanoTransition.targetTime = 6.0f;
        volcanoTransition.allowEntities = false;
        volcanoTransition.nextLevel = city;
        
        city.type = PARALLAX_LAYER_TYPE_CITY;
        city.number = 6;
        city.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("6")->objectForKey("minDifficulty")->floatValue();
        city.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("6")->objectForKey("maxDifficulty")->floatValue();
        city.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("6")->objectForKey("length")->floatValue();
        city.allowEntities = true;
        city.allowEnemyParachutist = true;
        city.allowEnemyJetpack = true;
        city.allowEnemyBoat = true;
        city.allowEnemyMine = true;
        city.allowEnemyHelicopter = true;
        city.allowEnemyBomber = true;
        city.allowEnemySubmarine = true;
        city.nextLevel = cityTransition;
        
        cityTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        cityTransition.minDifficult = 0.0f;
        cityTransition.maxDifficult = 0.0f;
        cityTransition.targetTime = 6.0f;
        cityTransition.allowEntities = false;
        cityTransition.nextLevel = tempest;
        
        tempest.type = PARALLAX_LAYER_TYPE_TEMPEST;
        tempest.number = 7;
        tempest.minDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("7")->objectForKey("minDifficulty")->floatValue();
        tempest.maxDifficult = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("7")->objectForKey("maxDifficulty")->floatValue();
        tempest.targetDistance = ParamManager::sharedManager()->objectForKey("Stages")->objectForKey("7")->objectForKey("length")->floatValue();
        tempest.allowEntities = true;
        tempest.allowEnemyParachutist = true;
        tempest.allowEnemyJetpack = true;
        tempest.allowEnemyBoat = true;
        tempest.allowEnemyMine = true;
        tempest.allowEnemyHelicopter = true;
        tempest.allowEnemyBomber = true;
        tempest.allowEnemySubmarine = true;
        tempest.nextLevel = NULL;
        
        level = entry;
        
        this->loadScoresToBeat();, :defaultCenter()->addObserver(this->selector:@selector(loadScoresToBeat), FacebookChangedStateNotification, NULL);
    }
    return this;
}

void GameState::dealloc() 
{
    NSNotificationCenter::defaultCenter()->removeObserver(this);
    this->delegate = NULL;
    videoSnapshot->release();
    avatarPlayer1->release();
    avatarPlayer2->release();
    vehiclePlayer1->release();
    vehiclePlayer2->release();
    weaponPlayer1->release();
    weaponPlayer2->release();
    cardHand->release();
    timeEntities->release();
    kills->release();
    dodges->release();
    powerUpsBuyed->release();
    levels->release();
    levelStatistics->release();
    weaponsUsedForKilling->release();
    missions->release();
    cards->release();
    scoresToBeat->release();
    hitSources->release();
    loadedWeapons->release();
    super->dealloc();
}

void GameState::enableCrazyMode() 
{
    isCrazyModeEnabled = true;
    for (Level *l in levels){
        l.minDifficult = 1.0f;
        l.maxDifficult = 1.0f;
    }
}

bool GameState::isCrazyModeEnabled() 
{
    return isCrazyModeEnabled;
}

bool GameState::isPlayerDead(int numPlayer) 
{
    return numPlayer == 1 ? isPlayer1Died : isPlayer2Died;
}

void GameState::setDied(bool died, int numPlayer) 
 {
    if (numPlayer == 1) isPlayer1Died = died;
    else                isPlayer2Died = died;
}

bool GameState::isSecondChanceRefusedForPlayer(int numPlayer) 
{
    return numPlayer == 1 ? isSecondChanceRefusedForPlayer1 : isSecondChanceRefusedForPlayer2;
}

void GameState::setSecondChanceRefused(bool refused, int numPlayer) 
 {
    if (numPlayer == 1) isSecondChanceRefusedForPlayer1 = refused;
    else                isSecondChanceRefusedForPlayer2 = refused;
}

bool GameState::areAllPlayersDied() 
{
    return isPlayer1Died && isPlayer2Died;
}

void GameState::loadScoresToBeat() 
{
    // La puntuación del usuario en Facebook y GC false se tiene en cuenta, siempre se
    // tiene en cuenta su puntuación local (comportamiento igual al de Run Sheldon)
    // LAS PUNTUACIONES ESTÁN DE MAYOR A MENOR
    scoresToBeat->removeAllObjects();
    int bestDistance = GameManager::sharedManager().bestDistance;
    
    auto allScores = new Vector<T>;
    
    // Puntuaciones Game Center
    GCManager *gcManager = GCManager::sharedManager();
    if (gcManager->isLocalPlayerAuthenticated()){
       Vector<T>* gcScores = gcManager->allScores();
        if (gcScores)
            allScores->addObjectsFromArray(gcScores);
    }
    
    // Puntuaciones Facebook
    FacebookManager *fbManager = FacebookManager::sharedManager();
    if (fbManager->isLogged()){
       Vector<T>* fbScores = fbManager->allScores();
        if (fbScores)
            allScores->addObjectsFromArray(fbScores);
    }
    
    // Añade las puntuaciones una a una (sólo aquellas mayores al récord actual) en orden
    // de mayor a menor
    for (UserScore *newScore in allScores){
        if (!newScore->isMe()){
            if (newScore->score() > bestDistance){
                bool lastEntry = true;
                int index = 0;
                for (UserScore *s in scoresToBeat){
                    if (newScore->score() == [s score]){
                        // Puntuaciones iguales false
                        lastEntry = false;
                        break;
                    }
                    else if (newScore->score() > [s score]){
                        lastEntry = false;
                        scoresToBeat->insertObject(newScore, index);
                        break;
                    }
                    index++;
                }
                // Lo añade al final si es necesario
                if (lastEntry)
                    scoresToBeat->pushBack(newScore);
            }
        }
    }
    
    // Por último añade el récord del jugador el primero
    if (bestDistance > 0){
        // Prueba primero con GC, si false con FB que siempre devuelve algo
        UserScore *myScore = gcManager->myScoreWithCustomScore(bestDistance);
        if (!myScore)
            myScore = fbManager->myScoreWithCustomScore(bestDistance);
        scoresToBeat->pushBack(myScore);
    }
}

float GameState::randomEntityInterval() 
{
    float interval = (MIN_INTERVAL_ENTITY + arc4random() % (MAX_INTERVAL_ENTITY - MIN_INTERVAL_ENTITY));
    if (this->numPlayers == 2)
        interval *= 0.65f;
    return interval;
}

float GameState::randomPowerUpWeaponInterval() 
{
    return (minIntervalPowerUpWeapon + arc4random() % (maxIntervalPowerUpWeapon - minIntervalPowerUpWeapon));
}

float GameState::randomBonusLetterInterval() 
{
    return (minIntervalBonusLetter + arc4random() % (maxIntervalBonusLetter - minIntervalBonusLetter));
}

float GameState::randomCardInterval() 
{
    return (MIN_INTERVAL_CARD + arc4random() % (MAX_INTERVAL_CARD - MIN_INTERVAL_CARD));
}

float GameState::randomSpecialCoinInterval() 
{
    return (minIntervalSpecialCoin + arc4random() % (maxIntervalSpecialCoin - minIntervalSpecialCoin));
}

float GameState::randomPokerChipInterval() 
{
    return (minIntervalPokerChip + arc4random() % (maxIntervalPokerChip - minIntervalPokerChip));
}

float GameState::randomHealthPackInterval() 
{
    return (minIntervalHealthPack + arc4random() % (maxIntervalHealthPack - minIntervalHealthPack));
}

float GameState::randomWaveInterval() 
{
    return (MIN_INTERVAL_WAVE + arc4random() % (MAX_INTERVAL_WAVE - MIN_INTERVAL_WAVE));
}

float GameState::randomMoneyPlaneInterval() 
{
    return (MIN_INTERVAL_MONEY_PLANE + arc4random() % (MAX_INTERVAL_MONEY_PLANE - MIN_INTERVAL_MONEY_PLANE));
}

float GameState::randomSambaInterval() 
{
    return (MIN_INTERVAL_SAMBA + arc4random() % (MAX_INTERVAL_SAMBA - MIN_INTERVAL_SAMBA));
}

GameEntityType GameState::randomEnemy() 
{
    int random = arc4random() % 10;
    if (random <= 2 && level.allowEnemyParachutist) // 30%
        return GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
    else if (random <= 4 && level.allowEnemyJetpack) // 20%
        return GAME_ENTITY_TYPE_ENEMY_JETPACK;
    else{ // 50%
        bool canBeHelicopter = level.allowEnemyHelicopter;
        bool canBeBomber = level.allowEnemyBomber;
        bool canBeSubmarine = level.allowEnemySubmarine;
        bool canBeBoat = level.allowEnemyBoat;
        bool canBeMine = level.allowEnemyMine;
        
        GameLayer *gameLayer = this->gameLayer();
       Vector<T>* entities = gameLayer->entities();
        for (GameEntity *e in entities){
            switch (e.type) {
                case GAME_ENTITY_TYPE_ENEMY_HELICOPTER:
                case GAME_ENTITY_TYPE_ENEMY_BOMBER:
                    canBeHelicopter = false;
                    canBeBomber = false;
                    break;
                case GAME_ENTITY_TYPE_ENEMY_SUBMARINE:
                    canBeSubmarine = false;
                    break;
                case GAME_ENTITY_TYPE_ENEMY_BOAT:
                case GAME_ENTITY_TYPE_ENEMY_MINE:
                    canBeBoat = false;
                    canBeMine = false;
                    break;
                default:
                    break;
            }
        }
        
        auto types = new Vector<T>;
        if (canBeHelicopter)    types->pushBack(NSNumber::numberWithInt(GAME_ENTITY_TYPE_ENEMY_HELICOPTER));
        if (canBeBomber)        types->pushBack(NSNumber::numberWithInt(GAME_ENTITY_TYPE_ENEMY_BOMBER));
        if (canBeSubmarine)     types->pushBack(NSNumber::numberWithInt(GAME_ENTITY_TYPE_ENEMY_SUBMARINE));
        if (canBeBoat)          types->pushBack(NSNumber::numberWithInt(GAME_ENTITY_TYPE_ENEMY_BOAT));
        if (canBeMine)          types->pushBack(NSNumber::numberWithInt(GAME_ENTITY_TYPE_ENEMY_MINE));
        
        if (types->count() == 0)
            return GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
        else{
            int random = arc4random() % types->count();
            return (GameEntityType)[types->objectAtIndex(random]intValue);
        }
    }
}

GameEntityType GameState::randomPowerUpWeapon() 
{
   Vector<T>* weapons = NULL;
    int player = (arc4random() % _numPlayers) + 1;
    if (player == 1)    weapons = avatarPlayer1->weapons();
    else                weapons = avatarPlayer2->weapons();
    WeaponConfig *weapon = weapons->objectAtIndex(arc4random()%weapons->count());
    return weapon->powerUpRelationship();
}

bool GameState::existEnemiesOnGame() 
{
    return [[this->gameLayer()enemies]count] > 0 || timeEntities->count() > 0;
}

bool GameState::existGunsOnGame() 
{
    return [[this->gameLayer()guns]count] > 0;
}

bool GameState::existsMoneyPlaneOnGame() 
{
    GameLayer *gameLayer = this->gameLayer();
   Vector<T>* entities = gameLayer->entities();
    bool found = false;
    for (GameEntity *e in entities){
        if (e.type == GAME_ENTITY_TYPE_MONEY_PLANE){
            found = true;
            break;
        }
    }
    return found;
}

bool GameState::existsItemBoxOnGame() 
{
    GameLayer *gameLayer = this->gameLayer();
   Vector<T>* entities = gameLayer->entities();
    bool found = false;
    for (GameEntity *e in entities){
        if (e.type == GAME_ENTITY_TYPE_ITEM_BOX){
            found = true;
            break;
        }
    }
    return found;
}

GameScene* GameState::gameScene() 
{
    return GameScene::runningScene();
}

GameLayer* GameState::gameLayer() 
{
    return [this->gameScene()gameLayer];
}

void GameState::update(ccTime dt) 
{
    level.currentTime += dt;
    timeWithoutHit += dt;
    if (this->gameLayer()->isPressingRunningButtonForPlayer(1))
        timeAccelerating += dt;
    else
        timeAccelerating = 0.0f;
    
    // Esto se hace para que false frene de pronto por haber sido golpeado por un enemigo
    if (targetSpeed > 0.0f){
        if (speed > targetSpeed)
            speed *= 0.99f;
        else
            targetSpeed = 0.0f;
    }
    else
        speed = fminf(MAX_SPEED, fmaxf(MIN_SPEED, speed + dt*this->difficultyFactor()));
    
    comboTime += dt;
    if (comboTime >= 1.25f)
        combo = 0;
    
    this->updateMissions();
    
    if (!level.allowEntities){
        // Ya que false permite entidades aumenta el tiempo a la que deben aparecer las entidades
        nextPowerUpWeaponTime += dt;
        nextBonusLetterTime += dt;
        nextCardTime += dt;
        nextPokerChipTime += dt;
        nextHealthPackTime += dt;
        nextSambaTime += dt;
    }
}

void GameState::updateMissions() 
{
    for (Mission *m in missions)
        [m updateWithGameState:this];
}

void GameState::addDistance(float distance) 
{
    if (!this->isTutorial){
        _distance += distance;
        _distance = fminf(99999.0f,_distance);
        level.currentDistance += distance;
        
        if (!level.allowEntities){
            // Ya que false permite entidades aumenta la distancia a la que deben aparecer las entidades
            nextEnemyDistance = this->distance + this->randomEntityInterval();
            nextWaveDistance = this->distance + this->randomWaveInterval();
        }
    }
}

AvatarConfig* GameState::avatarForPlayer(int player) 
{
    return player == 1 ? avatarPlayer1 : avatarPlayer2;
}

VehicleConfig* GameState::vehicleForPlayer(int player) 
{
    return player == 1 ? vehiclePlayer1 : vehiclePlayer2;
}

GameEntityType GameState::nextEnemy() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (timeEntities->count() > 0){
        TimeEntity *candidate = NULL;
        for (TimeEntity *t in timeEntities){
            if (this->distance >= t.distance){
                candidate = t;
                break;
            }
        }
        if (candidate){
            GameEntityType type = candidate.type;
            timeEntities->removeObject(candidate);
            return type;
        }
    }
    else if (level->canCreateEntity()){
        if (this->isGadgetEnabled(GADGET_TYPE_MONEY_PLANE) && this->numKills() >= nextMoneyPlaneKills){
            if (!this->existEnemiesOnGame()){
                type = GAME_ENTITY_TYPE_MONEY_PLANE;
                nextMoneyPlaneKills = this->numKills() + this->randomMoneyPlaneInterval();
            }
        }
        else if (this->distance >= nextEnemyDistance && !this->existsMoneyPlaneOnGame()){
            nextEnemyDistance = this->distance + this->randomEntityInterval();
            
            GameEntityType nextType = this->randomEnemy();
            
            if (nextType == GAME_ENTITY_TYPE_ENEMY_PARACHUTIST){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,this->difficultyFactor()*10)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 20.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = TimeEntity::create();
                    entity.type = GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
                    entity.distance = this->distance + i*incdistance;
                    timeEntities->pushBack(entity);
                }
            }
            else if (nextType == GAME_ENTITY_TYPE_ENEMY_JETPACK){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,this->difficultyFactor()*5)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 1.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = TimeEntity::create();
                    entity.type = GAME_ENTITY_TYPE_ENEMY_JETPACK;
                    entity.distance = this->distance + i*incdistance;
                    timeEntities->pushBack(entity);
                }
            }
            else if (nextType == GAME_ENTITY_TYPE_ENEMY_SUBMARINE){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,this->difficultyFactor()*4.0f)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 50.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = TimeEntity::create();
                    entity.type = GAME_ENTITY_TYPE_ENEMY_SUBMARINE;
                    entity.distance = this->distance + i*incdistance;
                    timeEntities->pushBack(entity);
                }
            }
            else
                type = nextType;
        }
    }

    return type;
}

GameEntityType GameState::nextItemBox() 
{
    GameEntityType itemType = GAME_ENTITY_TYPE_NONE;
    itemType = this->nextPowerUp();
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = this->nextBonusLetter();
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = this->nextCard();
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = this->nextSpecialCoin();
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = this->nextPokerChip();
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = this->nextShamrock();
    return itemType;
}

void GameState::resetWeaponItemBoxTimer() 
{
    nextPowerUpWeaponTime = this->time + this->randomPowerUpWeaponInterval();
}

GameEntityType GameState::nextPowerUp() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (level->canCreateEntity()){
        if (this->time >= nextPowerUpWeaponTime){
            if (!this->existsItemBoxOnGame()){
                type = this->randomPowerUpWeapon();
                nextPowerUpWeaponTime = this->time + this->randomPowerUpWeaponInterval();
            }
        }
        else if (this->time >= nextHealthPackTime){
            if (!this->existsItemBoxOnGame()){
                if (this->lifePlayer1 < this->maxLife || this->lifePlayer2 < this->maxLife)
                    type = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                nextHealthPackTime = this->time + this->randomHealthPackInterval();
            }
        }
        else if (isSambaAvailable && this->time >= nextSambaTime){
            if (!this->existsItemBoxOnGame()){
                type = GAME_ENTITY_TYPE_POWER_UP_SAMBA;
                nextSambaTime = this->time + this->randomSambaInterval();
            }
        }
    }
    
    return type;
}

GameEntityType GameState::nextBonusLetter() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (isDailyRewardAvailable && level->canCreateEntity()){
        if (this->time >= nextBonusLetterTime){
            if (!this->existsItemBoxOnGame()){
                switch (this->lastBonusLetterPicked) {
                    case GAME_ENTITY_TYPE_NONE:             type = GAME_ENTITY_TYPE_BONUS_LETTER_B; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_B:   type = GAME_ENTITY_TYPE_BONUS_LETTER_O; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_O:   type = GAME_ENTITY_TYPE_BONUS_LETTER_N; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_N:   type = GAME_ENTITY_TYPE_BONUS_LETTER_U; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_U:   type = GAME_ENTITY_TYPE_BONUS_LETTER_S; break;
                    default:break;
                }
                nextBonusLetterTime = this->time + this->randomBonusLetterInterval();
            }
        }
    }
    
    return type;
}

GameEntityType GameState::nextCard() 
{
    // Ya false se reparten cartas durante el juego
    return GAME_ENTITY_TYPE_NONE;
    /*
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (level->canCreateEntity()){
        if ([this->cards()count] < 5 && this->time >= nextCardTime){
            if (!this->existsItemBoxOnGame()){
                type = GAME_ENTITY_TYPE_CARD;
                nextCardTime = this->time + this->randomCardInterval();
            }
        }
    }
    
    return type;
     */
}

GameEntityType GameState::nextSpecialCoin() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (level->canCreateEntity()){
        if (this->distance >= nextSpecialCoinDistance){
            if (!this->existsItemBoxOnGame()){
                type = GAME_ENTITY_TYPE_SPECIAL_COIN;
                nextSpecialCoinDistance = this->distance + this->randomSpecialCoinInterval();
            }
        }
    }
    
    return type;
}

GameEntityType GameState::nextPokerChip() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (level->canCreateEntity()){
        if (this->time >= nextPokerChipTime){
            if (!this->existsItemBoxOnGame()){
                type = GAME_ENTITY_TYPE_POKER_CHIP;
                nextPokerChipTime = this->time + this->randomPokerChipInterval();
            }
        }
    }
    
    return type;
}

GameEntityType GameState::nextShamrock() 
{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (level->canCreateEntity() && level.allowShamrock){
        if (this->distance >= nextShamrockDistance){
            if (!this->existsItemBoxOnGame()){
                type = GAME_ENTITY_TYPE_SHAMROCK;
                // Solo 1 shamrock por nivel
                level.allowShamrock = false;
                nextShamrockDistance = 99999;
            }
        }
    }
    
    return type;
}

UserScore* GameState::nextScoreToBeat() 
{
    UserScore *score = NULL;
    if (scoresToBeat->count() > 0)
        score = scoresToBeat->lastObject();
    return score;
}

UserScore* GameState::nextScoreBeaten() 
{
    UserScore *score = NULL;
    if (scoresToBeat->count() > 0){
        UserScore *next = scoresToBeat->lastObject();
        if (next.score < this->distance){
            score = next;
            // Esto para evitar que el retainCount sea 0 en cuanto se quite del array
            [score->retain()autorelease];
            scoresToBeat->removeLastObject();
        }
    }
    return score;
}

ParallaxLayerType GameState::currentLevel() 
{
    return level.type;
}

int GameState::currentLevelNumber() 
{
    return level.number;
}

float GameState::currentLevelTargetTime() 
{
    return level.targetTime;
}

ParallaxLayerType GameState::nextLevel() 
{
    ParallaxLayerType type = PARALLAX_LAYER_TYPE_NONE;
    if (!this->isTutorial && level.nextLevel != NULL){
        if ((level.targetTime > 0.0f && level.currentTime > level.targetTime) || (level.targetDistance > 0.0f && level.currentDistance > level.targetDistance)){
            if (    !this->existEnemiesOnGame()
                    && !this->existGunsOnGame()
                    && !this->existsMoneyPlaneOnGame()
                    && !this->existsItemBoxOnGame()
                    && ((this->lifePlayer1 > 0 && !isPlayer1Died) || (this->lifePlayer2 > 0 && !isPlayer2Died))){
                // Estadísticas en el caso de que sea un nivel jugable
                if (level.number > 0){
                    GameEntityType powerUpBuyedInLevel = GAME_ENTITY_TYPE_NONE;
                    int totalPowerUpsBuyed = (int)powerUpsBuyed->count();
                    if (totalPowerUpsBuyed > statisticsLevelLastNumWeaponsBuyed){
                        for (int i = statisticsLevelLastNumWeaponsBuyed; i < totalPowerUpsBuyed; i++){
                            GameEntityType type = (GameEntityType)[powerUpsBuyed->objectAtIndex(i]intValue);
                            if (type != GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                                powerUpBuyedInLevel = type;
                                break;
                            }
                        }
                    }
                    
                    int numRevivesBuyed = 0;
                    if (totalPowerUpsBuyed > statisticsLevelLastNumWeaponsBuyed){
                        for (int i = statisticsLevelLastNumWeaponsBuyed; i < totalPowerUpsBuyed; i++){
                            GameEntityType type = (GameEntityType)[powerUpsBuyed->objectAtIndex(i]intValue);
                            if (type == GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK)
                                numRevivesBuyed++;
                        }
                    }
                    
                    int powerUpsUsedInLevel = this->powerUpsUsed - statisticsLevelLastNumPowerUpsUsed;
                    int livesLostInLevel = this->livesLost - statisticsLevelLastNumLivesLost;
                    int killsInLevel = this->numKills() - statisticsLevelLastNumKills;
                    int coinsCollectedInLevel = this->coins - statisticsLevelLastNumCoinsCollected;
                    int coinsSpentInLevel = this->coinsSpent - statisticsLevelLastNumCoinsSpent;
                    int specialCoinsCollectedInLevel = this->specialCoins - statisticsLevelLastNumSpecialCoinsCollected;
                    int specialCoinsSpentInLevel = this->specialCoinsSpent - statisticsLevelLastNumSpecialCoinsSpent;
                    int pokerChipsInLevel = this->pokerChipsTotal - statisticsLevelLastNumPokerChipsCollected;
                    int distanceInLevel = (int)(this->distance - statisticsLevelLastDistance);
                    int durationInLevel = (int)(this->time - statisticsLevelLastTime);
                    
                    LevelStatistics *levelStats = LevelStatistics::create();
                    levelStats.level = level.number;
                    levelStats.powerUpBuyed = powerUpBuyedInLevel;
                    levelStats.numPowerUpsUsed = powerUpsUsedInLevel;
                    levelStats.numLostLives = livesLostInLevel;
                    levelStats.numEnemiesKilled = killsInLevel;
                    levelStats.numCoinsCollected = coinsCollectedInLevel;
                    levelStats.numCoinsSpent = coinsSpentInLevel;
                    levelStats.numSpecialCoinsCollected = specialCoinsCollectedInLevel;
                    levelStats.numSpecialCoinsSpent = specialCoinsSpentInLevel;
                    levelStats.numPokerChipsCollected = pokerChipsInLevel;
                    levelStats.numRebornsBuyed = numRevivesBuyed;
                    levelStats.distance = distanceInLevel;
                    levelStats.duration = durationInLevel;
                    levelStatistics->pushBack(levelStats);
                    
                    statisticsLevelLastNumWeaponsBuyed = (int)powerUpsBuyed->count();
                    statisticsLevelLastNumPowerUpsUsed = this->powerUpsUsed;
                    statisticsLevelLastNumLivesLost = this->livesLost;
                    statisticsLevelLastNumKills = this->numKills();
                    statisticsLevelLastNumCoinsCollected = this->coins;
                    statisticsLevelLastNumCoinsSpent = this->coinsSpent;
                    statisticsLevelLastNumSpecialCoinsCollected = this->specialCoins;
                    statisticsLevelLastNumSpecialCoinsSpent = this->specialCoinsSpent;
                    statisticsLevelLastNumPokerChipsCollected = this->pokerChipsTotal;
                    statisticsLevelLastDistance = (int)this->distance;
                    statisticsLevelLastTime = (int)this->time;
                }
                
                // Cambia de nivel
                level = level.nextLevel;
                type = level.type;
                
                // Nueva distancia del shamrock
                if (level.allowShamrock && level.targetDistance > 0)
                    nextShamrockDistance = this->distance + level.targetDistance * ((arc4random()%10)+1) * 0.09f;
            }
        }
    }
    
    // Sustituido en addDistance y update
    /*
    if (type != PARALLAX_LAYER_TYPE_NONE){
        // Reinicia los contadores
        nextEnemyDistance = this->distance + this->randomEntityInterval();
        nextPowerUpWeaponTime = this->time + this->randomPowerUpWeaponInterval();
        nextBonusLetterTime = this->time + this->randomBonusLetterInterval();
        nextCardTime = this->time + this->randomCardInterval();
        nextHealthPackTime = this->time + this->randomHealthPackInterval();
        nextWaveDistance = this->distance + this->randomWaveInterval();
    }
    */
    
    return type;
}

ParallaxLayerType GameState::nextLevelToPreload() 
{
    if (level.nextLevel)
        return level.nextLevel.type;
    else
        return PARALLAX_LAYER_TYPE_NONE;
}

float GameState::nextWave() 
{
    /*if (this->distance >= nextWaveDistance){
        nextWavedistance = this->distance + this->randomWaveInterval();
        
        // false puede haber una ola si hay un bote
        GameLayer *gameLayer = this->gameLayer();
       Vector<T>* entities = gameLayer->entities();
        bool canWave = true;
        for (GameEntity *e in entities){
            if (e.type == GAME_ENTITY_TYPE_ENEMY_BOAT){
                canWave = false;
                break;
            }
        }

        if (canWave)
            return 3.0f;
    }*/
    
    return 0.0f;
}

float GameState::speed() 
{
    return speed;
}

float GameState::difficultyFactor() 
{
    float hitFactor = fminf(1.0f,0.5f + (0.5f*timeWithoutHit/RELAX_TIME_AFTER_HIT));
    return level->difficultyFactor() * hitFactor;
}

void GameState::didHitPlayer(int numPlayer, GameEntityType source) 
 {
    hitSources->pushBack(NSNumber::numberWithInt(source));
    timeWithoutHit = 0.0f;
    if (!isCrazyModeEnabled)
        targetSpeed = fmaxf(MIN_SPEED, speed*0.5f);
}

int GameState::numHits() 
{
    return (int)hitSources->count();
}

int GameState::numHitsWithSource(GameEntityType source) 
{
    int numHits = 0;
    for (NSNumber *n in hitSources)
        if ([n intValue] == source)
            numHits++;
    return numHits;
}

bool GameState::isGadgetEnabled(GadgetType type) 
{
    return this->gadget1 == type || this->gadget2 == type || this->gadget3 == type;
}

int GameState::numGadgetsEnabled() 
{
    int count = 0;
    if (this->gadget1 != GADGET_TYPE_NONE) count++;
    if (this->gadget2 != GADGET_TYPE_NONE) count++;
    if (this->gadget3 != GADGET_TYPE_NONE) count++;
    return count;
}
Vector<T>* GameState::missions() 
{
    return missions;
}

void GameState::addKill(Kill* kill) 
{
    kills->pushBack(kill);
    if (kill.gun != GUN_TYPE_NONE){
        comboTime = 0.0f;
        combo++;
        if (combo > 1){
            maxCombo = MAX(maxCombo,combo);
            [this->delegate didPerformCombo:combo withKill:kill];
        }
    }
    
    // Como false hay una conversión directa entre GunType y WeaponType presuponemos que el arma utilizada
    // para matar es la que tiene en este momento
    NSNumber *obj = NSNumber::numberWithInt(weaponPlayer1.type);
    if (weaponsUsedForKilling->indexOfObject(obj) == NSNotFound)
        weaponsUsedForKilling->pushBack(obj);
    
    // Logros
    Achievement::didKill(this);
    if (kill.entity == GAME_ENTITY_TYPE_ENEMY_SUBMARINE), :didDestroySub(this);
}

int GameState::numKills() 
{
    return (int)kills->count();
}

int GameState::numKillsWithEntityType(GameEntityType type) 
{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.entity == type)
            count++;
    return count;
}

int GameState::numKillsWithGunType(GunType type) 
{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.gun == type)
            count++;
    return count;
}

int GameState::numKillsInBulletTime() 
{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.bulletTime)
            count++;
    return count;
}

int GameState::numDifferentWeaponsUsedForKilling() 
{
    return (int)weaponsUsedForKilling->count();
}

void GameState::addDodge(Dodge* dodge) 
{
    dodges->pushBack(dodge);
    
    // Logros
    Achievement::didDodge(this);
}

int GameState::numDodges() 
{
    return (int)dodges->count();
}

int GameState::numDodgesWithEntityType(GameEntityType type) 
{
    int count = 0;
    for (Dodge *dodge in dodges)
        if (dodge.entity == type)
            count++;
    return count;
}

WeaponConfig* GameState::weaponForPlayer(int player) 
{
    return player == 1 ? weaponPlayer1 : weaponPlayer2;
}

void GameState::loadWeapon(WeaponType type, int numPlayer) 
 {
    if (numPlayer == 1){
        weaponPlayer1->release();
        weaponPlayer1 = [WeaponConfig::weaponWithType(type]retain);
        this->ammoPlayer1 = weaponPlayer1->maxAmmo();
        if (this->ammoPlayer1 != INFINITE_AMMO)
            this->ammoPlayer1 = (int)(this->ammoPlayer1 * (1.0f + this->weaponAmmoBonus));
        
        // Logros
        Achievement::didLoadWeapon(this);
    }
    else{
        weaponPlayer2->release();
        weaponPlayer2 = [WeaponConfig::weaponWithType(type]retain);
        this->ammoPlayer2 = weaponPlayer2->maxAmmo();
        if (this->ammoPlayer2 != INFINITE_AMMO)
            this->ammoPlayer2 = (int)(this->ammoPlayer2 * (1.0f + this->weaponAmmoBonus));
    }
    loadedWeapons->pushBack(NSNumber::numberWithInt(type));
}

int GameState::numLoadedWeapons() 
{
    return (int)loadedWeapons->count();
}

int GameState::numLoadedWeaponsWithType(WeaponType type) 
{
    int count = 0;
    for (NSNumber *n in loadedWeapons)
        if ([n intValue] == type)
            count++;
    return count;
}

int GameState::combo() 
{
    return combo;
}

int GameState::maxCombo() 
{
    return maxCombo;
}

void GameState::addPowerUpBuyed(GameEntityType type) 
{
    powerUpsBuyed->pushBack(NSNumber::numberWithInt(type));
}

int GameState::numPowerUpsBuyed() 
{
    return (int)powerUpsBuyed->count();
}

int GameState::numPowerUpsBuyedWithType(GameEntityType type) 
{
    int count = 0;
    for (NSNumber *n in powerUpsBuyed)
        if ([n intValue] == type)
            count++;
    return count;
}

GameEntityType GameState::powerUpBuyedAtIndex(int index) 
{
    return (GameEntityType)[powerUpsBuyed->objectAtIndex(index]intValue);
}
Vector<T>* GameState::differentTypesOfPowerUpsBuyed() 
{
    auto differentPowerUps = new Vector<T>;
    for (NSNumber *n in powerUpsBuyed)
        if (differentPowerUps->indexOfObject(n) == NSNotFound)
            differentPowerUps->pushBack(n);
    return differentPowerUps;
}

PokerCardConfig* GameState::randomCard() 
{
    PokerCardConfig *config = NULL;
    while(true){
       Vector<T>* array = this->cards();
        
        bool canBeJoker = this->isGadgetEnabled(GADGET_TYPE_JOKER);
        int numJokers = 0;
        for (PokerCardConfig *c in array){
            if (c.suit == POKER_CARD_SUIT_TYPE_JOKER)
                numJokers++;
        }
        if (numJokers >= 3)
            canBeJoker = false;
        
        int cardNumber = (arc4random()%13)+1;
        int cardIndex = arc4random()%(canBeJoker ?,  4);
        PokerCardSuitType cardType = POKER_CARD_SUIT_TYPE_CLUB;
        switch (cardIndex) {
            case 0: cardType = POKER_CARD_SUIT_TYPE_CLUB; break;
            case 1: cardType = POKER_CARD_SUIT_TYPE_DIAMOND; break;
            case 2: cardType = POKER_CARD_SUIT_TYPE_HEART; break;
            case 3: cardType = POKER_CARD_SUIT_TYPE_SPADE; break;
            case 4: cardType = POKER_CARD_SUIT_TYPE_JOKER; break;
            default:break;
        }
        
        bool found = false;
        if (cardType != POKER_CARD_SUIT_TYPE_JOKER){
            for (PokerCardConfig *c in array){
                if (c.suit == cardType && c.number == cardNumber){
                    found = true;
                    break;
                }
            }
        }
        else
            cardNumber = 0;
        
        if (!found){
            config = PokerCardConfig::configWithSuit(cardType, cardNumber);
            break;
        }
    }
    return config;
}

void GameState::addCard(PokerCardConfig* config) 
{
    cards->pushBack(config);
    cardHand->release();
    cardHand = [PokerCardHand::handWithCards(cards]retain);
}

void GameState::replaceCardInIndex(int index, PokerCardConfig* config) 
 {
    cards->replaceObjectAtIndex(index, config);
    cardHand->release();
    cardHand = [PokerCardHand::handWithCards(cards]retain);
}
Vector<T>* GameState::cards() 
{
    return cards;
}

PokerCardHand* GameState::currentHand() 
{
    return cardHand;
}

void GameState::setVideoSnapshot(UIImage* snapshot, VideoSnapshotType type) 
 {
    videoSnapshot->release();
    videoSnapshot = snapshot->retain();
    videoSnapshotType = type;
}

UIImage* GameState::videoSnapshot() 
{
    return videoSnapshot;
}

VideoSnapshotType GameState::videoSnapshotType() 
{
    return videoSnapshotType;
}

int GameState::numLevelStatictics() 
{
    return (int)levelStatistics->count();
}

LevelStatistics* GameState::levelStaticticsAtIndex(int index) 
{
    return levelStatistics->objectAtIndex(index);
}

float GameState::timeWithoutHit() 
{
    return timeWithoutHit;
}

float GameState::timeAccelerating() 
{
    return timeAccelerating;
}


