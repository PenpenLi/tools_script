//
//  Mission.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 26/11/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "Mission.h"
#import "GameState.h"
#import "SDCloudUserDefaults.h"
#import "FacebookManager.h"
#import "GameManager.h"
#import "KamcordHelper.h"
#import "ParamManager.h"
#import "Achievement.h"

NSString *const MissionAccomplishedNotification = @"MissionAccomplishedNotification";

typedef enum {
    MISSION_TYPE_MONEY_IN_A_GAME_1 = 10000,                 // Recoge X dinero en una partida
    MISSION_TYPE_MONEY_IN_A_GAME_2 = 10001,
    MISSION_TYPE_MONEY_IN_A_GAME_3 = 10002,
    MISSION_TYPE_MONEY_IN_A_GAME_4 = 10003,
    
    MISSION_TYPE_MONEY_ACCUMULATION_1 = 10010,              // Recoge X dinero en total
    MISSION_TYPE_MONEY_ACCUMULATION_2 = 10011,
    MISSION_TYPE_MONEY_ACCUMULATION_3 = 10012,
    MISSION_TYPE_MONEY_ACCUMULATION_4 = 10013,
    
    MISSION_TYPE_NO_MONEY = 10020,                          // Completa una partida sin coger dinero
    
    MISSION_TYPE_REACH_DISTANCE_1 = 10030,                  // Llega a la distancia en una partida
    MISSION_TYPE_REACH_DISTANCE_2 = 10031,
    MISSION_TYPE_REACH_DISTANCE_3 = 10032,
    MISSION_TYPE_REACH_DISTANCE_4 = 10033,
    
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_1 = 10040,         // Completa una partida entre [x,y] metros
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_2 = 10041,
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_3 = 10042,
    
    MISSION_TYPE_DISTANCE_ACCUMULATION_1 = 10050,           // Acumula X metros
    MISSION_TYPE_DISTANCE_ACCUMULATION_2 = 10051,
    MISSION_TYPE_DISTANCE_ACCUMULATION_3 = 10052,
    
    MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_1 = 10060,  // Recorrer X distancia en una partida sin saltar
    MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_2 = 10061,
    MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_3 = 10062,
    
    MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_1 = 10070,  // Recorre X metros sin ser herido
    MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_2 = 10071,
    MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_3 = 10072,
    
    MISSION_TYPE_JUMPS_IN_A_GAME_1 = 10080,                 // Salta X veces en una partida
    MISSION_TYPE_JUMPS_IN_A_GAME_2 = 10081,
    MISSION_TYPE_JUMPS_IN_A_GAME_3 = 10082,
    
    MISSION_TYPE_DIVES_IN_A_GAME_1 = 10090,                 // Salta X veces en una partida
    MISSION_TYPE_DIVES_IN_A_GAME_2 = 10091,
    MISSION_TYPE_DIVES_IN_A_GAME_3 = 10092,
    
    MISSION_TYPE_DODGE_MINE = 10100,                        // Esquiva 1 mina (es decir, no la destruyas)
    
    MISSION_TYPE_TOUCH_SUBMARINE = 10110,                   // Toca un submarino
    MISSION_TYPE_TOUCH_BOAT = 10111,                        // Toca un bote
    MISSION_TYPE_TOUCH_HELICOPTER = 10112,                  // Toca un helicóptero
    
    MISSION_TYPE_PEACEMAKER = 10120,                        // Completa una partida sin matar a nadie
    
    MISSION_TYPE_NINJA = 10130,                             // Completa una partida sin disparar matando al menos a un enemigo (útil Minecrusher)
    
    MISSION_TYPE_USE_A_GADGET = 10140,                      // Compra y utiliza un gadget
    MISSION_TYPE_USE_3_GADGETS_IN_A_ROW = 10141,            // Utiliza 3 gadgets a la vez (último objetivo)
    
    MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_1 = 10150,          // Mata a X enemigos en una partida
    MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_2 = 10151,
    MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_3 = 10152,
    MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_4 = 10153,
    
    MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_1 = 10160,       // Mata a X enemigos en total
    MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_2 = 10161,
    MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_3 = 10162,
    
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1 = 10170,   // Mata a X enemigos usando el arma Z
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2 = 10171,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3 = 10172,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4 = 10173,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5 = 10174,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_6 = 10175,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7 = 10176,   // NO UTILIZADO
    
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1 = 10190,       // Mata a X submarinos en una partida
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_2 = 10191,       // NO UTILIZADO
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_3 = 10192,       // NO UTILIZADO
    
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1 = 10200,      // Mata a X helicópteros en una partida
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2 = 10201,
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_3 = 10202,      // NO UTILIZADO
    
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1 = 10210,          // Mata a X bombarderos en una partida
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2 = 10211,
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_3 = 10212,
    
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_1 = 10220,            // Mata a X lanchas en una partida
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_2 = 10221,
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_3 = 10222,
    
    MISSION_TYPE_KILL_MINES_IN_A_GAME_1 = 10230,            // Mata a X minas en una partida
    MISSION_TYPE_KILL_MINES_IN_A_GAME_2 = 10231,            // NO UTILIZADO
    MISSION_TYPE_KILL_MINES_IN_A_GAME_3 = 10232,            // NO UTILIZADO
    
    MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_1 = 10240,      // Mata a X paracaidistas en una partida
    MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_2 = 10241,
    MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_3 = 10242,
    
    MISSION_TYPE_MERCY = 10250,                             // Deja a X enemigos con vida en una partida
    
    MISSION_TYPE_MATRIX = 10260,                            // Mata a X enemigos usando el tiempo bala
    
    MISSION_TYPE_REBORN = 10270,                            // Resucita 1 vez
    
    MISSION_TYPE_FISHES_IN_A_GAME_1 = 10280,                // Recoge X pescados
    MISSION_TYPE_FISHES_IN_A_GAME_2 = 10281,
    MISSION_TYPE_FISHES_IN_A_GAME_3 = 10282,
    
    MISSION_TYPE_RINGS_IN_A_GAME = 10290,                   // Recoge X anillos
    
    MISSION_TYPE_COMBOS_IN_A_GAME_1 = 10300,                // Haz X combos
    MISSION_TYPE_COMBOS_IN_A_GAME_2 = 10301,
    
    MISSION_TYPE_BUY_POWER_UP_1 = 10310,                    // Compra de power ups
    MISSION_TYPE_BUY_POWER_UP_2 = 10311,
    
    MISSION_TYPE_JUMP_MINES = 10320,                        // Salta X minas
    
    MISSION_TYPE_DIVE_MINES = 10330,                        // Bucea por debajo de X minas
    
    MISSION_TYPE_RELEASE_KRAKEN = 10340,                    // Liberar al Kraken
    
    //MISSION_TYPE_ENEMY_BITTEN = 10350,                    // Mordedura a un enemigo (tiburón)
    MISSION_TYPE_USE_VEHICLE_SHARK = 10350,                 // (se cambia a que simplemente tenga que utilizar el tiburón
    
    MISSION_TYPE_ENEMY_ELECTROCUTED = 10360,                // Enemigo electrocutado (arma plasma nivel 5)
    
    MISSION_TYPE_LOGIN_FACEBOOK = 10370,                    // Logarse en Facebook
    
    MISSION_TYPE_PERFECT = 10380,                           // En un nivel entero matar a todos los enemigos
    
    MISSION_TYPE_CHANGE_AVATAR = 10390,                    // Usar un personaje que no sea Mambo
    
    MISSION_TYPE_CHANGE_VEHICLE = 10400,                    // Usar un vehículo que no sea la barca
    
    MISSION_TYPE_PLAY_GAMES = 10410,                        // Jugar X partidas
    
    MISSION_TYPE_LEVEL_2_WITHOUT_KILLS = 10420,             // Llega al nivel 2 sin matar a ningún enemigo
    
    MISSION_TYPE_ACCURACY_1 = 10430,                        // Acaba con un porcentaje de acierto
    
    MISSION_TYPE_SHARE_VIDEO = 10440,                       // Comparte un vídeo
    
    MISSION_TYPE_UPGRADE_WEAPON = 10450,                    // Juega con un arma upgradeada
    
    MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_1 = 10460,     // Mata usando diferentes armas
    MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_2 = 10461,
    MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_3 = 10462,
    
    MISSION_TYPE_REACH_STAGE_2 = 10470,                     // Termina la primera fase
    
    MISSION_TYPE_USE_GADGET_1 = 10480,                      // Utiliza Gadget X
    MISSION_TYPE_USE_GADGET_2 = 10481,
    MISSION_TYPE_USE_GADGET_3 = 10482,
    MISSION_TYPE_USE_GADGET_4 = 10483,
    MISSION_TYPE_USE_GADGET_5 = 10484,
    
    MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1 = 10490,           // Upgradea el arma X al nivel Y
    MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2 = 10491,
    MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3 = 10492,
    MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4 = 10493,
    MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5 = 10494,
    
    MISSION_TYPE_USE_VEHICLE_POLICE = 10500,                     // Usar vehículo X
    MISSION_TYPE_USE_VEHICLE_SURF = 10501,
    MISSION_TYPE_USE_VEHICLE_HIPPO = 10502,
    MISSION_TYPE_USE_VEHICLE_SUBMARINE = 10503,
    
    MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS_1 = 10510,     // Llega al nivel X sólo con la pistola
    
    MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME_1 = 10520,    // Destruye cajas sorpresa en una partida
    
    MISSION_TYPE_MAX_VELOCITY_TIME = 10530,                 // A máxima velocidad durante X segundos
    
    MISSION_TYPE_KILL_MINE_USING_GADGET = 10540,            // Destruye una mina usando el gadget de destroza minas
    
    MISSION_TYPE_REACH_STAGE_WITHOUT_HURT = 10550,          // Llega a la fase X sin que te hieran
    
    MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1 = 10560,         // Revive dos veces en la fase 1
    
    MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS = 10570, // Juega con un personajes que tenga todas las armas upgradeadas
    
    MISSION_TYPE_CARD_ONE_PAIR = 20000,                     // Consigue una pareja cualquiera
    MISSION_TYPE_CARD_ONE_PAIR_10 = 20001,                  // Consigue una pareja de 10s
    MISSION_TYPE_CARD_ONE_PAIR_J = 20002,                   // Consigue una pareja de J's
    MISSION_TYPE_CARD_ONE_PAIR_Q = 20003,                   // Consigue una pareja de Q's
    MISSION_TYPE_CARD_ONE_PAIR_K = 20004,                   // Consigue una pareja de K`s
    MISSION_TYPE_CARD_ONE_PAIR_ACES = 20005,                // Consigue una pareja de ases
    MISSION_TYPE_CARD_TWO_PAIRS = 20006,                    // Consigue dobles parejas
    MISSION_TYPE_CARD_THREE_OF_A_KIND = 20007,              // Consigue un trío
    MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES = 20008,         // Consigue un trío de ases
    MISSION_TYPE_CARD_FLUSH = 20009,                        // Consigue color
    MISSION_TYPE_CARD_STRAIGHT = 20010,                     // Consigue una escalera
    MISSION_TYPE_CARD_FULL_HOUSE = 20011,                   // Consigue un full
    MISSION_TYPE_CARD_JOKER = 20012,                        // Consigue un jóker
    MISSION_TYPE_CARD_JQK = 20014,                          // Consigue JQK en un juego    
    MISSION_TYPE_CARD_BLACKJACK = 20020,                    // Consigue black jack
    MISSION_TYPE_CARD_CHIPS_IN_A_GAME_1 = 20021,            // Consigue X fichas en una partida
    MISSION_TYPE_CARD_CHIPS_IN_A_GAME_2 = 20022,
    MISSION_TYPE_CARD_CASH_IN = 20023,                      // Haz una liquidación de fichas
    MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS = 20024,           // Juega sin usar fichas
    MISSION_TYPE_CARD_RED_CARDS = 20025,                    // Consigue 5 cartas rojas
    MISSION_TYPE_CARD_BLACK_CARDS = 20026,                  // Consigue 5 cartas negras
    MISSION_TYPE_CARD_FIGURES = 20027,                      // Consigue 5 figuras
    MISSION_TYPE_CARD_CHIPS_IN_A_GAME_3 = 20028,
    MISSION_TYPE_CARD_CHIPS_ACCUMULATION_1 = 20029,         // Consigue X fichas acumuladas
    MISSION_TYPE_CARD_CHIPS_ACCUMULATION_2 = 20030,
    MISSION_TYPE_CARD_CHIPS_ACCUMULATION_3 = 20031,
    MISSION_TYPE_CARD_PLAY_IN_A_ROW = 20032,                // Juega X veces seguidas
    MISSION_TYPE_CARD_PRIZES_ACCUMULATION = 20033,          // Acumula X premios
    MISSION_TYPE_CARD_ONE_PRIZE = 20034                     // Consigue un premio de póker
}MissionType;


