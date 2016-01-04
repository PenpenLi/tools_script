//
//  GameState.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 18/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "GameState.h"
#import "GameScene.h"
#import "GameManager.h"
#import "Mission.h"
#import "DailyReward.h"
#import "ParamManager.h"
#import "FacebookManager.h"
#import "GCManager.h"
#import "Achievement.h"

#pragma mark -
#pragma mark TimeEntity
#pragma mark -

@interface TimeEntity : NSObject
@property GameEntityType type;
@property float distance;
@end

@implementation TimeEntity
@end


#pragma mark -
#pragma mark LevelStatistics
#pragma mark -

@implementation LevelStatistics
@end


#pragma mark -
#pragma mark Level
#pragma mark -

@interface Level : NSObject
@property ParallaxLayerType type;
@property int number;
@property float minDifficult;
@property float maxDifficult;
@property float targetDistance;
@property float targetTime;
@property float currentDistance;
@property float currentTime;
@property BOOL allowEntities;
@property BOOL allowEnemyParachutist;
@property BOOL allowEnemyJetpack;
@property BOOL allowEnemyBoat;
@property BOOL allowEnemyMine;
@property BOOL allowEnemyHelicopter;
@property BOOL allowEnemyBomber;
@property BOOL allowEnemySubmarine;
@property BOOL allowShamrock;
@property (retain) Level *nextLevel;
@end

@implementation Level

-(id)init{
    self = [super init];
    if (self){
        self.number = 0;
        self.targetDistance = 0.0f;
        self.targetTime = 0.0f;
        self.currentDistance = 0.0f;
        self.currentTime = 0.0f;
        self.allowEntities = NO;
        self.allowEnemyParachutist = NO;
        self.allowEnemyJetpack = NO;
        self.allowEnemyBoat = NO;
        self.allowEnemyMine = NO;
        self.allowEnemyHelicopter = NO;
        self.allowEnemyBomber = NO;
        self.allowEnemySubmarine = NO;
        self.allowShamrock = NO;
    }
    return self;
}

-(BOOL)canCreateEntity{
    // Deja al menos un par de segundos para crear entidades para que dé tiempo a esconderse el texto de la GUI
    if (self.currentTime > 2.0f){
        if (self.targetTime > 0.0f)
            return self.allowEntities && (self.currentTime < self.targetTime || self.nextLevel == nil);
        else
            return self.allowEntities && (self.currentDistance < self.targetDistance || self.nextLevel == nil);
    }
    return NO;
}

-(float)difficultyFactor{
    if (self.targetTime > 0.0f)
        return fminf(self.maxDifficult,self.minDifficult + (self.maxDifficult-self.minDifficult)*(self.currentTime/self.targetTime));
    else
        return fminf(self.maxDifficult,self.minDifficult + (self.maxDifficult-self.minDifficult)*(self.currentDistance/self.targetDistance));
}

-(void)dealloc{
    self.nextLevel = nil;
    [super dealloc];
}
@end


#pragma mark -
#pragma mark GameState
#pragma mark -

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
    BOOL isDailyRewardAvailable;
    BOOL isPlayer1Died;
    BOOL isPlayer2Died;
    BOOL isSecondChanceRefusedForPlayer1;
    BOOL isSecondChanceRefusedForPlayer2;
    BOOL isSambaAvailable;
    BOOL isCrazyModeEnabled;
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
    NSArray *missions;
    NSArray *levels;
}

