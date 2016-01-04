//
//  GameState.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 18/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#include "GameEntity.h"
#include "PowerUp.h"
#include "Gun.h"
#include "Kill.h"
#include "Dodge.h"
#include "AvatarConfig.h"
#include "GadgetConfig.h"
#include "WeaponConfig.h"
#include "VehicleConfig.h"
#include "ParallaxLayer.h"
#include "PokerCard.h"
#include "UserScore.h"

typedef enum{
    VIDEO_SNAPSHOT_TYPE_NONE = 0,
    VIDEO_SNAPSHOT_TYPE_START = 1,
    VIDEO_SNAPSHOT_TYPE_SHOOT = 2,
    VIDEO_SNAPSHOT_TYPE_BULLET = 3,
    VIDEO_SNAPSHOT_TYPE_BUOY = 4
}VideoSnapshotType;

class LevelStatistics : public NSObject {public:
    int level;
    GameEntityType powerUpBuyed;
    int numPowerUpsUsed;
    int numLostLives;
    int numEnemiesKilled;
    int numCoinsCollected;
    int numCoinsSpent;
    int numSpecialCoinsCollected;
    int numSpecialCoinsSpent;
    int numPokerChipsCollected;
    int numRebornsBuyed;
    int distance;
    int duration;
};


    void didPerformCombo(int combo, Kill* kill);
};

class GameState : public NSObject {public:
@property (assign) id<GameStateDelegate> delegate;
@property (readonly) int numPlayers;
@property (readonly) float distance;
    float time;
    int lifePlayer1;
    int lifePlayer2;
    int ammoPlayer1;
    int ammoPlayer2;
    int maxLife;
    int livesLost;
    int coins;
    int coinsSpent;
    int coinsWhileDead;
    int specialCoins;
    int specialCoinsSpent;
    int pokerChipsTotal;
    int pokerChipsRemaining;
    int points;
    int jumps;
    int dives;
    int rings;
    int fishes;
    int minesJumped;
    int minesDived;
    int playerShotHits;
    int playerShotMisses;
    int enemyShotHits;
    int enemyShotMisses;
    int cardsDodged;
    int reborns;
    int pokerReborns;
    int friendBuoysDestroyed;
    int powerUpsUsed;
    int itemBoxesDestroyed;
    int minesDestroyedWithGadget;
    float weaponPowerBonus;
    float weaponSpeedBonus;
    float weaponAmmoBonus;
    bool helicopterTouched;
    bool submarineTouched;
    bool boatTouched;
    bool isGameStarted;
    bool isGameOver;
    bool isKrakenReleased;
    bool isEnemyBitten;
    bool isEnemyElectrocuted;
    bool isTutorial;
    bool isVideoRewarded;
    GameEntityType lastBonusLetterPicked;
    GadgetType gadget1;
    GadgetType gadget2;
    GadgetType gadget3;

   static GameState* initWithNumPlayers(int numPlayers);
    void enableCrazyMode();
    bool isCrazyModeEnabled();

    bool isPlayerDead(int numPlayer);
    void setDied(bool died, int numPlayer);

    bool isSecondChanceRefusedForPlayer(int numPlayer);
    void setSecondChanceRefused(bool refused, int numPlayer);

    bool areAllPlayersDied();

    void update(ccTime dt);
    void addDistance(float distance);

    AvatarConfig* avatarForPlayer(int player);
    VehicleConfig* vehicleForPlayer(int player);
    WeaponConfig* weaponForPlayer(int player);
    void loadWeapon(WeaponType type, int numPlayer);
    int numLoadedWeapons();
    int numLoadedWeaponsWithType(WeaponType type);

    GameEntityType nextEnemy();
    GameEntityType nextItemBox();
    void resetWeaponItemBoxTimer();
    UserScore* nextScoreToBeat();
    UserScore* nextScoreBeaten();
    ParallaxLayerType currentLevel();
    int currentLevelNumber();
    float currentLevelTargetTime();
    ParallaxLayerType nextLevel();
    ParallaxLayerType nextLevelToPreload();
    float nextWave();
    float speed();
    float difficultyFactor(); // De 0 a 1

    void didHitPlayer(int numPlayer, GameEntityType source);
    int numHits();
    int numHitsWithSource(GameEntityType source);

    bool isGadgetEnabled(GadgetType type);
    int numGadgetsEnabled();

   Vector<T>* missions();

    void addKill(Kill* kill);
    int numKills();
    int numKillsWithEntityType(GameEntityType type);
    int numKillsWithGunType(GunType type);
    int numKillsInBulletTime();
    int numDifferentWeaponsUsedForKilling();

    void addDodge(Dodge* dodge);
    int numDodges();
    int numDodgesWithEntityType(GameEntityType type);

    int combo();
    int maxCombo();

    void addPowerUpBuyed(GameEntityType type);
    int numPowerUpsBuyed();
    int numPowerUpsBuyedWithType(GameEntityType type);
    GameEntityType powerUpBuyedAtIndex(int index);
   Vector<T>* differentTypesOfPowerUpsBuyed();

    PokerCardConfig* randomCard();
    void addCard(PokerCardConfig* config);
    void replaceCardInIndex(int index, PokerCardConfig* config);
   Vector<T>* cards();
    PokerCardHand* currentHand();

    void setVideoSnapshot(UIImage* snapshot, VideoSnapshotType type);
    UIImage* videoSnapshot();
    VideoSnapshotType videoSnapshotType();

    int numLevelStatictics();
    LevelStatistics* levelStaticticsAtIndex(int index);

    float timeWithoutHit();
    float timeAccelerating();

};
