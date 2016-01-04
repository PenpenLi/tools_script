//
//  GameState.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 18/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "GameEntity.h"
#import "PowerUp.h"
#import "Gun.h"
#import "Kill.h"
#import "Dodge.h"
#import "AvatarConfig.h"
#import "GadgetConfig.h"
#import "WeaponConfig.h"
#import "VehicleConfig.h"
#import "ParallaxLayer.h"
#import "PokerCard.h"
#import "UserScore.h"

typedef enum{
    VIDEO_SNAPSHOT_TYPE_NONE = 0,
    VIDEO_SNAPSHOT_TYPE_START = 1,
    VIDEO_SNAPSHOT_TYPE_SHOOT = 2,
    VIDEO_SNAPSHOT_TYPE_BULLET = 3,
    VIDEO_SNAPSHOT_TYPE_BUOY = 4
}VideoSnapshotType;

@interface LevelStatistics : NSObject
@property int level;
@property GameEntityType powerUpBuyed;
@property int numPowerUpsUsed;
@property int numLostLives;
@property int numEnemiesKilled;
@property int numCoinsCollected;
@property int numCoinsSpent;
@property int numSpecialCoinsCollected;
@property int numSpecialCoinsSpent;
@property int numPokerChipsCollected;
@property int numRebornsBuyed;
@property int distance;
@property int duration;
@end

@protocol GameStateDelegate <NSObject>
-(void)didPerformCombo:(int)combo withKill:(Kill*)kill;
@end

@interface GameState : NSObject
@property (assign) id<GameStateDelegate> delegate;
@property (readonly) int numPlayers;
@property (readonly) float distance;
@property float time;
@property int lifePlayer1;
@property int lifePlayer2;
@property int ammoPlayer1;
@property int ammoPlayer2;
@property int maxLife;
@property int livesLost;
@property int coins;
@property int coinsSpent;
@property int coinsWhileDead;
@property int specialCoins;
@property int specialCoinsSpent;
@property int pokerChipsTotal;
@property int pokerChipsRemaining;
@property int points;
@property int jumps;
@property int dives;
@property int rings;
@property int fishes;
@property int minesJumped;
@property int minesDived;
@property int playerShotHits;
@property int playerShotMisses;
@property int enemyShotHits;
@property int enemyShotMisses;
@property int cardsDodged;
@property int reborns;
@property int pokerReborns;
@property int friendBuoysDestroyed;
@property int powerUpsUsed;
@property int itemBoxesDestroyed;
@property int minesDestroyedWithGadget;
@property float weaponPowerBonus;
@property float weaponSpeedBonus;
@property float weaponAmmoBonus;
@property BOOL helicopterTouched;
@property BOOL submarineTouched;
@property BOOL boatTouched;
@property BOOL isGameStarted;
@property BOOL isGameOver;
@property BOOL isKrakenReleased;
@property BOOL isEnemyBitten;
@property BOOL isEnemyElectrocuted;
@property BOOL isTutorial;
@property BOOL isVideoRewarded;
@property GameEntityType lastBonusLetterPicked;
@property GadgetType gadget1;
@property GadgetType gadget2;
@property GadgetType gadget3;

-(id)initWithNumPlayers:(int)numPlayers;
-(void)enableCrazyMode;
-(BOOL)isCrazyModeEnabled;

-(BOOL)isPlayerDead:(int)numPlayer;
-(void)setDied:(BOOL)died forPlayer:(int)numPlayer;

-(BOOL)isSecondChanceRefusedForPlayer:(int)numPlayer;
-(void)setSecondChanceRefused:(BOOL)refused forPlayer:(int)numPlayer;

-(BOOL)areAllPlayersDied;

-(void)update:(ccTime)dt;
-(void)addDistance:(float)distance;

-(AvatarConfig*)avatarForPlayer:(int)player;
-(VehicleConfig*)vehicleForPlayer:(int)player;
-(WeaponConfig*)weaponForPlayer:(int)player;
-(void)loadWeapon:(WeaponType)type forPlayer:(int)numPlayer;
-(int)numLoadedWeapons;
-(int)numLoadedWeaponsWithType:(WeaponType)type;

-(GameEntityType)nextEnemy;
-(GameEntityType)nextItemBox;
-(void)resetWeaponItemBoxTimer;
-(UserScore*)nextScoreToBeat;
-(UserScore*)nextScoreBeaten;
-(ParallaxLayerType)currentLevel;
-(int)currentLevelNumber;
-(float)currentLevelTargetTime;
-(ParallaxLayerType)nextLevel;
-(ParallaxLayerType)nextLevelToPreload;
-(float)nextWave;
-(float)speed;
-(float)difficultyFactor; // De 0 a 1

-(void)didHitPlayer:(int)numPlayer withSource:(GameEntityType)source;
-(int)numHits;
-(int)numHitsWithSource:(GameEntityType)source;

-(BOOL)isGadgetEnabled:(GadgetType)type;
-(int)numGadgetsEnabled;

-(NSArray*)missions;

-(void)addKill:(Kill*)kill;
-(int)numKills;
-(int)numKillsWithEntityType:(GameEntityType)type;
-(int)numKillsWithGunType:(GunType)type;
-(int)numKillsInBulletTime;
-(int)numDifferentWeaponsUsedForKilling;

-(void)addDodge:(Dodge*)dodge;
-(int)numDodges;
-(int)numDodgesWithEntityType:(GameEntityType)type;

-(int)combo;
-(int)maxCombo;

-(void)addPowerUpBuyed:(GameEntityType)type;
-(int)numPowerUpsBuyed;
-(int)numPowerUpsBuyedWithType:(GameEntityType)type;
-(GameEntityType)powerUpBuyedAtIndex:(int)index;
-(NSArray*)differentTypesOfPowerUpsBuyed;

-(PokerCardConfig*)randomCard;
-(void)addCard:(PokerCardConfig*)config;
-(void)replaceCardInIndex:(int)index withCard:(PokerCardConfig*)config;
-(NSArray*)cards;
-(PokerCardHand*)currentHand;

-(void)setVideoSnapshot:(UIImage*)snapshot withType:(VideoSnapshotType)type;
-(UIImage*)videoSnapshot;
-(VideoSnapshotType)videoSnapshotType;

-(int)numLevelStatictics;
-(LevelStatistics*)levelStaticticsAtIndex:(int)index;

-(float)timeWithoutHit;
-(float)timeAccelerating;

@end