-(id)initWithNumPlayers:(int)numPlayers{
    self = [super init];
    if (self){
        GameManager *manager = [GameManager sharedManager];
        
        minIntervalPowerUpWeapon = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"weapon"]objectForKey:@"min"]intValue];
        maxIntervalPowerUpWeapon = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"weapon"]objectForKey:@"max"]intValue];
        minIntervalBonusLetter = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"bonus"]objectForKey:@"min"]intValue];
        maxIntervalBonusLetter = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"bonus"]objectForKey:@"max"]intValue];
        minIntervalSpecialCoin = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"gem"]objectForKey:@"min"]intValue];
        maxIntervalSpecialCoin = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"gem"]objectForKey:@"max"]intValue];
        minIntervalPokerChip = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"poker"]objectForKey:@"min"]intValue];
        maxIntervalPokerChip = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"poker"]objectForKey:@"max"]intValue];
        minIntervalHealthPack = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"heal"]objectForKey:@"min"]intValue];
        maxIntervalHealthPack = [[[[[ParamManager sharedManager]objectForKey:@"Box"]objectForKey:@"heal"]objectForKey:@"max"]intValue];
        
        isDailyRewardAvailable = [DailyReward nextBonusLetter] != GAME_ENTITY_TYPE_NONE;
        isPlayer1Died = NO;
        isPlayer2Died = numPlayers == 1 ? YES : NO;
        isSecondChanceRefusedForPlayer1 = NO;
        isSecondChanceRefusedForPlayer2 = numPlayers == 1 ? YES : NO;
        isCrazyModeEnabled = NO;
        avatarPlayer1 = [[AvatarConfig avatarWithType:[manager avatarFirstPlayer]]retain];
        avatarPlayer2 = numPlayers == 2 ? [[AvatarConfig avatarWithType:[manager avatarSecondPlayer]]retain] : nil;
        vehiclePlayer1 = [[VehicleConfig vehicleWithType:[manager vehicleFirstPlayer]]retain];
        vehiclePlayer2 = numPlayers == 2 ? [[VehicleConfig vehicleWithType:[manager vehicleSecondPlayer]]retain] : nil;
        isSambaAvailable = [vehiclePlayer1 type] == VEHICLE_TYPE_CARRIAGE || [vehiclePlayer2 type] == VEHICLE_TYPE_CARRIAGE;
        weaponPlayer1 = nil;
        weaponPlayer2 = nil;
        cardHand = nil;
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
        self.delegate = nil;
        _numPlayers = numPlayers;
        _distance = 0.0f;
        self.time = 0.0f;
        self.maxLife = 3;
        self.lifePlayer1 = self.maxLife;
        self.lifePlayer2 = numPlayers == 2 ? self.maxLife : 0;
        self.livesLost = 0;
        self.coins = 0;
        self.coinsSpent = 0;
        self.coinsWhileDead = 0;
        self.specialCoins = 0;
        self.specialCoinsSpent = 0;
        self.pokerChipsTotal = 0;
        self.pokerChipsRemaining = 0;
        self.points = 0;
        self.jumps = 0;
        self.dives = 0;
        self.rings = 0;
        self.fishes = 0;
        self.minesJumped = 0;
        self.minesDived = 0;
        self.playerShotHits = 0;
        self.playerShotMisses = 0;
        self.enemyShotHits = 0;
        self.enemyShotMisses = 0;
        self.cardsDodged = 0;
        self.reborns = 0;
        self.pokerReborns = 0;
        self.friendBuoysDestroyed = 0;
        self.powerUpsUsed = 0;
        self.itemBoxesDestroyed = 0;
        self.minesDestroyedWithGadget = 0;
        self.weaponPowerBonus = 0.0f;
        self.weaponSpeedBonus = 0.0f;
        self.weaponAmmoBonus = 0.0f;
        switch ([DailyReward nextBonusLetter]) {
            case GAME_ENTITY_TYPE_BONUS_LETTER_B: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_NONE; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_O: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_B; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_N: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_O; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_U: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_N; break;
            case GAME_ENTITY_TYPE_BONUS_LETTER_S: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_BONUS_LETTER_U; break;
            default: self.lastBonusLetterPicked = GAME_ENTITY_TYPE_NONE; break;
        }
        self.gadget1 = numPlayers == 1 ? manager.gadget1 : GADGET_TYPE_NONE;
        self.gadget2 = numPlayers == 1 ? manager.gadget2 : GADGET_TYPE_NONE;
        self.gadget3 = numPlayers == 1 ? manager.gadget3 : GADGET_TYPE_NONE;
        self.helicopterTouched = NO;
        self.submarineTouched = NO;
        self.boatTouched = NO;
        self.isGameStarted = NO;
        self.isGameOver = NO;
        self.isKrakenReleased = NO;
        self.isEnemyBitten = NO;
        self.isEnemyElectrocuted = NO;
        self.isTutorial = NO;
        self.isVideoRewarded = NO;
        videoSnapshot = nil;
        videoSnapshotType = VIDEO_SNAPSHOT_TYPE_NONE;
        timeEntities = [[NSMutableArray alloc]initWithCapacity:10];
        kills = [[NSMutableArray alloc]initWithCapacity:100];
        dodges = [[NSMutableArray alloc]initWithCapacity:100];
        powerUpsBuyed = [[NSMutableArray alloc]initWithCapacity:20];
        cards = [[NSMutableArray alloc]initWithCapacity:5];
        scoresToBeat = [[NSMutableArray alloc]initWithCapacity:20];
        levelStatistics = [[NSMutableArray alloc]initWithCapacity:10];
        weaponsUsedForKilling = [[NSMutableArray alloc]initWithCapacity:10];
        hitSources = [[NSMutableArray alloc]initWithCapacity:10];
        loadedWeapons = [[NSMutableArray alloc]initWithCapacity:10];
        MilitarRange *range = [MilitarRange currentRange];
        missions = [[NSArray alloc]initWithArray:[Mission missionsWithRange:[range type]]];
        
        // Bonus
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_POWER_1])    self.weaponPowerBonus += 0.1f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_POWER_2])    self.weaponPowerBonus += 0.15f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_POWER_3])    self.weaponPowerBonus += 0.2f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_SPEED_1])    self.weaponSpeedBonus += 0.1f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_SPEED_2])    self.weaponSpeedBonus += 0.15f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_SPEED_3])    self.weaponSpeedBonus += 0.2f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_AMMO_1])     self.weaponAmmoBonus += 0.1f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_AMMO_2])     self.weaponAmmoBonus += 0.15f;
        if ([self isGadgetEnabled:GADGET_TYPE_MORE_AMMO_3])     self.weaponAmmoBonus += 0.2f;
        
        // Las primeras 5 partidas aumenta la probabilidad de salir ítems
        float beginnerFactor = 0.5f + fminf(manager.gamesPlayed,5) * 0.1f;
        
        nextEnemyDistance = [self randomEntityInterval];
        nextPowerUpWeaponTime = [self randomPowerUpWeaponInterval] * beginnerFactor;
        nextBonusLetterTime = [self randomBonusLetterInterval] * beginnerFactor;
        nextSpecialCoinDistance = [self randomSpecialCoinInterval] * beginnerFactor;
        nextPokerChipTime = [self randomPokerChipInterval] * beginnerFactor;
        nextShamrockDistance = 99999.0f;
        nextMoneyPlaneKills = [self randomMoneyPlaneInterval];
        nextSambaTime = [self randomSambaInterval];
        nextHealthPackTime = [self randomHealthPackInterval];
        nextWaveDistance = [self randomWaveInterval];
        nextCardTime = [self randomCardInterval];
        [self loadWeapon:WEAPON_TYPE_PISTOL forPlayer:1];
        [self loadWeapon:WEAPON_TYPE_PISTOL forPlayer:2];
        
        Level *entry = [[[Level alloc]init]autorelease];
        Level *jungle = [[[Level alloc]init]autorelease];
        Level *jungleTransition = [[[Level alloc]init]autorelease];
        Level *asian = [[[Level alloc]init]autorelease];
        Level *asianTransition = [[[Level alloc]init]autorelease];
        Level *snow = [[[Level alloc]init]autorelease];
        Level *snowTransition = [[[Level alloc]init]autorelease];
        Level *sunset = [[[Level alloc]init]autorelease];
        Level *sunsetTransition = [[[Level alloc]init]autorelease];
        Level *volcano = [[[Level alloc]init]autorelease];
        Level *volcanoTransition = [[[Level alloc]init]autorelease];
        Level *city = [[[Level alloc]init]autorelease];
        Level *cityTransition = [[[Level alloc]init]autorelease];
        Level *tempest = [[[Level alloc]init]autorelease];
        // Para tener hecho un retain de ellos se pone el array
        levels = [[NSArray alloc]initWithObjects:entry,jungle,jungle,jungleTransition,asian,asianTransition,snow,snowTransition,sunset,sunsetTransition,volcano,volcanoTransition,city,cityTransition,tempest,nil];
        
        entry.type = PARALLAX_LAYER_TYPE_ENTRY;
        entry.minDifficult = 0.0f;
        entry.maxDifficult = 0.0f;
        entry.targetDistance = 200.0f;
        entry.targetTime = 6.0f;
        entry.allowEntities = NO;
        entry.nextLevel = jungle;

        jungle.type = PARALLAX_LAYER_TYPE_JUNGLE;
        jungle.number = 1;
        jungle.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"1"]objectForKey:@"minDifficulty"]floatValue];
        // Para el primer nivel incrementa la dificultad en usuarios avanzados
        jungle.minDifficult = fmaxf(jungle.minDifficult*0.1f,fminf(jungle.minDifficult,jungle.minDifficult*0.1f*manager.gamesPlayed));
        jungle.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"1"]objectForKey:@"maxDifficulty"]floatValue];
        jungle.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"1"]objectForKey:@"length"]floatValue];
        jungle.allowEntities = YES;
        jungle.allowEnemyParachutist = YES;
        jungle.allowEnemyJetpack = YES;
        jungle.allowEnemyBoat = YES;
        jungle.allowEnemyMine = NO;
        jungle.allowEnemyHelicopter = NO;
        jungle.allowEnemyBomber = NO;
        jungle.allowEnemySubmarine = NO;
        jungle.allowShamrock = arc4random() % 2 == 0; // Shamrock aleatorio en el primer nivel al 50% probabilidad
        jungle.nextLevel = jungleTransition;
        
        jungleTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        jungleTransition.minDifficult = 0.0f;
        jungleTransition.maxDifficult = 0.0f;
        jungleTransition.targetTime = 6.0f;
        jungleTransition.allowEntities = NO;
        jungleTransition.nextLevel = asian;
        
        asian.type = PARALLAX_LAYER_TYPE_ASIAN;
        asian.number = 2;
        asian.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"2"]objectForKey:@"minDifficulty"]floatValue];
        asian.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"2"]objectForKey:@"maxDifficulty"]floatValue];
        asian.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"2"]objectForKey:@"length"]floatValue];
        asian.allowEntities = YES;
        asian.allowEnemyParachutist = YES;
        asian.allowEnemyJetpack = YES;
        asian.allowEnemyBoat = YES;
        asian.allowEnemyMine = YES;
        asian.allowEnemyHelicopter = NO;
        asian.allowEnemyBomber = YES;
        asian.allowEnemySubmarine = NO;
        asian.nextLevel = asianTransition;
        
        asianTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        asianTransition.minDifficult = 0.0f;
        asianTransition.maxDifficult = 0.0f;
        asianTransition.targetTime = 6.0f;
        asianTransition.allowEntities = NO;
        asianTransition.nextLevel = sunset;
        
        sunset.type = PARALLAX_LAYER_TYPE_SUNSET;
        sunset.number = 3;
        sunset.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"3"]objectForKey:@"minDifficulty"]floatValue];
        sunset.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"3"]objectForKey:@"maxDifficulty"]floatValue];
        sunset.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"3"]objectForKey:@"length"]floatValue];
        sunset.allowEntities = YES;
        sunset.allowEnemyParachutist = YES;
        sunset.allowEnemyJetpack = YES;
        sunset.allowEnemyBoat = YES;
        sunset.allowEnemyMine = YES;
        sunset.allowEnemyHelicopter = YES;
        sunset.allowEnemyBomber = YES;
        sunset.allowEnemySubmarine = YES;
        sunset.nextLevel = sunsetTransition;
        
        sunsetTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        sunsetTransition.minDifficult = 0.0f;
        sunsetTransition.maxDifficult = 0.0f;
        sunsetTransition.targetTime = 6.0f;
        sunsetTransition.allowEntities = NO;
        sunsetTransition.nextLevel = snow;
        
        snow.type = PARALLAX_LAYER_TYPE_SNOW;
        snow.number = 4;
        snow.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"4"]objectForKey:@"minDifficulty"]floatValue];
        snow.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"4"]objectForKey:@"maxDifficulty"]floatValue];
        snow.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"4"]objectForKey:@"length"]floatValue];
        snow.allowEntities = YES;
        snow.allowEnemyParachutist = YES;
        snow.allowEnemyJetpack = YES;
        snow.allowEnemyBoat = YES;
        snow.allowEnemyMine = YES;
        snow.allowEnemyHelicopter = YES;
        snow.allowEnemyBomber = YES;
        snow.allowEnemySubmarine = YES;
        snow.nextLevel = snowTransition;
        
        snowTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        snowTransition.minDifficult = 0.0f;
        snowTransition.maxDifficult = 0.0f;
        snowTransition.targetTime = 6.0f;
        snowTransition.allowEntities = NO;
        snowTransition.nextLevel = volcano;
        
        volcano.type = PARALLAX_LAYER_TYPE_VOLCANO;
        volcano.number = 5;
        volcano.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"5"]objectForKey:@"minDifficulty"]floatValue];
        volcano.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"5"]objectForKey:@"maxDifficulty"]floatValue];
        volcano.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"5"]objectForKey:@"length"]floatValue];
        volcano.allowEntities = YES;
        volcano.allowEnemyParachutist = YES;
        volcano.allowEnemyJetpack = YES;
        volcano.allowEnemyBoat = YES;
        volcano.allowEnemyMine = YES;
        volcano.allowEnemyHelicopter = YES;
        volcano.allowEnemyBomber = YES;
        volcano.allowEnemySubmarine = YES;
        volcano.nextLevel = volcanoTransition;
        
        volcanoTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        volcanoTransition.minDifficult = 0.0f;
        volcanoTransition.maxDifficult = 0.0f;
        volcanoTransition.targetTime = 6.0f;
        volcanoTransition.allowEntities = NO;
        volcanoTransition.nextLevel = city;
        
        city.type = PARALLAX_LAYER_TYPE_CITY;
        city.number = 6;
        city.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"6"]objectForKey:@"minDifficulty"]floatValue];
        city.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"6"]objectForKey:@"maxDifficulty"]floatValue];
        city.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"6"]objectForKey:@"length"]floatValue];
        city.allowEntities = YES;
        city.allowEnemyParachutist = YES;
        city.allowEnemyJetpack = YES;
        city.allowEnemyBoat = YES;
        city.allowEnemyMine = YES;
        city.allowEnemyHelicopter = YES;
        city.allowEnemyBomber = YES;
        city.allowEnemySubmarine = YES;
        city.nextLevel = cityTransition;
        
        cityTransition.type = PARALLAX_LAYER_TYPE_CAVE;
        cityTransition.minDifficult = 0.0f;
        cityTransition.maxDifficult = 0.0f;
        cityTransition.targetTime = 6.0f;
        cityTransition.allowEntities = NO;
        cityTransition.nextLevel = tempest;
        
        tempest.type = PARALLAX_LAYER_TYPE_TEMPEST;
        tempest.number = 7;
        tempest.minDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"7"]objectForKey:@"minDifficulty"]floatValue];
        tempest.maxDifficult = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"7"]objectForKey:@"maxDifficulty"]floatValue];
        tempest.targetDistance = [[[[[ParamManager sharedManager]objectForKey:@"Stages"]objectForKey:@"7"]objectForKey:@"length"]floatValue];
        tempest.allowEntities = YES;
        tempest.allowEnemyParachutist = YES;
        tempest.allowEnemyJetpack = YES;
        tempest.allowEnemyBoat = YES;
        tempest.allowEnemyMine = YES;
        tempest.allowEnemyHelicopter = YES;
        tempest.allowEnemyBomber = YES;
        tempest.allowEnemySubmarine = YES;
        tempest.nextLevel = nil;
        
        level = entry;
        
        [self loadScoresToBeat];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadScoresToBeat) name:FacebookChangedStateNotification object:nil];
    }
    return self;
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self];
    self.delegate = nil;
    [videoSnapshot release];
    [avatarPlayer1 release];
    [avatarPlayer2 release];
    [vehiclePlayer1 release];
    [vehiclePlayer2 release];
    [weaponPlayer1 release];
    [weaponPlayer2 release];
    [cardHand release];
    [timeEntities release];
    [kills release];
    [dodges release];
    [powerUpsBuyed release];
    [levels release];
    [levelStatistics release];
    [weaponsUsedForKilling release];
    [missions release];
    [cards release];
    [scoresToBeat release];
    [hitSources release];
    [loadedWeapons release];
    [super dealloc];
}