typedef enum{
    MISSION_ICON_TYPE_1,
    MISSION_ICON_TYPE_2,
    MISSION_ICON_TYPE_3,
    MISSION_ICON_TYPE_4,
    MISSION_ICON_TYPE_5,
    MISSION_ICON_TYPE_6,
    MISSION_ICON_TYPE_7,
    MISSION_ICON_TYPE_8,
    MISSION_ICON_TYPE_9,
    MISSION_ICON_TYPE_10,
    MISSION_ICON_TYPE_11,
    MISSION_ICON_TYPE_12,
    MISSION_ICON_TYPE_13,
    MISSION_ICON_TYPE_14,
    MISSION_ICON_TYPE_15,
    MISSION_ICON_TYPE_16,
    MISSION_ICON_TYPE_17,
    MISSION_ICON_TYPE_18,
    MISSION_ICON_TYPE_19,
    MISSION_ICON_TYPE_20,
    MISSION_ICON_TYPE_21,
    MISSION_ICON_TYPE_22,
    MISSION_ICON_TYPE_23,
    MISSION_ICON_TYPE_24,
    MISSION_ICON_TYPE_25,
    MISSION_ICON_TYPE_26,
    MISSION_ICON_TYPE_27
}MissionIconType;

typedef void (^MissionUpdateBlock)(GameState *state);

@interface Mission ()
@property (copy) NSString *string;
@property (copy) NSString *baseString;
@end

@implementation Mission{
    MissionType _type;
    MissionIconType _icon;
    BOOL _isAccomplished;
    BOOL _isRewarded;
    int _rewardCoins;
    int _rewardSpecialCoins;
    int _specialCoins;
    int _order;
    MissionUpdateBlock _block;
}

+(NSMutableDictionary*)allMissions{
    static NSMutableDictionary *dictionary = nil;
    if (!dictionary)
        dictionary = [[NSMutableDictionary alloc]initWithCapacity:100];
    return dictionary;
}

