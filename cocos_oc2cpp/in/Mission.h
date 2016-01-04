//
//  Mission.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 26/11/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "MilitarRange.h"
#import "VehicleConfig.h"

UIKIT_EXTERN NSString *const MissionAccomplishedNotification;

@class GameState;
@class Mission;
@class PokerCardHand;

@interface Mission : NSObject
+(NSArray*)missionsWithRange:(MilitarRangeType)range;
-(void)updateWithGameState:(GameState*)state;
-(void)updateWithCards:(NSArray*)cards pokerHand:(PokerCardHand*)hand numPlaysInARow:(int)numPlays;
-(void)updateBecauseCashInPokerChips;
-(void)updateBecauseDidPlayWithoutPokerChips;
-(void)updateBecauseDidShareVideo;
-(BOOL)isAccomplished;
-(void)setAccomplished:(BOOL)accomplished;
-(BOOL)isRewarded;
-(void)setRewarded:(BOOL)rewarded;
-(int)rewardCoins;
-(int)rewardSpecialCoins;
-(BOOL)needFishes;
-(BOOL)needRings;
-(BOOL)isTheFirstUpgradeWeaponMission;
-(BOOL)mustUseVehicleToAccomplish:(VehicleType)type;
-(NSString*)text;
-(NSString*)icon;
-(NSString*)iconAccomplished;
-(NSString*)iconNotAccomplished;
-(int)price; // En monedas especiales
-(int)ident;
-(int)order;
// Elimina toda la información de la misión
-(void)reset;
// Imprime todas las misiones en consola
+(void)printMissions;
// Comprueba que no hay misiones repetidas
+(BOOL)checkMissions;
@end