-(void)enableCrazyMode{
    isCrazyModeEnabled = YES;
    for (Level *l in levels){
        l.minDifficult = 1.0f;
        l.maxDifficult = 1.0f;
    }
}

-(BOOL)isCrazyModeEnabled{
    return isCrazyModeEnabled;
}

-(BOOL)isPlayerDead:(int)numPlayer{
    return numPlayer == 1 ? isPlayer1Died : isPlayer2Died;
}

-(void)setDied:(BOOL)died forPlayer:(int)numPlayer{
    if (numPlayer == 1) isPlayer1Died = died;
    else                isPlayer2Died = died;
}

-(BOOL)isSecondChanceRefusedForPlayer:(int)numPlayer{
    return numPlayer == 1 ? isSecondChanceRefusedForPlayer1 : isSecondChanceRefusedForPlayer2;
}

-(void)setSecondChanceRefused:(BOOL)refused forPlayer:(int)numPlayer{
    if (numPlayer == 1) isSecondChanceRefusedForPlayer1 = refused;
    else                isSecondChanceRefusedForPlayer2 = refused;
}

-(BOOL)areAllPlayersDied{
    return isPlayer1Died && isPlayer2Died;
}

-(void)loadScoresToBeat{
    // La puntuación del usuario en Facebook y GC no se tiene en cuenta, siempre se
    // tiene en cuenta su puntuación local (comportamiento igual al de Run Sheldon)
    // LAS PUNTUACIONES ESTÁN DE MAYOR A MENOR
    [scoresToBeat removeAllObjects];
    int bestDistance = [GameManager sharedManager].bestDistance;
    
    NSMutableArray *allScores = [NSMutableArray arrayWithCapacity:100];
    
    // Puntuaciones Game Center
    GCManager *gcManager = [GCManager sharedManager];
    if ([gcManager isLocalPlayerAuthenticated]){
        NSArray *gcScores = [gcManager allScores];
        if (gcScores)
            [allScores addObjectsFromArray:gcScores];
    }
    
    // Puntuaciones Facebook
    FacebookManager *fbManager = [FacebookManager sharedManager];
    if ([fbManager isLogged]){
        NSArray *fbScores = [fbManager allScores];
        if (fbScores)
            [allScores addObjectsFromArray:fbScores];
    }
    
    // Añade las puntuaciones una a una (sólo aquellas mayores al récord actual) en orden
    // de mayor a menor
    for (UserScore *newScore in allScores){
        if (![newScore isMe]){
            if ([newScore score] > bestDistance){
                BOOL lastEntry = YES;
                int index = 0;
                for (UserScore *s in scoresToBeat){
                    if ([newScore score] == [s score]){
                        // Puntuaciones iguales NO
                        lastEntry = NO;
                        break;
                    }
                    else if ([newScore score] > [s score]){
                        lastEntry = NO;
                        [scoresToBeat insertObject:newScore atIndex:index];
                        break;
                    }
                    index++;
                }
                // Lo añade al final si es necesario
                if (lastEntry)
                    [scoresToBeat addObject:newScore];
            }
        }
    }
    
    // Por último añade el récord del jugador el primero
    if (bestDistance > 0){
        // Prueba primero con GC, si no con FB que siempre devuelve algo
        UserScore *myScore = [gcManager myScoreWithCustomScore:bestDistance];
        if (!myScore)
            myScore = [fbManager myScoreWithCustomScore:bestDistance];
        [scoresToBeat addObject:myScore];
    }
}