+(NSArray*)missionsWithRange:(MilitarRangeType)range{
    NSMutableArray *array = [NSMutableArray arrayWithCapacity:3];
    
    switch (range) {
        case MILITAR_RANGE_TYPE_01_PRIVATE:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_1 order:1 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_STAGE_2 order:2 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_PLAY_GAMES order:3 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_JUMPS_IN_A_GAME_1 order:4 specialCoins:2]];
            break;
        case MILITAR_RANGE_TYPE_02_PRIVATE_FIRST_CLASS:
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_WEAPON order:5 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DIVES_IN_A_GAME_1 order:6 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_1 order:7 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_DISTANCE_1 order:8 specialCoins:2]];
            break;
        case MILITAR_RANGE_TYPE_03_CORPORAL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_IN_A_GAME_1 order:9 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_VEHICLE_SHARK order:10 specialCoins:15]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_FISHES_IN_A_GAME_1 order:11 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_CHIPS_IN_A_GAME_1 order:12 specialCoins:2]];
            break;
        case MILITAR_RANGE_TYPE_04_SERGEANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1 order:13 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_ACCUMULATION_1 order:14 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_ACCUMULATION_1 order:15 specialCoins:2]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_GADGET_1 order:16 specialCoins:4]];
            break;
        case MILITAR_RANGE_TYPE_05_STAFF_SERGEANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_IN_A_GAME_2 order:17 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_1 order:18 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1 order:19 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_ONE_PRIZE order:20 specialCoins:4]];
            break;
        case MILITAR_RANGE_TYPE_06_SERGEANT_FIRST_CLASS:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_2 order:21 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1 order:22 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_FISHES_IN_A_GAME_2 order:23 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_1 order:24 specialCoins:4]];
            break;
        case MILITAR_RANGE_TYPE_07_MASTER_SERGEANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_ACCUMULATION_2 order:25 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1 order:26 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_VEHICLE_POLICE order:27 specialCoins:40]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_BLACK_CARDS order:28 specialCoins:4]];
            break;
        case MILITAR_RANGE_TYPE_08_FIRST_SERGEANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_COMBOS_IN_A_GAME_1 order:29 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_BOATS_IN_A_GAME_1 order:30 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_DISTANCE_2 order:31 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_GADGET_2 order:32 specialCoins:15]];
            break;
        case MILITAR_RANGE_TYPE_09_SERGEANT_MAJOR:
            [array addObject:[Mission missionWithType:MISSION_TYPE_TOUCH_BOAT order:33 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_2 order:34 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_1 order:35 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2 order:36 specialCoins:25]];
            break;
        case MILITAR_RANGE_TYPE_10_COMMAND_SERGEANT_MAJOR:
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_IN_A_GAME_3 order:37 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_MINES_IN_A_GAME_1 order:38 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_2 order:39 specialCoins:4]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_ONE_PAIR order:40 specialCoins:4]];
            break;
        case MILITAR_RANGE_TYPE_11_SERGEANT_MAJOR_OF_THE_ARMY:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2 order:41 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_ACCUMULATION_2 order:42 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_VEHICLE_SURF order:43 specialCoins:80]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS order:44 specialCoins:6]];
            break;
        case MILITAR_RANGE_TYPE_12_SECOND_LIEUTENANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2 order:45 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_LEVEL_2_WITHOUT_KILLS order:46 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MATRIX order:47 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_GADGET_3 order:48 specialCoins:15]];
            break;
        case MILITAR_RANGE_TYPE_13_FIRST_LIEUTENANT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2 order:49 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REBORN order:50 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DODGE_MINE order:51 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3 order:52 specialCoins:35]];
            break;
        case MILITAR_RANGE_TYPE_14_CAPTAIN:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3 order:53 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_BOATS_IN_A_GAME_2 order:54 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_FISHES_IN_A_GAME_3 order:55 specialCoins:6]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_RED_CARDS order:56 specialCoins:6]];
            break;
        case MILITAR_RANGE_TYPE_15_MAJOR:
            [array addObject:[Mission missionWithType:MISSION_TYPE_PERFECT order:57 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_2 order:58 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_VEHICLE_HIPPO order:59 specialCoins:140]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_ACCUMULATION_3 order:60 specialCoins:8]];
            break;
        case MILITAR_RANGE_TYPE_16_LIEUTENANT_COLONEL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_2 order:61 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MERCY order:62 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_1 order:63 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_GADGET_4 order:64 specialCoins:80]];
            break;
        case MILITAR_RANGE_TYPE_17_COLONEL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_3 order:65 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DISTANCE_ACCUMULATION_3 order:66 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_ACCUMULATION_4 order:67 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4 order:68 specialCoins:35]];
            break;
        case MILITAR_RANGE_TYPE_18_BRIGADIER_GENERAL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4 order:69 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_DISTANCE_3 order:70 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_2 order:71 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_CHIPS_IN_A_GAME_2 order:72 specialCoins:8]];
            break;
        case MILITAR_RANGE_TYPE_19_MAJOR_GENERAL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_3 order:73 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_COMBOS_IN_A_GAME_2 order:74 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_TOUCH_SUBMARINE order:75 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_VEHICLE_SUBMARINE order:76 specialCoins:200]];
            break;
        case MILITAR_RANGE_TYPE_20_LIEUTENANT_GENERAL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5 order:77 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_MONEY_IN_A_GAME_4 order:78 specialCoins:8]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_USE_GADGET_5 order:79 specialCoins:100]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_TWO_PAIRS order:80 specialCoins:8]];
            break;
        case MILITAR_RANGE_TYPE_21_GENERAL:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1 order:81 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_3 order:82 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5 order:83 specialCoins:75]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_THREE_OF_A_KIND order:84 specialCoins:10]];
            break;
        case MILITAR_RANGE_TYPE_22_GENERAL_OF_THE_ARMY:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7 order:85 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS_1 order:86 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME_1 order:87 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_CHIPS_ACCUMULATION_1 order:88 specialCoins:10]];
            break;
        case MILITAR_RANGE_TYPE_23_LEGION_OF_MERIT:
            [array addObject:[Mission missionWithType:MISSION_TYPE_MAX_VELOCITY_TIME order:89 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_DISTANCE_INTERVAL_1 order:90 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_MINE_USING_GADGET order:91 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_PLAY_IN_A_ROW order:92 specialCoins:10]];
            break;
        case MILITAR_RANGE_TYPE_24_SILVER_STAR:
            [array addObject:[Mission missionWithType:MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_4 order:93 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_STAGE_WITHOUT_HURT order:94 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_DIVE_MINES order:95 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_PRIZES_ACCUMULATION order:96 specialCoins:10]];
            break;
        case MILITAR_RANGE_TYPE_25_SUPERIOR_SERVICE:
            [array addObject:[Mission missionWithType:MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1 order:97 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS order:98 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_REACH_DISTANCE_4 order:99 specialCoins:10]];
            [array addObject:[Mission missionWithType:MISSION_TYPE_CARD_FLUSH order:100 specialCoins:10]];
            break;
        case MILITAR_RANGE_TYPE_26_DISTINGUISHED_SERVICE:
            break;
        default:
            NSAssert(NO,@"MilitarRange:initWithType Rango desconocido");
            break;
    }
    
    return array;
}

+(id)missionWithType:(MissionType)type order:(int)order specialCoins:(int)specialCoins{
    NSMutableDictionary *dict = [self allMissions];
    NSNumber *key = [NSNumber numberWithInt:type];
    id obj = [dict objectForKey:key];
    if (!obj){
        obj = [[[Mission alloc]initWithType:type order:order specialCoins:specialCoins]autorelease];
        [dict setObject:obj forKey:key];
    }
    return obj;           
}

