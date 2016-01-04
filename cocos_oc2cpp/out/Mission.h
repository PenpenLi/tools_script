//
//  Mission.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 26/11/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#include "MilitarRange.h"
#include "VehicleConfig.h"

UIKIT_EXTERNstd::string const MissionAccomplishedNotification;

@class GameState;
@class Mission;
@class PokerCardHand;

class Mission : public NSObject {public:
   Vector<T>* missionsWithRange(MilitarRangeType range);
    void updateWithGameState(GameState* state);
    void updateWithCardsVector<T>* cards, PokerCardHand* hand, int numPlays);
    void updateBecauseCashInPokerChips();
    void updateBecauseDidPlayWithoutPokerChips();
    void updateBecauseDidShareVideo();
    bool isAccomplished();
    void setAccomplished(bool accomplished);
    bool isRewarded();
    void setRewarded(bool rewarded);
    int rewardCoins();
    int rewardSpecialCoins();
    bool needFishes();
    bool needRings();
    bool isTheFirstUpgradeWeaponMission();
    bool mustUseVehicleToAccomplish(VehicleType type);
   std::string text();
   std::string icon();
   std::string iconAccomplished();
   std::string iconNotAccomplished();
    int price(); // En monedas especiales
    int ident();
    int order();
// Elimina toda la información de la misión
    void reset();
// Imprime todas las misiones en consola
    void printMissions();
// Comprueba que false hay misiones repetidas
    bool checkMissions();
};