-(float)randomEntityInterval{
    float interval = (MIN_INTERVAL_ENTITY + arc4random() % (MAX_INTERVAL_ENTITY - MIN_INTERVAL_ENTITY));
    if (self.numPlayers == 2)
        interval *= 0.65f;
    return interval;
}

-(float)randomPowerUpWeaponInterval{
    return (minIntervalPowerUpWeapon + arc4random() % (maxIntervalPowerUpWeapon - minIntervalPowerUpWeapon));
}

-(float)randomBonusLetterInterval{
    return (minIntervalBonusLetter + arc4random() % (maxIntervalBonusLetter - minIntervalBonusLetter));
}

-(float)randomCardInterval{
    return (MIN_INTERVAL_CARD + arc4random() % (MAX_INTERVAL_CARD - MIN_INTERVAL_CARD));
}

-(float)randomSpecialCoinInterval{
    return (minIntervalSpecialCoin + arc4random() % (maxIntervalSpecialCoin - minIntervalSpecialCoin));
}

-(float)randomPokerChipInterval{
    return (minIntervalPokerChip + arc4random() % (maxIntervalPokerChip - minIntervalPokerChip));
}

-(float)randomHealthPackInterval{
    return (minIntervalHealthPack + arc4random() % (maxIntervalHealthPack - minIntervalHealthPack));
}

-(float)randomWaveInterval{
    return (MIN_INTERVAL_WAVE + arc4random() % (MAX_INTERVAL_WAVE - MIN_INTERVAL_WAVE));
}

-(float)randomMoneyPlaneInterval{
    return (MIN_INTERVAL_MONEY_PLANE + arc4random() % (MAX_INTERVAL_MONEY_PLANE - MIN_INTERVAL_MONEY_PLANE));
}

-(float)randomSambaInterval{
    return (MIN_INTERVAL_SAMBA + arc4random() % (MAX_INTERVAL_SAMBA - MIN_INTERVAL_SAMBA));
}