-(id)initWithType:(MissionType)aType order:(int)anOrder specialCoins:(int)sp{
    self = [super init];
    if (self){
        self.string = nil;
        self.baseString = nil;
        _type = aType;
        _order = anOrder;
        _icon = MISSION_ICON_TYPE_1;
        _isAccomplished = [SDCloudUserDefaults boolForKey:[self keyAccomplished]];
        _isRewarded = [SDCloudUserDefaults boolForKey:[self keyRewarded]];
        _rewardCoins = [[[[ParamManager sharedManager]objectForKey:@"MissionReward"]objectForKey:@"coins"]intValue];
        _rewardSpecialCoins = [[[[ParamManager sharedManager]objectForKey:@"MissionReward"]objectForKey:@"specialCoins"]intValue];
        _specialCoins = sp;
        _block = nil;
    
        
        switch (_type) {
            case MISSION_TYPE_MONEY_IN_A_GAME_1:
            case MISSION_TYPE_MONEY_IN_A_GAME_2:
            case MISSION_TYPE_MONEY_IN_A_GAME_3:
            case MISSION_TYPE_MONEY_IN_A_GAME_4:
            {
                _icon = MISSION_ICON_TYPE_6;
                int targetInt = 0;
                if (_type == MISSION_TYPE_MONEY_IN_A_GAME_1) targetInt = 100;
                if (_type == MISSION_TYPE_MONEY_IN_A_GAME_2) targetInt = 300;
                if (_type == MISSION_TYPE_MONEY_IN_A_GAME_3) targetInt = 400;
                if (_type == MISSION_TYPE_MONEY_IN_A_GAME_4) targetInt = 500;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_MONEY_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.coins;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_MONEY_ACCUMULATION_1:
            case MISSION_TYPE_MONEY_ACCUMULATION_2:
            case MISSION_TYPE_MONEY_ACCUMULATION_3:
            case MISSION_TYPE_MONEY_ACCUMULATION_4:
            {
                _icon = MISSION_ICON_TYPE_6;
                int targetInt = 0;
                if (_type == MISSION_TYPE_MONEY_ACCUMULATION_1) targetInt = 1000;
                if (_type == MISSION_TYPE_MONEY_ACCUMULATION_2) targetInt = 2000;
                if (_type == MISSION_TYPE_MONEY_ACCUMULATION_3) targetInt = 3000;
                if (_type == MISSION_TYPE_MONEY_ACCUMULATION_4) targetInt = 5000;
                __block int accumInt = (int)[[NSUserDefaults standardUserDefaults] integerForKey:[self keyAux]];
                int left = targetInt - accumInt;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_MONEY_ACCUMULATION",nil),targetInt];
                [self updateTextWithLeft:left];
                _block = Block_copy(^(GameState *state){
                    int totalCoins = state.coins + accumInt;
                    int left = targetInt - totalCoins;
                    [self updateTextWithLeft:left];
                    if (left <= 0)
                        [self didAccomplish];
                    if (state.isGameOver){
                        [[NSUserDefaults standardUserDefaults] setInteger:totalCoins forKey:[self keyAux]];
                        accumInt = totalCoins;
                    }
                });
                break;
            }
            case MISSION_TYPE_NO_MONEY:
            {
                _icon = MISSION_ICON_TYPE_6;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_NO_MONEY",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.coins == 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_REACH_DISTANCE_1:
            case MISSION_TYPE_REACH_DISTANCE_2:
            case MISSION_TYPE_REACH_DISTANCE_3:
            case MISSION_TYPE_REACH_DISTANCE_4:
            {
                _icon = MISSION_ICON_TYPE_2;
                float targetFloat = 0.0f;
                if (_type == MISSION_TYPE_REACH_DISTANCE_1) targetFloat = 2000.0f;
                if (_type == MISSION_TYPE_REACH_DISTANCE_2) targetFloat = 8000.0f;
                if (_type == MISSION_TYPE_REACH_DISTANCE_3) targetFloat = 10000.0f;
                if (_type == MISSION_TYPE_REACH_DISTANCE_4) targetFloat = 11000.0f;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_REACH_DISTANCE",nil),(int)targetFloat];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = (int)(targetFloat - state.distance);
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_DISTANCE_INTERVAL_1:
            case MISSION_TYPE_REACH_DISTANCE_INTERVAL_2:
            case MISSION_TYPE_REACH_DISTANCE_INTERVAL_3:
            {
                _icon = MISSION_ICON_TYPE_2;
                NSRange targetRange;
                if (_type == MISSION_TYPE_REACH_DISTANCE_INTERVAL_1) targetRange = NSMakeRange(5000, 1000);
                if (_type == MISSION_TYPE_REACH_DISTANCE_INTERVAL_2) targetRange = NSMakeRange(1000, 200);
                if (_type == MISSION_TYPE_REACH_DISTANCE_INTERVAL_3) targetRange = NSMakeRange(1000, 100);
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_REACH_DISTANCE_INTERVAL",nil),(int)targetRange.location,(int)(targetRange.location+targetRange.length)];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver){
                        if (NSLocationInRange((int)state.distance, targetRange))
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_DISTANCE_ACCUMULATION_1:
            case MISSION_TYPE_DISTANCE_ACCUMULATION_2:
            case MISSION_TYPE_DISTANCE_ACCUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_2;
                float targetFloat = 0.0f;
                if (_type == MISSION_TYPE_DISTANCE_ACCUMULATION_1) targetFloat = 10000.0f;
                if (_type == MISSION_TYPE_DISTANCE_ACCUMULATION_2) targetFloat = 20000.0f;
                if (_type == MISSION_TYPE_DISTANCE_ACCUMULATION_3) targetFloat = 50000.0f;
                __block float accumFloat = [[NSUserDefaults standardUserDefaults] floatForKey:[self keyAux]];
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DISTANCE_ACCUMULATION",nil),(int)targetFloat];
                [self updateTextWithLeft:(int)(targetFloat - accumFloat)];
                _block = Block_copy(^(GameState *state){
                    float totalDistance = state.distance + accumFloat;
                    int left = (int)(targetFloat - totalDistance);
                    if (totalDistance == 0.0f)
                        self.string = self.baseString;
                    else
                        [self updateTextWithLeft:left];
                    if (left <= 0)
                        [self didAccomplish];
                    if (state.isGameOver){
                        [[NSUserDefaults standardUserDefaults] setFloat:totalDistance forKey:[self keyAux]];
                        accumFloat = totalDistance;
                    }
                });
                break;
            }
            case MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_1:
            case MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_2:
            case MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_3:
            {
                _icon = MISSION_ICON_TYPE_2;
                float targetFloat = 0.0f;
                __block float lastJump = 0.0f;
                __block int numJumps = 0;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_1) targetFloat = 2000.0f;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_2) targetFloat = 4000.0f;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_3) targetFloat = 3000.0f;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW",nil),(int)targetFloat];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        lastJump = 0.0f;
                        numJumps = 0;
                        self.string = self.baseString;
                    }
                    else{
                        if (state.jumps != numJumps){
                            numJumps = state.jumps;
                            lastJump = state.distance;
                        }
                        float totalDistance = state.distance - lastJump;
                        int left = (int)(targetFloat - totalDistance);
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_1:
            case MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_2:
            case MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_3:
            {
                _icon = MISSION_ICON_TYPE_2;
                float targetFloat = 0.0f;
                __block float lastHurt = 0.0f;
                __block int numLives = 0;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_1) targetFloat = 3000.0f;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_2) targetFloat = 6000.0f;
                if (_type == MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_3) targetFloat = 8000.0f;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW",nil),(int)targetFloat];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        lastHurt = 0.0f;
                        numLives = state.maxLife;
                        self.string = self.baseString;
                    }
                    else{
                        if (state.lifePlayer1 > numLives)
                            numLives = state.lifePlayer1; // Ha cogido una vida
                        if (state.lifePlayer1 != numLives){
                            numLives = state.lifePlayer1;
                            lastHurt = state.distance;
                        }
                        float totalDistance = state.distance - lastHurt;
                        int left = (int)(targetFloat - totalDistance);
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_JUMPS_IN_A_GAME_1:
            case MISSION_TYPE_JUMPS_IN_A_GAME_2:
            case MISSION_TYPE_JUMPS_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_13;
                int targetInt = 0;
                if (_type == MISSION_TYPE_JUMPS_IN_A_GAME_1) targetInt = 5;
                if (_type == MISSION_TYPE_JUMPS_IN_A_GAME_2) targetInt = 20;
                if (_type == MISSION_TYPE_JUMPS_IN_A_GAME_3) targetInt = 100;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_JUMPS_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.jumps;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_DIVES_IN_A_GAME_1:
            case MISSION_TYPE_DIVES_IN_A_GAME_2:
            case MISSION_TYPE_DIVES_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_14;
                int targetInt = 0;
                if (_type == MISSION_TYPE_DIVES_IN_A_GAME_1) targetInt = 5;
                if (_type == MISSION_TYPE_DIVES_IN_A_GAME_2) targetInt = 20;
                if (_type == MISSION_TYPE_DIVES_IN_A_GAME_3) targetInt = 100;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DIVES_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.dives;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });

                break;
            }
            case MISSION_TYPE_DODGE_MINE:
            {
                _icon = MISSION_ICON_TYPE_16;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_DODGE_MINE",nil);
                _block = Block_copy(^(GameState *state){
                    if ([state numDodgesWithEntityType:GAME_ENTITY_TYPE_ENEMY_MINE] > 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_TOUCH_SUBMARINE:
            {
                _icon = MISSION_ICON_TYPE_17;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_TOUCH_SUBMARINE",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.submarineTouched)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_TOUCH_BOAT:
            {
                _icon = MISSION_ICON_TYPE_15;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_TOUCH_BOAT",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.boatTouched)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_TOUCH_HELICOPTER:
            {
                _icon = MISSION_ICON_TYPE_11;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_TOUCH_HELICOPTER",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.helicopterTouched)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_PEACEMAKER:
            {
                _icon = MISSION_ICON_TYPE_21;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_PEACEMAKER",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.numKills == 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_NINJA:
            {
                _icon = MISSION_ICON_TYPE_5;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_NINJA",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.numKills > 0 && state.playerShotHits == 0 && state.playerShotMisses == 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_USE_A_GADGET:
            {
                _icon = MISSION_ICON_TYPE_8;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_USE_A_GADGET",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && [state numGadgetsEnabled] > 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_USE_3_GADGETS_IN_A_ROW:
            {
                _icon = MISSION_ICON_TYPE_8;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_USE_3_GADGETS_IN_A_ROW",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && [state numGadgetsEnabled] == 3)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_1:
            case MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_2:
            case MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_3:
            case MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_4:
            {
                _icon = MISSION_ICON_TYPE_3;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_1) targetInt = 20;
                if (_type == MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_2) targetInt = 100;
                if (_type == MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_3) targetInt = 150;
                if (_type == MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_4) targetInt = 200;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMIES_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKills];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_1:
            case MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_2:
            case MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_3;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_1) targetInt = 200;
                if (_type == MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_2) targetInt = 500;
                if (_type == MISSION_TYPE_KILL_ENEMIES_ACCUMULATION_3) targetInt = 1000;
                __block int accumInt = (int)[[NSUserDefaults standardUserDefaults] integerForKey:[self keyAux]];
                int left = targetInt - accumInt;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMIES_ACCUMULATION",nil),targetInt];
                [self updateTextWithLeft:left];
                _block = Block_copy(^(GameState *state){
                    int totalKills = [state numKills] + accumInt;
                    int left = targetInt - totalKills;
                    [self updateTextWithLeft:left];
                    if (left <= 0)
                        [self didAccomplish];
                    if (state.isGameOver){
                        [[NSUserDefaults standardUserDefaults] setInteger:totalKills forKey:[self keyAux]];
                        accumInt = totalKills;
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_6:
            case MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7:
            {
                _icon = MISSION_ICON_TYPE_3;
                int targetInt = 60;
                GunType gunType1 = GUN_TYPE_NONE;
                GunType gunType2 = GUN_TYPE_NONE;
                NSString *substring;
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1){
                    substring = NSLocalizedString(@"WEAPON_PISTOL_NAME",nil);
                    gunType1 = GUN_TYPE_PISTOL;
                    targetInt = 30;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2){
                    substring = NSLocalizedString(@"WEAPON_HEAVY_MACHINE_GUN_NAME",nil);
                    gunType1 = GUN_TYPE_HEAVY_MACHINE_GUN;
                    gunType2 = GUN_TYPE_HEAVY_MACHINE_GUN_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3){
                    substring = NSLocalizedString(@"WEAPON_ROCKET_LAUNCHER_NAME",nil);
                    gunType1 = GUN_TYPE_ROCKET;
                    gunType2 = GUN_TYPE_ROCKET_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4){
                    substring = NSLocalizedString(@"WEAPON_FLAME_SHOT_NAME",nil);
                    gunType1 = GUN_TYPE_FLAME_SHOT;
                    gunType2 = GUN_TYPE_FLAME_SHOT_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5){
                    substring = NSLocalizedString(@"WEAPON_SPREAD_GUN_NAME",nil);
                    gunType1 = GUN_TYPE_SPREAD_GUN;
                    gunType2 = GUN_TYPE_SPREAD_GUN_CARTRIDGE;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_6){
                    substring = NSLocalizedString(@"WEAPON_LASER_NAME",nil);
                    gunType1 = GUN_TYPE_LASER;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7){
                    substring = NSLocalizedString(@"WEAPON_CLUSTER_NAME",nil);
                    gunType1 = GUN_TYPE_CLUSTER;
                    gunType2 = GUN_TYPE_CLUSTER_GRAPE_SHOT;
                }
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME",nil),targetInt,substring];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int numKills1 = 0, numKills2 = 0;
                        if (gunType1 != GUN_TYPE_NONE)
                            numKills1 = [state numKillsWithGunType:gunType1];
                        if (gunType2 != GUN_TYPE_NONE)
                            numKills2 = [state numKillsWithGunType:gunType2];
                        int left = targetInt - (numKills1 + numKills2);
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1:
            case MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_2:
            case MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_17;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1) targetInt = 3;
                if (_type == MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_2) targetInt = 10;
                if (_type == MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_3) targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_SUBMARINE" : @"ENEMY_SUBMARINE_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_SUBMARINE];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1:
            case MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2:
            case MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_11;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1) targetInt = 3;
                if (_type == MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2) targetInt = 5;
                if (_type == MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_3) targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_HELICOPTER" : @"ENEMY_HELICOPTER_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_HELICOPTER];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1:
            case MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2:
            case MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_20;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1) targetInt = 5;
                if (_type == MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2) targetInt = 10;
                if (_type == MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_3) targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_BOMBER" : @"ENEMY_BOMBER_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_BOMBER];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_BOATS_IN_A_GAME_1:
            case MISSION_TYPE_KILL_BOATS_IN_A_GAME_2:
            case MISSION_TYPE_KILL_BOATS_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_15;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_BOATS_IN_A_GAME_1) targetInt = 8;
                if (_type == MISSION_TYPE_KILL_BOATS_IN_A_GAME_2) targetInt = 15;
                if (_type == MISSION_TYPE_KILL_BOATS_IN_A_GAME_3) targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_BOAT" : @"ENEMY_BOAT_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_BOAT];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_MINES_IN_A_GAME_1:
            case MISSION_TYPE_KILL_MINES_IN_A_GAME_2:
            case MISSION_TYPE_KILL_MINES_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_16;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_MINES_IN_A_GAME_1) targetInt = 5;
                if (_type == MISSION_TYPE_KILL_MINES_IN_A_GAME_2) targetInt = 10;
                if (_type == MISSION_TYPE_KILL_MINES_IN_A_GAME_3) targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_MINE" : @"ENEMY_MINE_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_MINE];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_1:
            case MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_2:
            case MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_19;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_1) targetInt = 70;
                if (_type == MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_2) targetInt = 120;
                if (_type == MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_3) targetInt = 150;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_ENEMY_IN_A_GAME",nil),targetInt,NSLocalizedString(targetInt == 1 ? @"ENEMY_PARACHUTIST" : @"ENEMY_PARACHUTIST_PLURAL",nil)];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsWithEntityType:GAME_ENTITY_TYPE_ENEMY_PARACHUTIST];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_MERCY:
            {
                _icon = MISSION_ICON_TYPE_5;
                int targetInt = 20;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_MERCY",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numDodges];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_MATRIX:
            {
                _icon = MISSION_ICON_TYPE_1;
                int targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_MATRIX",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numKillsInBulletTime];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REBORN:
            {
                _icon = MISSION_ICON_TYPE_12;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_REBORN",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.reborns > 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_FISHES_IN_A_GAME_1:
            case MISSION_TYPE_FISHES_IN_A_GAME_2:
            case MISSION_TYPE_FISHES_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_18;
                int targetInt = 0;
                if (_type == MISSION_TYPE_FISHES_IN_A_GAME_1) targetInt = 10;
                if (_type == MISSION_TYPE_FISHES_IN_A_GAME_2) targetInt = 20;
                if (_type == MISSION_TYPE_FISHES_IN_A_GAME_3) targetInt = 25;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_FISHES_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.fishes;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_RINGS_IN_A_GAME:
            {
                _icon = MISSION_ICON_TYPE_5;
                int targetInt = 10;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_RINGS_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.rings;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_COMBOS_IN_A_GAME_1:
            case MISSION_TYPE_COMBOS_IN_A_GAME_2:
            {
                _icon = MISSION_ICON_TYPE_1;
                int targetInt = 0;
                if (_type == MISSION_TYPE_COMBOS_IN_A_GAME_1) targetInt = 15;
                if (_type == MISSION_TYPE_COMBOS_IN_A_GAME_2) targetInt = 20;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_COMBOS_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if ([state maxCombo] > targetInt)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_BUY_POWER_UP_1:
            case MISSION_TYPE_BUY_POWER_UP_2:
            {
                _icon = MISSION_ICON_TYPE_7;
                int targetInt = 0;
                if (_type == MISSION_TYPE_BUY_POWER_UP_1) targetInt = 2;
                if (_type == MISSION_TYPE_BUY_POWER_UP_2) targetInt = 4;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_BUY_POWER_UP",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numPowerUpsBuyed];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_JUMP_MINES:
            {
                _icon = MISSION_ICON_TYPE_16;
                int targetInt = 5;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_JUMP_MINES",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.minesJumped;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_DIVE_MINES:
            {
                _icon = MISSION_ICON_TYPE_16;
                int targetInt = 3;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DIVE_MINES",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.minesDived;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_RELEASE_KRAKEN:
            {
                _icon = MISSION_ICON_TYPE_9;
                NSString *pirateName = NSLocalizedString(@"VEHICLE_PIRATE_NAME",nil);
                NSString *missionText = NSLocalizedString(@"MISSION_TYPE_RELEASE_KRAKEN",nil);
                self.baseString = [NSString stringWithFormat:missionText,pirateName];
                _block = Block_copy(^(GameState *state){
                    if (state.isKrakenReleased)
                        [self didAccomplish];
                });
                break;
            }
            /*
            case MISSION_TYPE_ENEMY_BITTEN:
            {
                _icon = MISSION_ICON_TYPE_24;
                NSString *sharkName = NSLocalizedString(@"VEHICLE_SHARK_NAME",nil);
                NSString *missionText = NSLocalizedString(@"MISSION_TYPE_ENEMY_BITTEN",nil);
                self.baseString = [NSString stringWithFormat:missionText,sharkName];
                _block = Block_copy(^(GameState *state){
                    if (state.isEnemyBitten)
                        [self didAccomplish];
                });
                break;
            }
            */
            case MISSION_TYPE_ENEMY_ELECTROCUTED:
            {
                _icon = MISSION_ICON_TYPE_7;
                NSString *weaponName = NSLocalizedString(@"WEAPON_PLASMA_NAME",nil);
                NSString *missionText = NSLocalizedString(@"MISSION_TYPE_ENEMY_ELECTROCUTED",nil);
                self.baseString = [NSString stringWithFormat:missionText,weaponName];
                _block = Block_copy(^(GameState *state){
                    if (state.isEnemyElectrocuted)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_LOGIN_FACEBOOK:
            {
                _icon = MISSION_ICON_TYPE_10;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_LOGIN_FACEBOOK",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && [[FacebookManager sharedManager]isLogged])
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_PERFECT:
            {
                _icon = MISSION_ICON_TYPE_3;
                __block int level = -1;
                __block int numDodges = -1;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_PERFECT",nil);
                _block = Block_copy(^(GameState *state){
                    if (level < 0){
                        if ([state currentLevelNumber] > 0){
                            level = [state currentLevelNumber];
                            numDodges = [state numDodges];
                        }
                    }
                    else{
                        if ([state currentLevelNumber] != level){
                            if (level > 0 && numDodges == [state numDodges] && [state numKills] > 0)
                                [self didAccomplish];
                            else{
                                level = [state currentLevelNumber];
                                numDodges = [state numDodges];
                            }
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_CHANGE_AVATAR:
            {
                _icon = MISSION_ICON_TYPE_5;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CHANGE_AVATAR",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && [GameManager sharedManager].avatarFirstPlayer != AVATAR_TYPE_MAMBO)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_CHANGE_VEHICLE:
            {
                _icon = MISSION_ICON_TYPE_2;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CHANGE_VEHICLE",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && [GameManager sharedManager].vehicleFirstPlayer != VEHICLE_TYPE_BOAT)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_PLAY_GAMES:
            {
                _icon = MISSION_ICON_TYPE_1;
                int targetInt = 1;
                int left = targetInt - [GameManager sharedManager].gamesPlayed;
                
                if (targetInt == 1)
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_PLAY_GAME_SINGLE",nil),targetInt];
                else
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_PLAY_GAME_PLURAL",nil),targetInt];
                
                if (left != targetInt)
                    [self updateTextWithLeft:left];
                _block = Block_copy(^(GameState *state){
                    int left = targetInt - [GameManager sharedManager].gamesPlayed;
                    if (left != targetInt)
                        [self updateTextWithLeft:left];
                    if (left <= 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_LEVEL_2_WITHOUT_KILLS:
            {
                _icon = MISSION_ICON_TYPE_21;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_LEVEL_2_WITHOUT_KILLS",nil);
                _block = Block_copy(^(GameState *state){
                    if ([state currentLevelNumber] == 2 && [state numKills] == 0)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_ACCURACY_1:
            {
                _icon = MISSION_ICON_TYPE_3;
                float targetFloat = 0.8f;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_ACCURACY",nil),(int)(targetFloat*100.0f)];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.playerShotHits > 0 && (state.playerShotHits / (state.playerShotHits+state.playerShotMisses)) >= targetFloat)
                        [self didAccomplish];
                });
                break;
            }
            case MISSION_TYPE_SHARE_VIDEO:
            {
                _icon = MISSION_ICON_TYPE_10;
                if ([KamcordHelper isEnabled])
                    self.baseString = NSLocalizedString(@"MISSION_TYPE_SHARE_VIDEO",nil);
                else
                    self.baseString = NSLocalizedString(@"MISSION_TYPE_SHARE_PHOTO",nil);
                break;
            }
            case MISSION_TYPE_UPGRADE_WEAPON:
            {
                _icon = MISSION_ICON_TYPE_7;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_UPGRADE_WEAPON", nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([(WeaponConfig*)[WeaponConfig weaponWithType:WEAPON_TYPE_PISTOL]level] > 0)
                            [self didAccomplish];
                        else{
                            NSArray *weapons = [[state avatarForPlayer:1]weapons];
                            for (WeaponConfig *w in weapons)
                                if ([w level] > 0){
                                    [self didAccomplish];
                                    break;
                                }
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_1:
            case MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_2:
            case MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_3:
            {
                _icon = MISSION_ICON_TYPE_3;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_1) targetInt = 3;
                if (_type == MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_2) targetInt = 4;
                if (_type == MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_3) targetInt = 5;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - [state numDifferentWeaponsUsedForKilling];
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_2:
            {
                _icon = MISSION_ICON_TYPE_2;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_REACH_STAGE_2",nil);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([state currentLevelNumber] == 2)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_USE_GADGET_1:
            case MISSION_TYPE_USE_GADGET_2:
            case MISSION_TYPE_USE_GADGET_3:
            case MISSION_TYPE_USE_GADGET_4:
            case MISSION_TYPE_USE_GADGET_5:
            {
                _icon = MISSION_ICON_TYPE_8;
                GadgetConfig *gadget1 = nil;
                GadgetConfig *gadget2 = nil;
                GadgetConfig *gadget3 = nil;
                if (_type == MISSION_TYPE_USE_GADGET_1){
                    gadget1 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_POWER_1];
                    gadget2 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_POWER_2];
                    gadget3 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_POWER_3];
                }
                else if (_type == MISSION_TYPE_USE_GADGET_2){
                    gadget1 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_SPEED_1];
                    gadget2 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_SPEED_2];
                    gadget3 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_SPEED_3];
                }
                else if (_type == MISSION_TYPE_USE_GADGET_3){
                    gadget1 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_AMMO_1];
                    gadget2 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_AMMO_2];
                    gadget3 = [GadgetConfig gadgetWithType:GADGET_TYPE_MORE_AMMO_3];
                }
                else if (_type == MISSION_TYPE_USE_GADGET_4){
                    gadget1 = [GadgetConfig gadgetWithType:GADGET_TYPE_MAGNET];
                }
                else if (_type == MISSION_TYPE_USE_GADGET_5){
                    gadget1 = [GadgetConfig gadgetWithType:GADGET_TYPE_ACCELERATOR];
                }
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_USE_GADGET",nil),[gadget1 name]];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([gadget1 isEquipped])
                            [self didAccomplish];
                        else if (gadget2 && [gadget2 isEquipped])
                            [self didAccomplish];
                        else if (gadget3 && [gadget3 isEquipped])
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1:
            case MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2:
            case MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3:
            case MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4:
            case MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5:
            {
                _icon = MISSION_ICON_TYPE_7;
                WeaponConfig *weapon = nil;
                int level = 1;
                if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1){
                    weapon = [WeaponConfig weaponWithType:WEAPON_TYPE_HEAVY_MACHINE_GUN];
                    level = 2;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2){
                    weapon = [WeaponConfig weaponWithType:WEAPON_TYPE_FLAME_SHOT];
                    level = 3;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3){
                    weapon = [WeaponConfig weaponWithType:WEAPON_TYPE_SPREAD_GUN];
                    level = 4;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4){
                    weapon = [WeaponConfig weaponWithType:WEAPON_TYPE_ROCKET_LAUNCHER];
                    level = 4;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5){
                    weapon = [WeaponConfig weaponWithType:WEAPON_TYPE_HOMING_MISSILE];
                    level = 5;
                }
                
                if (level == 5)
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_UPGRADE_MAX_CUSTOM_WEAPON",nil),[weapon name]];
                else
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_UPGRADE_CUSTOM_WEAPON",nil),[weapon name],level];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        WeaponConfig *loadedWeapon = [state weaponForPlayer:1];
                        if ([loadedWeapon type] == [weapon type] && [loadedWeapon level] >= level)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_USE_VEHICLE_SHARK:
            case MISSION_TYPE_USE_VEHICLE_POLICE:
            case MISSION_TYPE_USE_VEHICLE_SURF:
            case MISSION_TYPE_USE_VEHICLE_HIPPO:
            case MISSION_TYPE_USE_VEHICLE_SUBMARINE:
            {
                VehicleConfig *vehicle = nil;
                if (_type == MISSION_TYPE_USE_VEHICLE_SHARK){
                    vehicle = [VehicleConfig vehicleWithType:VEHICLE_TYPE_SHARK];
                    _icon = MISSION_ICON_TYPE_24;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_POLICE){
                    vehicle = [VehicleConfig vehicleWithType:VEHICLE_TYPE_POLICE];
                    _icon = MISSION_ICON_TYPE_23;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_SURF){
                    vehicle = [VehicleConfig vehicleWithType:VEHICLE_TYPE_SURF];
                    _icon = MISSION_ICON_TYPE_26;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_HIPPO){
                    vehicle = [VehicleConfig vehicleWithType:VEHICLE_TYPE_HIPPO];
                    _icon = MISSION_ICON_TYPE_22;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_SUBMARINE){
                    vehicle = [VehicleConfig vehicleWithType:VEHICLE_TYPE_SUBMARINE];
                    _icon = MISSION_ICON_TYPE_25;
                }
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_USE_VEHICLE",nil),[vehicle name]];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([vehicle isEquipped])
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS_1:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 3;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS",nil),targetInt,[[WeaponConfig weaponWithType:WEAPON_TYPE_PISTOL]name]];
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([state currentLevelNumber] >= targetInt && [state numLoadedWeapons] == [state numLoadedWeaponsWithType:WEAPON_TYPE_PISTOL]){
                            [self didAccomplish];
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME_1:
            {
                _icon = MISSION_ICON_TYPE_27;
                int targetInt = 10;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        int left = targetInt - state.itemBoxesDestroyed;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_MAX_VELOCITY_TIME:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 30;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_MAX_VELOCITY_TIME",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        int left = targetInt - fminf([state timeAccelerating],[state timeWithoutHit]);
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_MINE_USING_GADGET:
            {
                _icon = MISSION_ICON_TYPE_16;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_KILL_MINE_USING_GADGET",nil),[[GadgetConfig gadgetWithType:GADGET_TYPE_MINE_CRUSHER]name]];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        if (state.minesDestroyedWithGadget > 0)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_WITHOUT_HURT:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 4;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_REACH_STAGE_WITHOUT_HURT",nil),targetInt];
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        if ([state numHits] == 0 && [state currentLevelNumber] >= targetInt)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1:
            {
                _icon = MISSION_ICON_TYPE_12;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1",nil);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        if ([state currentLevelNumber] == 1){
                            int left = 2 - state.reborns;
                            [self updateTextWithLeft:left];
                            if (left <= 0)
                                [self didAccomplish];
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS:
            {
                _icon = MISSION_ICON_TYPE_8;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS",nil);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        self.string = self.baseString;
                    }
                    else{
                        NSArray *weapons = [[state avatarForPlayer:1]weapons];
                        BOOL ok = YES;
                        for (WeaponConfig *w in weapons){
                            if ([w level] != 5){
                                ok = NO;
                                break;
                            }
                        }
                        if (ok)
                            [self didAccomplish];
                    }
                });
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR",nil);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_10:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR_10",nil);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_J:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR_J",nil);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_Q:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR_Q",nil);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_K:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR_K",nil);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_ACES:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PAIR_ACES",nil);
                break;
            }
            case MISSION_TYPE_CARD_TWO_PAIRS:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_TWO_PAIRS",nil);
                break;
            }
            case MISSION_TYPE_CARD_THREE_OF_A_KIND:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_THREE_OF_A_KIND",nil);
                break;
            }
            case MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES",nil);
                break;
            }
            case MISSION_TYPE_CARD_FLUSH:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_FLUSH",nil);
                break;
            }
            case MISSION_TYPE_CARD_STRAIGHT:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_STRAIGHT",nil);
                break;
            }
            case MISSION_TYPE_CARD_FULL_HOUSE:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_FULL_HOUSE",nil);
                break;
            }
            case MISSION_TYPE_CARD_JOKER:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_JOKER",nil);
                break;
            }
            case MISSION_TYPE_CARD_JQK:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_JQK",nil);
                break;
            }
            case MISSION_TYPE_CARD_BLACKJACK:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_BLACK_JACK",nil);
                break;
            }
            case MISSION_TYPE_CARD_CHIPS_IN_A_GAME_1:
            case MISSION_TYPE_CARD_CHIPS_IN_A_GAME_2:
            case MISSION_TYPE_CARD_CHIPS_IN_A_GAME_3:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 0;
                if (_type == MISSION_TYPE_CARD_CHIPS_IN_A_GAME_1) targetInt = 1;
                if (_type == MISSION_TYPE_CARD_CHIPS_IN_A_GAME_2) targetInt = 2;
                if (_type == MISSION_TYPE_CARD_CHIPS_IN_A_GAME_3) targetInt = 3;
                
                if (targetInt == 1)
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_CHIPS_IN_A_GAME_SINGLE",nil),targetInt];
                else
                    self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_CHIPS_IN_A_GAME_PLURAL",nil),targetInt];
                
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        self.string = self.baseString;
                    else{
                        int left = targetInt - state.pokerChipsTotal;
                        [self updateTextWithLeft:left];
                        if (left <= 0)
                            [self didAccomplish];
                    }
                });
                
                break;
            }
            case MISSION_TYPE_CARD_CASH_IN:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_CASH_IN",nil);
                break;
            }
            case MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS",nil);
                break;
            }
            case MISSION_TYPE_CARD_RED_CARDS:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_RED_CARDS",nil);
                break;
            }
            case MISSION_TYPE_CARD_BLACK_CARDS:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_BLACK_CARDS",nil);
                break;
            }
            case MISSION_TYPE_CARD_FIGURES:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_FIGURES",nil);
                break;
            }
            case MISSION_TYPE_CARD_CHIPS_ACCUMULATION_1:
            case MISSION_TYPE_CARD_CHIPS_ACCUMULATION_2:
            case MISSION_TYPE_CARD_CHIPS_ACCUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 0;
                if (_type == MISSION_TYPE_CARD_CHIPS_ACCUMULATION_1) targetInt = 12;
                if (_type == MISSION_TYPE_CARD_CHIPS_ACCUMULATION_2) targetInt = 20;
                if (_type == MISSION_TYPE_CARD_CHIPS_ACCUMULATION_3) targetInt = 30;
                __block int accumInt = (int)[[NSUserDefaults standardUserDefaults] integerForKey:[self keyAux]];
                int left = targetInt - accumInt;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_CHIPS_ACCUMULATION",nil),targetInt];
                [self updateTextWithLeft:left];
                _block = Block_copy(^(GameState *state){
                    int totalChips = state.pokerChipsTotal + accumInt;
                    int left = targetInt - totalChips;
                    [self updateTextWithLeft:left];
                    if (left <= 0)
                        [self didAccomplish];
                    if (state.isGameOver){
                        [[NSUserDefaults standardUserDefaults] setInteger:totalChips forKey:[self keyAux]];
                        accumInt = totalChips;
                    }
                });
                break;
            }
            case MISSION_TYPE_CARD_PLAY_IN_A_ROW:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 5; // Si cambias esto cámbialo también en el update de las cartas
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_PLAY_IN_A_ROW",nil),targetInt];
                break;
            }
            case MISSION_TYPE_CARD_PRIZES_ACCUMULATION:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 10; // Si cambias esto cámbialo también en el update de las cartas
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_PRIZES_ACCUMULATION",nil),targetInt];
                
                int accumInt = (int)[[NSUserDefaults standardUserDefaults] integerForKey:[self keyAux]];
                int left = targetInt - accumInt;
                [self updateTextWithLeft:left];
                break;
            }
            case MISSION_TYPE_CARD_ONE_PRIZE:
            {
                _icon = MISSION_ICON_TYPE_4;
                self.baseString = NSLocalizedString(@"MISSION_TYPE_CARD_ONE_PRIZE",nil);
                break;
            }
            default:
                break;
        }
        
        if (!self.string)
            self.string = self.baseString;
    }
    return self;
}