-(GameEntityType)randomEnemy{
    int random = arc4random() % 10;
    if (random <= 2 && level.allowEnemyParachutist) // 30%
        return GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
    else if (random <= 4 && level.allowEnemyJetpack) // 20%
        return GAME_ENTITY_TYPE_ENEMY_JETPACK;
    else{ // 50%
        BOOL canBeHelicopter = level.allowEnemyHelicopter;
        BOOL canBeBomber = level.allowEnemyBomber;
        BOOL canBeSubmarine = level.allowEnemySubmarine;
        BOOL canBeBoat = level.allowEnemyBoat;
        BOOL canBeMine = level.allowEnemyMine;
        
        GameLayer *gameLayer = [self gameLayer];
        NSArray *entities = [gameLayer entities];
        for (GameEntity *e in entities){
            switch (e.type) {
                case GAME_ENTITY_TYPE_ENEMY_HELICOPTER:
                case GAME_ENTITY_TYPE_ENEMY_BOMBER:
                    canBeHelicopter = NO;
                    canBeBomber = NO;
                    break;
                case GAME_ENTITY_TYPE_ENEMY_SUBMARINE:
                    canBeSubmarine = NO;
                    break;
                case GAME_ENTITY_TYPE_ENEMY_BOAT:
                case GAME_ENTITY_TYPE_ENEMY_MINE:
                    canBeBoat = NO;
                    canBeMine = NO;
                    break;
                default:
                    break;
            }
        }
        
        NSMutableArray *types = [NSMutableArray arrayWithCapacity:4];
        if (canBeHelicopter)    [types addObject:[NSNumber numberWithInt:GAME_ENTITY_TYPE_ENEMY_HELICOPTER]];
        if (canBeBomber)        [types addObject:[NSNumber numberWithInt:GAME_ENTITY_TYPE_ENEMY_BOMBER]];
        if (canBeSubmarine)     [types addObject:[NSNumber numberWithInt:GAME_ENTITY_TYPE_ENEMY_SUBMARINE]];
        if (canBeBoat)          [types addObject:[NSNumber numberWithInt:GAME_ENTITY_TYPE_ENEMY_BOAT]];
        if (canBeMine)          [types addObject:[NSNumber numberWithInt:GAME_ENTITY_TYPE_ENEMY_MINE]];
        
        if ([types count] == 0)
            return GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
        else{
            int random = arc4random() % [types count];
            return (GameEntityType)[[types objectAtIndex:random]intValue];
        }
    }
}

-(GameEntityType)randomPowerUpWeapon{
    NSArray *weapons = nil;
    int player = (arc4random() % _numPlayers) + 1;
    if (player == 1)    weapons = [avatarPlayer1 weapons];
    else                weapons = [avatarPlayer2 weapons];
    WeaponConfig *weapon = [weapons objectAtIndex:arc4random()%[weapons count]];
    return [weapon powerUpRelationship];
}

-(BOOL)existEnemiesOnGame{
    return [[[self gameLayer]enemies]count] > 0 || [timeEntities count] > 0;
}

-(BOOL)existGunsOnGame{
    return [[[self gameLayer]guns]count] > 0;
}

-(BOOL)existsMoneyPlaneOnGame{
    GameLayer *gameLayer = [self gameLayer];
    NSArray *entities = [gameLayer entities];
    BOOL found = NO;
    for (GameEntity *e in entities){
        if (e.type == GAME_ENTITY_TYPE_MONEY_PLANE){
            found = YES;
            break;
        }
    }
    return found;
}

-(BOOL)existsItemBoxOnGame{
    GameLayer *gameLayer = [self gameLayer];
    NSArray *entities = [gameLayer entities];
    BOOL found = NO;
    for (GameEntity *e in entities){
        if (e.type == GAME_ENTITY_TYPE_ITEM_BOX){
            found = YES;
            break;
        }
    }
    return found;
}

-(GameScene*)gameScene{
    return [GameScene runningScene];
}

-(GameLayer*)gameLayer{
    return [[self gameScene]gameLayer];
}

-(void)update:(ccTime)dt{
    level.currentTime += dt;
    timeWithoutHit += dt;
    if ([[self gameLayer]isPressingRunningButtonForPlayer:1])
        timeAccelerating += dt;
    else
        timeAccelerating = 0.0f;
    
    // Esto se hace para que no frene de pronto por haber sido golpeado por un enemigo
    if (targetSpeed > 0.0f){
        if (speed > targetSpeed)
            speed *= 0.99f;
        else
            targetSpeed = 0.0f;
    }
    else
        speed = fminf(MAX_SPEED, fmaxf(MIN_SPEED, speed + dt*[self difficultyFactor]));
    
    comboTime += dt;
    if (comboTime >= 1.25f)
        combo = 0;
    
    [self updateMissions];
    
    if (!level.allowEntities){
        // Ya que no permite entidades aumenta el tiempo a la que deben aparecer las entidades
        nextPowerUpWeaponTime += dt;
        nextBonusLetterTime += dt;
        nextCardTime += dt;
        nextPokerChipTime += dt;
        nextHealthPackTime += dt;
        nextSambaTime += dt;
    }
}

-(void)updateMissions{
    for (Mission *m in missions)
        [m updateWithGameState:self];
}

-(void)addDistance:(float)distance{
    if (!self.isTutorial){
        _distance += distance;
        _distance = fminf(99999.0f,_distance);
        level.currentDistance += distance;
        
        if (!level.allowEntities){
            // Ya que no permite entidades aumenta la distancia a la que deben aparecer las entidades
            nextEnemyDistance = self.distance + [self randomEntityInterval];
            nextWaveDistance = self.distance + [self randomWaveInterval];
        }
    }
}

-(AvatarConfig*)avatarForPlayer:(int)player{
    return player == 1 ? avatarPlayer1 : avatarPlayer2;
}

-(VehicleConfig*)vehicleForPlayer:(int)player{
    return player == 1 ? vehiclePlayer1 : vehiclePlayer2;
}

-(GameEntityType)nextEnemy{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([timeEntities count] > 0){
        TimeEntity *candidate = nil;
        for (TimeEntity *t in timeEntities){
            if (self.distance >= t.distance){
                candidate = t;
                break;
            }
        }
        if (candidate){
            GameEntityType type = candidate.type;
            [timeEntities removeObject:candidate];
            return type;
        }
    }
    else if ([level canCreateEntity]){
        if ([self isGadgetEnabled:GADGET_TYPE_MONEY_PLANE] && [self numKills] >= nextMoneyPlaneKills){
            if (![self existEnemiesOnGame]){
                type = GAME_ENTITY_TYPE_MONEY_PLANE;
                nextMoneyPlaneKills = [self numKills] + [self randomMoneyPlaneInterval];
            }
        }
        else if (self.distance >= nextEnemyDistance && ![self existsMoneyPlaneOnGame]){
            nextEnemyDistance = self.distance + [self randomEntityInterval];
            
            GameEntityType nextType = [self randomEnemy];
            
            if (nextType == GAME_ENTITY_TYPE_ENEMY_PARACHUTIST){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,[self difficultyFactor]*10)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 20.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = [[[TimeEntity alloc]init]autorelease];
                    entity.type = GAME_ENTITY_TYPE_ENEMY_PARACHUTIST;
                    entity.distance = self.distance + i*incdistance;
                    [timeEntities addObject:entity];
                }
            }
            else if (nextType == GAME_ENTITY_TYPE_ENEMY_JETPACK){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,[self difficultyFactor]*5)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 1.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = [[[TimeEntity alloc]init]autorelease];
                    entity.type = GAME_ENTITY_TYPE_ENEMY_JETPACK;
                    entity.distance = self.distance + i*incdistance;
                    [timeEntities addObject:entity];
                }
            }
            else if (nextType == GAME_ENTITY_TYPE_ENEMY_SUBMARINE){
                int maxEntities = (int)(ceilf(fmaxf(1.0f,[self difficultyFactor]*4.0f)));
                int numEntities = 1 + (arc4random() % maxEntities);
                float incdistance = 50.0f;
                for (int i = 0; i < numEntities; i++){
                    TimeEntity *entity = [[[TimeEntity alloc]init]autorelease];
                    entity.type = GAME_ENTITY_TYPE_ENEMY_SUBMARINE;
                    entity.distance = self.distance + i*incdistance;
                    [timeEntities addObject:entity];
                }
            }
            else
                type = nextType;
        }
    }

    return type;
}

-(GameEntityType)nextItemBox{
    GameEntityType itemType = GAME_ENTITY_TYPE_NONE;
    itemType = [self nextPowerUp];
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = [self nextBonusLetter];
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = [self nextCard];
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = [self nextSpecialCoin];
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = [self nextPokerChip];
    if (itemType == GAME_ENTITY_TYPE_NONE)
        itemType = [self nextShamrock];
    return itemType;
}

-(void)resetWeaponItemBoxTimer{
    nextPowerUpWeaponTime = self.time + [self randomPowerUpWeaponInterval];
}

-(GameEntityType)nextPowerUp{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([level canCreateEntity]){
        if (self.time >= nextPowerUpWeaponTime){
            if (![self existsItemBoxOnGame]){
                type = [self randomPowerUpWeapon];
                nextPowerUpWeaponTime = self.time + [self randomPowerUpWeaponInterval];
            }
        }
        else if (self.time >= nextHealthPackTime){
            if (![self existsItemBoxOnGame]){
                if (self.lifePlayer1 < self.maxLife || self.lifePlayer2 < self.maxLife)
                    type = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                nextHealthPackTime = self.time + [self randomHealthPackInterval];
            }
        }
        else if (isSambaAvailable && self.time >= nextSambaTime){
            if (![self existsItemBoxOnGame]){
                type = GAME_ENTITY_TYPE_POWER_UP_SAMBA;
                nextSambaTime = self.time + [self randomSambaInterval];
            }
        }
    }
    
    return type;
}

-(GameEntityType)nextBonusLetter{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if (isDailyRewardAvailable && [level canCreateEntity]){
        if (self.time >= nextBonusLetterTime){
            if (![self existsItemBoxOnGame]){
                switch (self.lastBonusLetterPicked) {
                    case GAME_ENTITY_TYPE_NONE:             type = GAME_ENTITY_TYPE_BONUS_LETTER_B; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_B:   type = GAME_ENTITY_TYPE_BONUS_LETTER_O; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_O:   type = GAME_ENTITY_TYPE_BONUS_LETTER_N; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_N:   type = GAME_ENTITY_TYPE_BONUS_LETTER_U; break;
                    case GAME_ENTITY_TYPE_BONUS_LETTER_U:   type = GAME_ENTITY_TYPE_BONUS_LETTER_S; break;
                    default:break;
                }
                nextBonusLetterTime = self.time + [self randomBonusLetterInterval];
            }
        }
    }
    
    return type;
}

-(GameEntityType)nextCard{
    // Ya no se reparten cartas durante el juego
    return GAME_ENTITY_TYPE_NONE;
    /*
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([level canCreateEntity]){
        if ([[self cards]count] < 5 && self.time >= nextCardTime){
            if (![self existsItemBoxOnGame]){
                type = GAME_ENTITY_TYPE_CARD;
                nextCardTime = self.time + [self randomCardInterval];
            }
        }
    }
    
    return type;
     */
}

-(GameEntityType)nextSpecialCoin{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([level canCreateEntity]){
        if (self.distance >= nextSpecialCoinDistance){
            if (![self existsItemBoxOnGame]){
                type = GAME_ENTITY_TYPE_SPECIAL_COIN;
                nextSpecialCoinDistance = self.distance + [self randomSpecialCoinInterval];
            }
        }
    }
    
    return type;
}

-(GameEntityType)nextPokerChip{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([level canCreateEntity]){
        if (self.time >= nextPokerChipTime){
            if (![self existsItemBoxOnGame]){
                type = GAME_ENTITY_TYPE_POKER_CHIP;
                nextPokerChipTime = self.time + [self randomPokerChipInterval];
            }
        }
    }
    
    return type;
}

-(GameEntityType)nextShamrock{
    GameEntityType type = GAME_ENTITY_TYPE_NONE;
    
    if ([level canCreateEntity] && level.allowShamrock){
        if (self.distance >= nextShamrockDistance){
            if (![self existsItemBoxOnGame]){
                type = GAME_ENTITY_TYPE_SHAMROCK;
                // Solo 1 shamrock por nivel
                level.allowShamrock = NO;
                nextShamrockDistance = 99999;
            }
        }
    }
    
    return type;
}

-(UserScore*)nextScoreToBeat{
    UserScore *score = nil;
    if ([scoresToBeat count] > 0)
        score = [scoresToBeat lastObject];
    return score;
}

-(UserScore*)nextScoreBeaten{
    UserScore *score = nil;
    if ([scoresToBeat count] > 0){
        UserScore *next = [scoresToBeat lastObject];
        if (next.score < self.distance){
            score = next;
            // Esto para evitar que el retainCount sea 0 en cuanto se quite del array
            [[score retain]autorelease];
            [scoresToBeat removeLastObject];
        }
    }
    return score;
}

-(ParallaxLayerType)currentLevel{
    return level.type;
}

-(int)currentLevelNumber{
    return level.number;
}

-(float)currentLevelTargetTime{
    return level.targetTime;
}