-(void)dealloc{
    self.string = nil;
    self.baseString = nil;
    if (_block)
        Block_release(_block);
    [super dealloc];
}

-(MissionType)type{
    return _type;
}

-(NSString*)keyAccomplished{
    return [NSString stringWithFormat:@"Mission%dAccomplished",_type];
}

-(NSString*)keyRewarded{
    return [NSString stringWithFormat:@"Mission%dRewarded",_type];
}

-(NSString*)keyAux{
    return [NSString stringWithFormat:@"Mission%dAux",_type];
}

-(void)updateWithGameState:(GameState*)state{
    if (!_isAccomplished && _block)
        _block(state);
}

-(void)updateWithCards:(NSArray*)cards pokerHand:(PokerCardHand*)hand numPlaysInARow:(int)numPlays{
    if (_isAccomplished)
        return;
    
    switch (_type) {
        case MISSION_TYPE_CARD_ONE_PAIR:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_10:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 10){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_J:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 11){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_Q:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 12){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_K:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 13){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_ACES:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_ONE_PAIR){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 1){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_TWO_PAIRS:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_TWO_PAIR)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_THREE_OF_A_KIND:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_THREE_OF_A_KIND)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_THREE_OF_A_KIND){
                NSArray *handCards = [hand handCards];
                for (PokerCardConfig *c in handCards){
                    if (c.number == 1){
                        [self didAccomplish];
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_FLUSH:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_FLUSH)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_STRAIGHT:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_STRAIGHT)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_FULL_HOUSE:
        {
            if ([hand hand] == POKER_CARD_HAND_TYPE_FULL_HOUSE)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_JOKER:
        {
            for (PokerCardConfig *c in cards){
                if (c.suit == POKER_CARD_SUIT_TYPE_JOKER){
                    [self didAccomplish];
                    break;
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_JQK:
        {
            BOOL j,q,k;
            j = q = k = NO;
            for (PokerCardConfig *c in cards){
                j |= c.number == 11;
                q |= c.number == 12;
                k |= c.number == 13;
            }
            if (j && q && k)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_BLACKJACK:
        {
            int sum = 0;
            
            // Primero sin ases
            for (PokerCardConfig *c in cards){
                if (c.suit != POKER_CARD_SUIT_TYPE_JOKER){
                    switch (c.number) {
                        case 1:
                            break;
                        case 11:
                        case 12:
                        case 13:
                            sum += 10;
                            break;
                        default:
                            sum += c.number;
                            break;
                    }
                }
            }
            
            // Ahora los ases
            for (PokerCardConfig *c in cards){
                if (c.suit != POKER_CARD_SUIT_TYPE_JOKER && c.number == 1){
                    if (sum <= 10)
                        sum += 11;
                    else
                        sum += 1;
                }
            }
            
            if (sum == 21)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_RED_CARDS:
        {
            int numRedCards = 0;
            for (PokerCardConfig *c in cards)
                if ((c.suit == POKER_CARD_SUIT_TYPE_DIAMOND) || (c.suit == POKER_CARD_SUIT_TYPE_HEART))
                    numRedCards++;
            if (numRedCards >= 3)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_BLACK_CARDS:
        {
            int numBlackCards = 0;
            for (PokerCardConfig *c in cards)
                if ((c.suit == POKER_CARD_SUIT_TYPE_CLUB) || (c.suit == POKER_CARD_SUIT_TYPE_SPADE))
                    numBlackCards++;
            if (numBlackCards >= 3)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_FIGURES:
        {
            BOOL ok = YES;
            for (PokerCardConfig *c in cards)
                ok &= (c.number == 11 || c.number == 12 || c.number == 13);
            if (ok)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_PLAY_IN_A_ROW:
        {
            int targetInt = 5;
            if (numPlays >= targetInt)
                [self didAccomplish];
            break;
        }
        case MISSION_TYPE_CARD_PRIZES_ACCUMULATION:
        {
            if ([hand hand] != POKER_CARD_HAND_TYPE_NONE){
                int targetInt = 10;
                int accumInt = (int)[[NSUserDefaults standardUserDefaults] integerForKey:[self keyAux]];
                accumInt++;
                [[NSUserDefaults standardUserDefaults]setInteger:(NSInteger)accumInt forKey:[self keyAux]];
                int left = targetInt - accumInt;
                self.baseString = [NSString stringWithFormat:NSLocalizedString(@"MISSION_TYPE_CARD_PRIZES_ACCUMULATION",nil),targetInt];
                [self updateTextWithLeft:left];
                if (left <= 0)
                    [self didAccomplish];
                break;
            }
        }
        case MISSION_TYPE_CARD_ONE_PRIZE:
        {
            if ([hand hand] != POKER_CARD_HAND_TYPE_NONE){
                [self didAccomplish];
                break;
            }
        }
        default:
            break;
    }
}

-(void)updateBecauseCashInPokerChips{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_CARD_CASH_IN)
        [self didAccomplish];
}

-(void)updateBecauseDidPlayWithoutPokerChips{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS)
        [self didAccomplish];
}

-(void)updateBecauseDidShareVideo{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_SHARE_VIDEO)
        [self didAccomplish];
}

-(void)didAccomplish{
    [self setAccomplished:YES];
    self.string = self.baseString;
    [[NSNotificationCenter defaultCenter] postNotificationName:MissionAccomplishedNotification object:self userInfo:nil];
    [Achievement didAccomplishMission];
}

-(BOOL)isAccomplished{
    return _isAccomplished;
}

-(void)setAccomplished:(BOOL)accomplished{
    _isAccomplished = accomplished;
    
    [SDCloudUserDefaults setBool:accomplished forKey:[self keyAccomplished]];
    [SDCloudUserDefaults synchronize];
}

-(BOOL)isRewarded{
    return _isRewarded;
}

-(void)setRewarded:(BOOL)rewarded{
    _isRewarded = rewarded;
    [SDCloudUserDefaults setBool:rewarded forKey:[self keyRewarded]];
    [SDCloudUserDefaults synchronize];
}

-(int)rewardCoins{
    return _rewardCoins;
}

-(int)rewardSpecialCoins{
    return _rewardSpecialCoins;
}

-(BOOL)needFishes{
    return ![self isAccomplished] && (_type == MISSION_TYPE_FISHES_IN_A_GAME_1 || _type == MISSION_TYPE_FISHES_IN_A_GAME_2 || _type == MISSION_TYPE_FISHES_IN_A_GAME_3);
}

-(BOOL)needRings{
    return _type == MISSION_TYPE_RINGS_IN_A_GAME;
}

-(BOOL)isTheFirstUpgradeWeaponMission{
    return _type == MISSION_TYPE_UPGRADE_WEAPON;
}

-(BOOL)mustUseVehicleToAccomplish:(VehicleType)vehicleType{
    switch (vehicleType) {
        case VEHICLE_TYPE_SHARK:
            return _type == MISSION_TYPE_USE_VEHICLE_SHARK;
            break;
        case VEHICLE_TYPE_PIRATE:
            return _type == MISSION_TYPE_RELEASE_KRAKEN;
            break;
        case VEHICLE_TYPE_POLICE:
            return _type == MISSION_TYPE_USE_VEHICLE_POLICE;
            break;
        case VEHICLE_TYPE_SURF:
            return _type == MISSION_TYPE_USE_VEHICLE_SURF;
            break;
        case VEHICLE_TYPE_HIPPO:
            return _type == MISSION_TYPE_USE_VEHICLE_HIPPO;
            break;
        case VEHICLE_TYPE_SUBMARINE:
            return _type == MISSION_TYPE_USE_VEHICLE_SUBMARINE;
            break;
        default:
            return NO;
            break;
    }
}

-(NSString*)text{
    return self.string;
}

-(void)updateTextWithLeft:(int)left{
    if (left > 0){
        if (left == 1)
            self.string = [NSString stringWithFormat:@"%@ %@",self.baseString, [NSString stringWithFormat:NSLocalizedString(@"MISSION_LEFT_SINGLE", nil),left]];
        else
            self.string = [NSString stringWithFormat:@"%@ %@",self.baseString, [NSString stringWithFormat:NSLocalizedString(@"MISSION_LEFT_PLURAL", nil),left]];
    }
    else
        self.string = self.baseString;
}

-(NSString*)icon{
    if ([self isAccomplished])
        return [self iconAccomplished];
    else
        return [self iconNotAccomplished];
}

-(NSString*)iconAccomplished{
    switch (_icon) {
        case MISSION_ICON_TYPE_1:
            return @"missionIcon01Accomplished.png";
            break;
        case MISSION_ICON_TYPE_2:
            return @"missionIcon02Accomplished.png";
            break;
        case MISSION_ICON_TYPE_3:
            return @"missionIcon03Accomplished.png";
            break;
        case MISSION_ICON_TYPE_4:
            return @"missionIcon04Accomplished.png";
            break;
        case MISSION_ICON_TYPE_5:
            return @"missionIcon05Accomplished.png";
            break;
        case MISSION_ICON_TYPE_6:
            return @"missionIcon06Accomplished.png";
            break;
        case MISSION_ICON_TYPE_7:
            return @"missionIcon07Accomplished.png";
            break;
        case MISSION_ICON_TYPE_8:
            return @"missionIcon08Accomplished.png";
            break;
        case MISSION_ICON_TYPE_9:
            return @"missionIcon09Accomplished.png";
            break;
        case MISSION_ICON_TYPE_10:
            return @"missionIcon10Accomplished.png";
            break;
        case MISSION_ICON_TYPE_11:
            return @"missionIcon11Accomplished.png";
            break;
        case MISSION_ICON_TYPE_12:
            return @"missionIcon12Accomplished.png";
            break;
        case MISSION_ICON_TYPE_13:
            return @"missionIcon13Accomplished.png";
            break;
        case MISSION_ICON_TYPE_14:
            return @"missionIcon14Accomplished.png";
            break;
        case MISSION_ICON_TYPE_15:
            return @"missionIcon15Accomplished.png";
            break;
        case MISSION_ICON_TYPE_16:
            return @"missionIcon16Accomplished.png";
            break;
        case MISSION_ICON_TYPE_17:
            return @"missionIcon17Accomplished.png";
            break;
        case MISSION_ICON_TYPE_18:
            return @"missionIcon18Accomplished.png";
            break;
        case MISSION_ICON_TYPE_19:
            return @"missionIcon19Accomplished.png";
            break;
        case MISSION_ICON_TYPE_20:
            return @"missionIcon20Accomplished.png";
            break;
        case MISSION_ICON_TYPE_21:
            return @"missionIcon21Accomplished.png";
            break;
        case MISSION_ICON_TYPE_22:
            return @"missionIcon22Accomplished.png";
            break;
        case MISSION_ICON_TYPE_23:
            return @"missionIcon23Accomplished.png";
            break;
        case MISSION_ICON_TYPE_24:
            return @"missionIcon24Accomplished.png";
            break;
        case MISSION_ICON_TYPE_25:
            return @"missionIcon25Accomplished.png";
            break;
        case MISSION_ICON_TYPE_26:
            return @"missionIcon26Accomplished.png";
            break;
        case MISSION_ICON_TYPE_27:
            return @"missionIcon27Accomplished.png";
            break;
        default:
            return @"";
            break;
    }
}

-(NSString*)iconNotAccomplished{
    switch (_icon) {
        case MISSION_ICON_TYPE_1:
            return @"missionIcon01NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_2:
            return @"missionIcon02NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_3:
            return @"missionIcon03NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_4:
            return @"missionIcon04NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_5:
            return @"missionIcon05NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_6:
            return @"missionIcon06NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_7:
            return @"missionIcon07NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_8:
            return @"missionIcon08NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_9:
            return @"missionIcon09NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_10:
            return @"missionIcon10NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_11:
            return @"missionIcon11NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_12:
            return @"missionIcon12NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_13:
            return @"missionIcon13NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_14:
            return @"missionIcon14NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_15:
            return @"missionIcon15NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_16:
            return @"missionIcon16NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_17:
            return @"missionIcon17NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_18:
            return @"missionIcon18NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_19:
            return @"missionIcon19NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_20:
            return @"missionIcon20NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_21:
            return @"missionIcon21NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_22:
            return @"missionIcon22NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_23:
            return @"missionIcon23NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_24:
            return @"missionIcon24NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_25:
            return @"missionIcon25NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_26:
            return @"missionIcon26NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_27:
            return @"missionIcon27NotAccomplished.png";
            break;
        default:
            return @"";
            break;
    }
}

-(int)price{
    return _specialCoins;
}

-(int)ident{
    return _type;
}

-(int)order{
    return _order;
}

-(void)reset{
    [self setAccomplished:NO];
    [self setRewarded:NO];
    [SDCloudUserDefaults removeObjectForKey:[self keyAux]];
}

-(BOOL)isEqual:(id)object{
    return _type == [((Mission*)object)type];
}

-(NSString*)description{
    return [NSString stringWithFormat:@"%d. (%d) %@",[self order],[self ident],self.baseString];
}

+(void)printMissions{
    NSString *string = @"";
    NSArray *ranges = [MilitarRange ranges];
    for (MilitarRange *r in ranges){
        string = [string stringByAppendingFormat:@"%@\n",r];
        NSArray *missions = [Mission missionsWithRange:[r type]];
        for (Mission *m in missions)
            string = [string stringByAppendingFormat:@"\t%@\n",m];
    }
    NSLog(@"%@",string);
}

+(BOOL)checkMissions{
    NSMutableArray *allMissions = [NSMutableArray arrayWithCapacity:100];
    NSArray *ranges = [MilitarRange ranges];
    for (MilitarRange *r in ranges){
        NSArray *missions = [Mission missionsWithRange:[r type]];
        [allMissions addObjectsFromArray:missions];
    }
    
    BOOL ok = YES;
    int count = (int)[allMissions count];
    for (int i = 0; i < count; i++){
        Mission *m1 = [allMissions objectAtIndex:i];
        for (int j = i+1; j < count; j++){
            Mission *m2 = [allMissions objectAtIndex:j];
            if ([m1 isEqual:m2]){
                NSLog(@"Misiones iguales:\n%@\n%@",m1,m2);
                ok = NO;
            }
        }
    }
    return ok;
}

@end