-(ParallaxLayerType)nextLevel{
    ParallaxLayerType type = PARALLAX_LAYER_TYPE_NONE;
    if (!self.isTutorial && level.nextLevel != nil){
        if ((level.targetTime > 0.0f && level.currentTime > level.targetTime) || (level.targetDistance > 0.0f && level.currentDistance > level.targetDistance)){
            if (    ![self existEnemiesOnGame]
                    && ![self existGunsOnGame]
                    && ![self existsMoneyPlaneOnGame]
                    && ![self existsItemBoxOnGame]
                    && ((self.lifePlayer1 > 0 && !isPlayer1Died) || (self.lifePlayer2 > 0 && !isPlayer2Died))){
                // Estadísticas en el caso de que sea un nivel jugable
                if (level.number > 0){
                    GameEntityType powerUpBuyedInLevel = GAME_ENTITY_TYPE_NONE;
                    int totalPowerUpsBuyed = (int)[powerUpsBuyed count];
                    if (totalPowerUpsBuyed > statisticsLevelLastNumWeaponsBuyed){
                        for (int i = statisticsLevelLastNumWeaponsBuyed; i < totalPowerUpsBuyed; i++){
                            GameEntityType type = (GameEntityType)[[powerUpsBuyed objectAtIndex:i]intValue];
                            if (type != GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                                powerUpBuyedInLevel = type;
                                break;
                            }
                        }
                    }
                    
                    int numRevivesBuyed = 0;
                    if (totalPowerUpsBuyed > statisticsLevelLastNumWeaponsBuyed){
                        for (int i = statisticsLevelLastNumWeaponsBuyed; i < totalPowerUpsBuyed; i++){
                            GameEntityType type = (GameEntityType)[[powerUpsBuyed objectAtIndex:i]intValue];
                            if (type == GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK)
                                numRevivesBuyed++;
                        }
                    }
                    
                    int powerUpsUsedInLevel = self.powerUpsUsed - statisticsLevelLastNumPowerUpsUsed;
                    int livesLostInLevel = self.livesLost - statisticsLevelLastNumLivesLost;
                    int killsInLevel = [self numKills] - statisticsLevelLastNumKills;
                    int coinsCollectedInLevel = self.coins - statisticsLevelLastNumCoinsCollected;
                    int coinsSpentInLevel = self.coinsSpent - statisticsLevelLastNumCoinsSpent;
                    int specialCoinsCollectedInLevel = self.specialCoins - statisticsLevelLastNumSpecialCoinsCollected;
                    int specialCoinsSpentInLevel = self.specialCoinsSpent - statisticsLevelLastNumSpecialCoinsSpent;
                    int pokerChipsInLevel = self.pokerChipsTotal - statisticsLevelLastNumPokerChipsCollected;
                    int distanceInLevel = (int)(self.distance - statisticsLevelLastDistance);
                    int durationInLevel = (int)(self.time - statisticsLevelLastTime);
                    
                    LevelStatistics *levelStats = [[[LevelStatistics alloc]init]autorelease];
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
                    [levelStatistics addObject:levelStats];
                    
                    statisticsLevelLastNumWeaponsBuyed = (int)[powerUpsBuyed count];
                    statisticsLevelLastNumPowerUpsUsed = self.powerUpsUsed;
                    statisticsLevelLastNumLivesLost = self.livesLost;
                    statisticsLevelLastNumKills = [self numKills];
                    statisticsLevelLastNumCoinsCollected = self.coins;
                    statisticsLevelLastNumCoinsSpent = self.coinsSpent;
                    statisticsLevelLastNumSpecialCoinsCollected = self.specialCoins;
                    statisticsLevelLastNumSpecialCoinsSpent = self.specialCoinsSpent;
                    statisticsLevelLastNumPokerChipsCollected = self.pokerChipsTotal;
                    statisticsLevelLastDistance = (int)self.distance;
                    statisticsLevelLastTime = (int)self.time;
                }
                
                // Cambia de nivel
                level = level.nextLevel;
                type = level.type;
                
                // Nueva distancia del shamrock
                if (level.allowShamrock && level.targetDistance > 0)
                    nextShamrockDistance = self.distance + level.targetDistance * ((arc4random()%10)+1) * 0.09f;
            }
        }
    }
    
    // Sustituido en addDistance y update
    /*
    if (type != PARALLAX_LAYER_TYPE_NONE){
        // Reinicia los contadores
        nextEnemyDistance = self.distance + [self randomEntityInterval];
        nextPowerUpWeaponTime = self.time + [self randomPowerUpWeaponInterval];
        nextBonusLetterTime = self.time + [self randomBonusLetterInterval];
        nextCardTime = self.time + [self randomCardInterval];
        nextHealthPackTime = self.time + [self randomHealthPackInterval];
        nextWaveDistance = self.distance + [self randomWaveInterval];
    }
    */
    
    return type;
}

-(ParallaxLayerType)nextLevelToPreload{
    if (level.nextLevel)
        return level.nextLevel.type;
    else
        return PARALLAX_LAYER_TYPE_NONE;
}

-(float)nextWave{
    /*if (self.distance >= nextWaveDistance){
        nextWavedistance = self.distance + [self randomWaveInterval];
        
        // No puede haber una ola si hay un bote
        GameLayer *gameLayer = [self gameLayer];
        NSArray *entities = [gameLayer entities];
        BOOL canWave = YES;
        for (GameEntity *e in entities){
            if (e.type == GAME_ENTITY_TYPE_ENEMY_BOAT){
                canWave = NO;
                break;
            }
        }

        if (canWave)
            return 3.0f;
    }*/
    
    return 0.0f;
}

-(float)speed{
    return speed;
}

-(float)difficultyFactor{
    float hitFactor = fminf(1.0f,0.5f + (0.5f*timeWithoutHit/RELAX_TIME_AFTER_HIT));
    return [level difficultyFactor] * hitFactor;
}

-(void)didHitPlayer:(int)numPlayer withSource:(GameEntityType)source{
    [hitSources addObject:[NSNumber numberWithInt:source]];
    timeWithoutHit = 0.0f;
    if (!isCrazyModeEnabled)
        targetSpeed = fmaxf(MIN_SPEED, speed*0.5f);
}

-(int)numHits{
    return (int)[hitSources count];
}

-(int)numHitsWithSource:(GameEntityType)source{
    int numHits = 0;
    for (NSNumber *n in hitSources)
        if ([n intValue] == source)
            numHits++;
    return numHits;
}

-(BOOL)isGadgetEnabled:(GadgetType)type{
    return self.gadget1 == type || self.gadget2 == type || self.gadget3 == type;
}

-(int)numGadgetsEnabled{
    int count = 0;
    if (self.gadget1 != GADGET_TYPE_NONE) count++;
    if (self.gadget2 != GADGET_TYPE_NONE) count++;
    if (self.gadget3 != GADGET_TYPE_NONE) count++;
    return count;
}

-(NSArray*)missions{
    return missions;
}

-(void)addKill:(Kill*)kill{
    [kills addObject:kill];
    if (kill.gun != GUN_TYPE_NONE){
        comboTime = 0.0f;
        combo++;
        if (combo > 1){
            maxCombo = MAX(maxCombo,combo);
            [self.delegate didPerformCombo:combo withKill:kill];
        }
    }
    
    // Como no hay una conversión directa entre GunType y WeaponType presuponemos que el arma utilizada
    // para matar es la que tiene en este momento
    NSNumber *obj = [NSNumber numberWithInt:weaponPlayer1.type];
    if ([weaponsUsedForKilling indexOfObject:obj] == NSNotFound)
        [weaponsUsedForKilling addObject:obj];
    
    // Logros
    [Achievement didKill:self];
    if (kill.entity == GAME_ENTITY_TYPE_ENEMY_SUBMARINE)
        [Achievement didDestroySub:self];
}

-(int)numKills{
    return (int)[kills count];
}

-(int)numKillsWithEntityType:(GameEntityType)type{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.entity == type)
            count++;
    return count;
}

-(int)numKillsWithGunType:(GunType)type{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.gun == type)
            count++;
    return count;
}

-(int)numKillsInBulletTime{
    int count = 0;
    for (Kill *kill in kills)
        if (kill.bulletTime)
            count++;
    return count;
}

-(int)numDifferentWeaponsUsedForKilling{
    return (int)[weaponsUsedForKilling count];
}

-(void)addDodge:(Dodge*)dodge{
    [dodges addObject:dodge];
    
    // Logros
    [Achievement didDodge:self];
}

-(int)numDodges{
    return (int)[dodges count];
}

-(int)numDodgesWithEntityType:(GameEntityType)type{
    int count = 0;
    for (Dodge *dodge in dodges)
        if (dodge.entity == type)
            count++;
    return count;
}

-(WeaponConfig*)weaponForPlayer:(int)player{
    return player == 1 ? weaponPlayer1 : weaponPlayer2;
}

-(void)loadWeapon:(WeaponType)type forPlayer:(int)numPlayer{
    if (numPlayer == 1){
        [weaponPlayer1 release];
        weaponPlayer1 = [[WeaponConfig weaponWithType:type]retain];
        self.ammoPlayer1 = [weaponPlayer1 maxAmmo];
        if (self.ammoPlayer1 != INFINITE_AMMO)
            self.ammoPlayer1 = (int)(self.ammoPlayer1 * (1.0f + self.weaponAmmoBonus));
        
        // Logros
        [Achievement didLoadWeapon:self];
    }
    else{
        [weaponPlayer2 release];
        weaponPlayer2 = [[WeaponConfig weaponWithType:type]retain];
        self.ammoPlayer2 = [weaponPlayer2 maxAmmo];
        if (self.ammoPlayer2 != INFINITE_AMMO)
            self.ammoPlayer2 = (int)(self.ammoPlayer2 * (1.0f + self.weaponAmmoBonus));
    }
    [loadedWeapons addObject:[NSNumber numberWithInt:type]];
}

-(int)numLoadedWeapons{
    return (int)[loadedWeapons count];
}

-(int)numLoadedWeaponsWithType:(WeaponType)type{
    int count = 0;
    for (NSNumber *n in loadedWeapons)
        if ([n intValue] == type)
            count++;
    return count;
}

-(int)combo{
    return combo;
}

-(int)maxCombo{
    return maxCombo;
}

-(void)addPowerUpBuyed:(GameEntityType)type{
    [powerUpsBuyed addObject:[NSNumber numberWithInt:type]];
}

-(int)numPowerUpsBuyed{
    return (int)[powerUpsBuyed count];
}

-(int)numPowerUpsBuyedWithType:(GameEntityType)type{
    int count = 0;
    for (NSNumber *n in powerUpsBuyed)
        if ([n intValue] == type)
            count++;
    return count;
}

-(GameEntityType)powerUpBuyedAtIndex:(int)index{
    return (GameEntityType)[[powerUpsBuyed objectAtIndex:index]intValue];
}

-(NSArray*)differentTypesOfPowerUpsBuyed{
    NSMutableArray *differentPowerUps = [NSMutableArray arrayWithCapacity:10];
    for (NSNumber *n in powerUpsBuyed)
        if ([differentPowerUps indexOfObject:n] == NSNotFound)
            [differentPowerUps addObject:n];
    return differentPowerUps;
}

-(PokerCardConfig*)randomCard{
    PokerCardConfig *config = nil;
    while(true){
        NSArray *array = [self cards];
        
        BOOL canBeJoker = [self isGadgetEnabled:GADGET_TYPE_JOKER];
        int numJokers = 0;
        for (PokerCardConfig *c in array){
            if (c.suit == POKER_CARD_SUIT_TYPE_JOKER)
                numJokers++;
        }
        if (numJokers >= 3)
            canBeJoker = NO;
        
        int cardNumber = (arc4random()%13)+1;
        int cardIndex = arc4random()%(canBeJoker ? 5 : 4);
        PokerCardSuitType cardType = POKER_CARD_SUIT_TYPE_CLUB;
        switch (cardIndex) {
            case 0: cardType = POKER_CARD_SUIT_TYPE_CLUB; break;
            case 1: cardType = POKER_CARD_SUIT_TYPE_DIAMOND; break;
            case 2: cardType = POKER_CARD_SUIT_TYPE_HEART; break;
            case 3: cardType = POKER_CARD_SUIT_TYPE_SPADE; break;
            case 4: cardType = POKER_CARD_SUIT_TYPE_JOKER; break;
            default:break;
        }
        
        BOOL found = NO;
        if (cardType != POKER_CARD_SUIT_TYPE_JOKER){
            for (PokerCardConfig *c in array){
                if (c.suit == cardType && c.number == cardNumber){
                    found = YES;
                    break;
                }
            }
        }
        else
            cardNumber = 0;
        
        if (!found){
            config = [PokerCardConfig configWithSuit:cardType number:cardNumber];
            break;
        }
    }
    return config;
}

-(void)addCard:(PokerCardConfig*)config{
    [cards addObject:config];
    [cardHand release];
    cardHand = [[PokerCardHand handWithCards:cards]retain];
}

-(void)replaceCardInIndex:(int)index withCard:(PokerCardConfig*)config{
    [cards replaceObjectAtIndex:index withObject:config];
    [cardHand release];
    cardHand = [[PokerCardHand handWithCards:cards]retain];
}

-(NSArray*)cards{
    return cards;
}

-(PokerCardHand*)currentHand{
    return cardHand;
}

-(void)setVideoSnapshot:(UIImage*)snapshot withType:(VideoSnapshotType)type{
    [videoSnapshot release];
    videoSnapshot = [snapshot retain];
    videoSnapshotType = type;
}

-(UIImage*)videoSnapshot{
    return videoSnapshot;
}

-(VideoSnapshotType)videoSnapshotType{
    return videoSnapshotType;
}

-(int)numLevelStatictics{
    return (int)[levelStatistics count];
}

-(LevelStatistics*)levelStaticticsAtIndex:(int)index{
    return [levelStatistics objectAtIndex:index];
}

-(float)timeWithoutHit{
    return timeWithoutHit;
}

-(float)timeAccelerating{
    return timeAccelerating;
}

@end
