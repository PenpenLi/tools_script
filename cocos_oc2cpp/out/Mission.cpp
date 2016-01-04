//
//  Mission.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 26/11/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#include "Mission.h"
#include "GameState.h"
#include "SDCloudUserDefaults.h"
#include "FacebookManager.h"
#include "GameManager.h"
#include "KamcordHelper.h"
#include "ParamManager.h"
#include "Achievement.h"
std::string const MissionAccomplishedNotification = "MissionAccomplishedNotification";

typedef enum {
    MISSION_TYPE_MONEY_IN_A_GAME_1 = 10000,                 // Recoge X dinero en una partida
    MISSION_TYPE_MONEY_IN_A_GAME_2 = 10001,
    MISSION_TYPE_MONEY_IN_A_GAME_3 = 10002,
    MISSION_TYPE_MONEY_IN_A_GAME_4 = 10003,
    
    MISSION_TYPE_MONEY_AUMULATION_1 = 10010,              // Recoge X dinero en total
    MISSION_TYPE_MONEY_AUMULATION_2 = 10011,
    MISSION_TYPE_MONEY_AUMULATION_3 = 10012,
    MISSION_TYPE_MONEY_AUMULATION_4 = 10013,
    
    MISSION_TYPE_NO_MONEY = 10020,                          // Completa una partida sin coger dinero
    
    MISSION_TYPE_REACH_DISTANCE_1 = 10030,                  // Llega a la distancia en una partida
    MISSION_TYPE_REACH_DISTANCE_2 = 10031,
    MISSION_TYPE_REACH_DISTANCE_3 = 10032,
    MISSION_TYPE_REACH_DISTANCE_4 = 10033,
    
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_1 = 10040,         // Completa una partida entre [x,y] metros
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_2 = 10041,
    MISSION_TYPE_REACH_DISTANCE_INTERVAL_3 = 10042,
    
    MISSION_TYPE_DISTANCE_AUMULATION_1 = 10050,           // Acumula X metros
    MISSION_TYPE_DISTANCE_AUMULATION_2 = 10051,
    MISSION_TYPE_DISTANCE_AUMULATION_3 = 10052,
    
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
    
    MISSION_TYPE_DODGE_MINE = 10100,                        // Esquiva 1 mina (es decir, false la destruyas)
    
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
    
    MISSION_TYPE_KILL_ENEMIES_AUMULATION_1 = 10160,       // Mata a X enemigos en total
    MISSION_TYPE_KILL_ENEMIES_AUMULATION_2 = 10161,
    MISSION_TYPE_KILL_ENEMIES_AUMULATION_3 = 10162,
    
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1 = 10170,   // Mata a X enemigos usando el arma Z
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2 = 10171,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3 = 10172,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4 = 10173,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5 = 10174,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_6 = 10175,
    MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7 = 10176,   // false UTILIZADO
    
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1 = 10190,       // Mata a X submarinos en una partida
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_2 = 10191,       // false UTILIZADO
    MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_3 = 10192,       // false UTILIZADO
    
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1 = 10200,      // Mata a X helicópteros en una partida
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2 = 10201,
    MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_3 = 10202,      // false UTILIZADO
    
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1 = 10210,          // Mata a X bombarderos en una partida
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2 = 10211,
    MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_3 = 10212,
    
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_1 = 10220,            // Mata a X lanchas en una partida
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_2 = 10221,
    MISSION_TYPE_KILL_BOATS_IN_A_GAME_3 = 10222,
    
    MISSION_TYPE_KILL_MINES_IN_A_GAME_1 = 10230,            // Mata a X minas en una partida
    MISSION_TYPE_KILL_MINES_IN_A_GAME_2 = 10231,            // false UTILIZADO
    MISSION_TYPE_KILL_MINES_IN_A_GAME_3 = 10232,            // false UTILIZADO
    
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
    
    MISSION_TYPE_CHANGE_AVATAR = 10390,                    // Usar un personaje que false sea Mambo
    
    MISSION_TYPE_CHANGE_VEHICLE = 10400,                    // Usar un vehículo que false sea la barca
    
    MISSION_TYPE_PLAY_GAMES = 10410,                        // Jugar X partidas
    
    MISSION_TYPE_LEVEL_2_WITHOUT_KILLS = 10420,             // Llega al nivel 2 sin matar a ningún enemigo
    
    MISSION_TYPE_AURACY_1 = 10430,                        // Acaba con un porcentaje de acierto
    
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
    MISSION_TYPE_CARD_CHIPS_AUMULATION_1 = 20029,         // Consigue X fichas acumuladas
    MISSION_TYPE_CARD_CHIPS_AUMULATION_2 = 20030,
    MISSION_TYPE_CARD_CHIPS_AUMULATION_3 = 20031,
    MISSION_TYPE_CARD_PLAY_IN_A_ROW = 20032,                // Juega X veces seguidas
    MISSION_TYPE_CARD_PRIZES_AUMULATION = 20033,          // Acumula X premios
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
@property (copy)std::string string;
@property (copy)std::string baseString;


@implementation Mission{
    MissionType _type;
    MissionIconType _icon;
    bool _isAccomplished;
    bool _isRewarded;
    int _rewardCoins;
    int _rewardSpecialCoins;
    int _specialCoins;
    int _order;
    MissionUpdateBlock _block;
}

NSMutableDictionary* Mission::allMissions() 
{
    static NSMutableDictionary *dictionary = NULL;
    if (!dictionary)
        dictionary = NSMutableDictionary::initWithCapacity(100);
    return dictionary;
}
Vector<T>* Mission::missionsWithRange(MilitarRangeType range) 
{
    auto array = new Vector<T>;
    
    switch (range) {
        case MILITAR_RANGE_TYPE_01_PRIVATE:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_1, 1, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_STAGE_2, 2, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_PLAY_GAMES, 3, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_JUMPS_IN_A_GAME_1, 4, 2));
            break;
        case MILITAR_RANGE_TYPE_02_PRIVATE_FIRST_CLASS:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_WEAPON, 5, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DIVES_IN_A_GAME_1, 6, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_AUMULATION_1, 7, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_DISTANCE_1, 8, 2));
            break;
        case MILITAR_RANGE_TYPE_03_CORPORAL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_IN_A_GAME_1, 9, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_VEHICLE_SHARK, 10, 15));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_FISHES_IN_A_GAME_1, 11, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_CHIPS_IN_A_GAME_1, 12, 2));
            break;
        case MILITAR_RANGE_TYPE_04_SERGEANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1, 13, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_AUMULATION_1, 14, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_AUMULATION_1, 15, 2));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_GADGET_1, 16, 4));
            break;
        case MILITAR_RANGE_TYPE_05_STAFF_SERGEANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_IN_A_GAME_2, 17, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_1, 18, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1, 19, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_ONE_PRIZE, 20, 4));
            break;
        case MILITAR_RANGE_TYPE_06_SERGEANT_FIRST_CLASS:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_2, 21, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_1, 22, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_FISHES_IN_A_GAME_2, 23, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_1, 24, 4));
            break;
        case MILITAR_RANGE_TYPE_07_MASTER_SERGEANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_AUMULATION_2, 25, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_1, 26, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_VEHICLE_POLICE, 27, 40));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_BLACK_CARDS, 28, 4));
            break;
        case MILITAR_RANGE_TYPE_08_FIRST_SERGEANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_COMBOS_IN_A_GAME_1, 29, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_BOATS_IN_A_GAME_1, 30, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_DISTANCE_2, 31, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_GADGET_2, 32, 15));
            break;
        case MILITAR_RANGE_TYPE_09_SERGEANT_MAJOR:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_TOUCH_BOAT, 33, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_AUMULATION_2, 34, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_1, 35, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2, 36, 25));
            break;
        case MILITAR_RANGE_TYPE_10_COMMAND_SERGEANT_MAJOR:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_IN_A_GAME_3, 37, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_MINES_IN_A_GAME_1, 38, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW_2, 39, 4));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_ONE_PAIR, 40, 4));
            break;
        case MILITAR_RANGE_TYPE_11_SERGEANT_MAJOR_OF_THE_ARMY:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2, 41, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_AUMULATION_2, 42, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_VEHICLE_SURF, 43, 80));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS, 44, 6));
            break;
        case MILITAR_RANGE_TYPE_12_SECOND_LIEUTENANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_HELICOPTERS_IN_A_GAME_2, 45, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_LEVEL_2_WITHOUT_KILLS, 46, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MATRIX, 47, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_GADGET_3, 48, 15));
            break;
        case MILITAR_RANGE_TYPE_13_FIRST_LIEUTENANT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_BOMBERS_IN_A_GAME_2, 49, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REBORN, 50, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DODGE_MINE, 51, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3, 52, 35));
            break;
        case MILITAR_RANGE_TYPE_14_CAPTAIN:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3, 53, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_BOATS_IN_A_GAME_2, 54, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_FISHES_IN_A_GAME_3, 55, 6));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_RED_CARDS, 56, 6));
            break;
        case MILITAR_RANGE_TYPE_15_MAJOR:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_PERFECT, 57, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_PARACHUTIST_IN_A_GAME_2, 58, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_VEHICLE_HIPPO, 59, 140));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_AUMULATION_3, 60, 8));
            break;
        case MILITAR_RANGE_TYPE_16_LIEUTENANT_COLONEL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW_2, 61, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MERCY, 62, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_1, 63, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_GADGET_4, 64, 80));
            break;
        case MILITAR_RANGE_TYPE_17_COLONEL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_3, 65, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DISTANCE_AUMULATION_3, 66, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_AUMULATION_4, 67, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4, 68, 35));
            break;
        case MILITAR_RANGE_TYPE_18_BRIGADIER_GENERAL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4, 69, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_DISTANCE_3, 70, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_2, 71, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_CHIPS_IN_A_GAME_2, 72, 8));
            break;
        case MILITAR_RANGE_TYPE_19_MAJOR_GENERAL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_AUMULATION_3, 73, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_COMBOS_IN_A_GAME_2, 74, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_TOUCH_SUBMARINE, 75, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_VEHICLE_SUBMARINE, 76, 200));
            break;
        case MILITAR_RANGE_TYPE_20_LIEUTENANT_GENERAL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5, 77, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MONEY_IN_A_GAME_4, 78, 8));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_USE_GADGET_5, 79, 100));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_TWO_PAIRS, 80, 8));
            break;
        case MILITAR_RANGE_TYPE_21_GENERAL:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_SUBMARINES_IN_A_GAME_1, 81, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS_3, 82, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5, 83, 75));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_THREE_OF_A_KIND, 84, 10));
            break;
        case MILITAR_RANGE_TYPE_22_GENERAL_OF_THE_ARMY:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7, 85, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS_1, 86, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME_1, 87, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_CHIPS_AUMULATION_1, 88, 10));
            break;
        case MILITAR_RANGE_TYPE_23_LEGION_OF_MERIT:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_MAX_VELOCITY_TIME, 89, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_DISTANCE_INTERVAL_1, 90, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_MINE_USING_GADGET, 91, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_PLAY_IN_A_ROW, 92, 10));
            break;
        case MILITAR_RANGE_TYPE_24_SILVER_STAR:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_KILL_ENEMIES_IN_A_GAME_4, 93, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_STAGE_WITHOUT_HURT, 94, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_DIVE_MINES, 95, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_PRIZES_AUMULATION, 96, 10));
            break;
        case MILITAR_RANGE_TYPE_25_SUPERIOR_SERVICE:
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1, 97, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS, 98, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_REACH_DISTANCE_4, 99, 10));
            array->pushBack(Mission::missionWithType(MISSION_TYPE_CARD_FLUSH, 100, 10));
            break;
        case MILITAR_RANGE_TYPE_26_DISTINGUISHED_SERVICE:
            break;
        default:
            NSAssert(false,"MilitarRange:initWithType Rango desconocido");
            break;
    }
    
    return array;
}
Mission* Mission::missionWithType(MissionType type, int order, int specialCoins) 
 {
    NSMutableDictionary *dict = this->allMissions();
    NSNumber *key = NSNumber::numberWithInt(type);
   Mission* obj = dict->objectForKey(key);
    if (!obj){
        obj = [Mission::initWithType(type, order, specialCoins]autorelease);
        dict->setObject(obj, key);
    }
    return obj;           
}
Mission* Mission::initWithType(MissionType aType, int anOrder, int sp) 
 {
    this = super->init();
    if (this){
        this->string = NULL;
        this->baseString = NULL;
        _type = aType;
        _order = anOrder;
        _icon = MISSION_ICON_TYPE_1;
        _isAccomplished = SDCloudUserDefaults::boolForKey(this->keyAccomplished());
        _isRewarded = SDCloudUserDefaults::boolForKey(this->keyRewarded());
        _rewardCoins = [[ParamManager::sharedManager()->objectForKey("MissionReward"]objectForKey:"coins"]intValue);
        _rewardSpecialCoins = [[ParamManager::sharedManager()->objectForKey("MissionReward"]objectForKey:"specialCoins"]intValue);
        _specialCoins = sp;
        _block = NULL;
    
        
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_MONEY_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.coins;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_MONEY_AUMULATION_1:
            case MISSION_TYPE_MONEY_AUMULATION_2:
            case MISSION_TYPE_MONEY_AUMULATION_3:
            case MISSION_TYPE_MONEY_AUMULATION_4:
            {
                _icon = MISSION_ICON_TYPE_6;
                int targetInt = 0;
                if (_type == MISSION_TYPE_MONEY_AUMULATION_1) targetInt = 1000;
                if (_type == MISSION_TYPE_MONEY_AUMULATION_2) targetInt = 2000;
                if (_type == MISSION_TYPE_MONEY_AUMULATION_3) targetInt = 3000;
                if (_type == MISSION_TYPE_MONEY_AUMULATION_4) targetInt = 5000;
                __block int accumInt = (int)NSUserDefaults::standardUserDefaults()->integerForKey(this->keyAux());
                int left = targetInt - accumInt;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_MONEY_AUMULATION",NULL),targetInt);
                this->updateTextWithLeft(left);
                _block = Block_copy(^(GameState *state){
                    int totalCoins = state.coins + accumInt;
                    int left = targetInt - totalCoins;
                    this->updateTextWithLeft(left);
                    if (left <= 0)
                        this->didAccomplish();
                    if (state.isGameOver){, :standardUserDefaults()->setInteger(totalCoins, this->keyAux());
                        accumInt = totalCoins;
                    }
                });
                break;
            }
            case MISSION_TYPE_NO_MONEY:
            {
                _icon = MISSION_ICON_TYPE_6;
                this->baseString = NSLocalizedString("MISSION_TYPE_NO_MONEY",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.coins == 0)
                        this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_REACH_DISTANCE",NULL),(int)targetFloat);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = (int)(targetFloat - state.distance);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_REACH_DISTANCE_INTERVAL",NULL),(int)targetRange.location,(int)(targetRange.location+targetRange.length));
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver){
                        if (NSLocationInRange((int)state.distance, targetRange))
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_DISTANCE_AUMULATION_1:
            case MISSION_TYPE_DISTANCE_AUMULATION_2:
            case MISSION_TYPE_DISTANCE_AUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_2;
                float targetFloat = 0.0f;
                if (_type == MISSION_TYPE_DISTANCE_AUMULATION_1) targetFloat = 10000.0f;
                if (_type == MISSION_TYPE_DISTANCE_AUMULATION_2) targetFloat = 20000.0f;
                if (_type == MISSION_TYPE_DISTANCE_AUMULATION_3) targetFloat = 50000.0f;
                __block float accumFloat = NSUserDefaults::standardUserDefaults()->floatForKey(this->keyAux());
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DISTANCE_AUMULATION",NULL),(int)targetFloat);
                this->updateTextWithLeft((int)(targetFloat - accumFloat));
                _block = Block_copy(^(GameState *state){
                    float totalDistance = state.distance + accumFloat;
                    int left = (int)(targetFloat - totalDistance);
                    if (totalDistance == 0.0f)
                        this->string = this->baseString;
                    else
                        this->updateTextWithLeft(left);
                    if (left <= 0)
                        this->didAccomplish();
                    if (state.isGameOver){, :standardUserDefaults()->setFloat(totalDistance, this->keyAux());
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DISTANCE_WITHOUT_JUMP_IN_A_ROW",NULL),(int)targetFloat);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        lastJump = 0.0f;
                        numJumps = 0;
                        this->string = this->baseString;
                    }
                    else{
                        if (state.jumps != numJumps){
                            numJumps = state.jumps;
                            lastJump = state.distance;
                        }
                        float totalDistance = state.distance - lastJump;
                        int left = (int)(targetFloat - totalDistance);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DISTANCE_WITHOUT_HURT_IN_A_ROW",NULL),(int)targetFloat);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        lastHurt = 0.0f;
                        numLives = state.maxLife;
                        this->string = this->baseString;
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
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_JUMPS_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.jumps;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DIVES_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.dives;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });

                break;
            }
            case MISSION_TYPE_DODGE_MINE:
            {
                _icon = MISSION_ICON_TYPE_16;
                this->baseString = NSLocalizedString("MISSION_TYPE_DODGE_MINE",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state->numDodgesWithEntityType(GAME_ENTITY_TYPE_ENEMY_MINE) > 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_TOUCH_SUBMARINE:
            {
                _icon = MISSION_ICON_TYPE_17;
                this->baseString = NSLocalizedString("MISSION_TYPE_TOUCH_SUBMARINE",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.submarineTouched)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_TOUCH_BOAT:
            {
                _icon = MISSION_ICON_TYPE_15;
                this->baseString = NSLocalizedString("MISSION_TYPE_TOUCH_BOAT",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.boatTouched)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_TOUCH_HELICOPTER:
            {
                _icon = MISSION_ICON_TYPE_11;
                this->baseString = NSLocalizedString("MISSION_TYPE_TOUCH_HELICOPTER",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.helicopterTouched)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_PEACEMAKER:
            {
                _icon = MISSION_ICON_TYPE_21;
                this->baseString = NSLocalizedString("MISSION_TYPE_PEACEMAKER",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.numKills == 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_NINJA:
            {
                _icon = MISSION_ICON_TYPE_5;
                this->baseString = NSLocalizedString("MISSION_TYPE_NINJA",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.numKills > 0 && state.playerShotHits == 0 && state.playerShotMisses == 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_USE_A_GADGET:
            {
                _icon = MISSION_ICON_TYPE_8;
                this->baseString = NSLocalizedString("MISSION_TYPE_USE_A_GADGET",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && state->numGadgetsEnabled() > 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_USE_3_GADGETS_IN_A_ROW:
            {
                _icon = MISSION_ICON_TYPE_8;
                this->baseString = NSLocalizedString("MISSION_TYPE_USE_3_GADGETS_IN_A_ROW",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && state->numGadgetsEnabled() == 3)
                        this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMIES_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKills();
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_ENEMIES_AUMULATION_1:
            case MISSION_TYPE_KILL_ENEMIES_AUMULATION_2:
            case MISSION_TYPE_KILL_ENEMIES_AUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_3;
                int targetInt = 0;
                if (_type == MISSION_TYPE_KILL_ENEMIES_AUMULATION_1) targetInt = 200;
                if (_type == MISSION_TYPE_KILL_ENEMIES_AUMULATION_2) targetInt = 500;
                if (_type == MISSION_TYPE_KILL_ENEMIES_AUMULATION_3) targetInt = 1000;
                __block int accumInt = (int)NSUserDefaults::standardUserDefaults()->integerForKey(this->keyAux());
                int left = targetInt - accumInt;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMIES_AUMULATION",NULL),targetInt);
                this->updateTextWithLeft(left);
                _block = Block_copy(^(GameState *state){
                    int totalKills = state->numKills() + accumInt;
                    int left = targetInt - totalKills;
                    this->updateTextWithLeft(left);
                    if (left <= 0)
                        this->didAccomplish();
                    if (state.isGameOver){, :standardUserDefaults()->setInteger(totalKills, this->keyAux());
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
               std::string substring;
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_1){
                    substring = NSLocalizedString("WEAPON_PISTOL_NAME",NULL);
                    gunType1 = GUN_TYPE_PISTOL;
                    targetInt = 30;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_2){
                    substring = NSLocalizedString("WEAPON_HEAVY_MACHINE_GUN_NAME",NULL);
                    gunType1 = GUN_TYPE_HEAVY_MACHINE_GUN;
                    gunType2 = GUN_TYPE_HEAVY_MACHINE_GUN_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_3){
                    substring = NSLocalizedString("WEAPON_ROCKET_LAUNCHER_NAME",NULL);
                    gunType1 = GUN_TYPE_ROCKET;
                    gunType2 = GUN_TYPE_ROCKET_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_4){
                    substring = NSLocalizedString("WEAPON_FLAME_SHOT_NAME",NULL);
                    gunType1 = GUN_TYPE_FLAME_SHOT;
                    gunType2 = GUN_TYPE_FLAME_SHOT_GRAPE_SHOT;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_5){
                    substring = NSLocalizedString("WEAPON_SPREAD_GUN_NAME",NULL);
                    gunType1 = GUN_TYPE_SPREAD_GUN;
                    gunType2 = GUN_TYPE_SPREAD_GUN_CARTRIDGE;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_6){
                    substring = NSLocalizedString("WEAPON_LASER_NAME",NULL);
                    gunType1 = GUN_TYPE_LASER;
                }
                if (_type == MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME_7){
                    substring = NSLocalizedString("WEAPON_CLUSTER_NAME",NULL);
                    gunType1 = GUN_TYPE_CLUSTER;
                    gunType2 = GUN_TYPE_CLUSTER_GRAPE_SHOT;
                }
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMIES_WEAPON_IN_A_GAME",NULL),targetInt,substring);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int numKills1 = 0, numKills2 = 0;
                        if (gunType1 != GUN_TYPE_NONE)
                            numKills1 = state->numKillsWithGunType(gunType1);
                        if (gunType2 != GUN_TYPE_NONE)
                            numKills2 = state->numKillsWithGunType(gunType2);
                        int left = targetInt - (numKills1 + numKills2);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_SUBMARINE" : "ENEMY_SUBMARINE_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_SUBMARINE);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_HELICOPTER" : "ENEMY_HELICOPTER_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_HELICOPTER);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_BOMBER" : "ENEMY_BOMBER_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_BOMBER);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_BOAT" : "ENEMY_BOAT_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_BOAT);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_MINE" : "ENEMY_MINE_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_MINE);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_ENEMY_IN_A_GAME",NULL),targetInt,NSLocalizedString(targetInt == 1 ? "ENEMY_PARACHUTIST" : "ENEMY_PARACHUTIST_PLURAL",NULL));
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsWithEntityType(GAME_ENTITY_TYPE_ENEMY_PARACHUTIST);
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_MERCY:
            {
                _icon = MISSION_ICON_TYPE_5;
                int targetInt = 20;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_MERCY",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numDodges();
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_MATRIX:
            {
                _icon = MISSION_ICON_TYPE_1;
                int targetInt = 30;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_MATRIX",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numKillsInBulletTime();
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_REBORN:
            {
                _icon = MISSION_ICON_TYPE_12;
                this->baseString = NSLocalizedString("MISSION_TYPE_REBORN",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.reborns > 0)
                        this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_FISHES_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.fishes;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_RINGS_IN_A_GAME:
            {
                _icon = MISSION_ICON_TYPE_5;
                int targetInt = 10;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_RINGS_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.rings;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_COMBOS_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (state->maxCombo() > targetInt)
                        this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_BUY_POWER_UP",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numPowerUpsBuyed();
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_JUMP_MINES:
            {
                _icon = MISSION_ICON_TYPE_16;
                int targetInt = 5;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_JUMP_MINES",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.minesJumped;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_DIVE_MINES:
            {
                _icon = MISSION_ICON_TYPE_16;
                int targetInt = 3;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DIVE_MINES",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.minesDived;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_RELEASE_KRAKEN:
            {
                _icon = MISSION_ICON_TYPE_9;
               std::string pirateName = NSLocalizedString("VEHICLE_PIRATE_NAME",NULL);
               std::string missionText = NSLocalizedString("MISSION_TYPE_RELEASE_KRAKEN",NULL);
                this->baseString = NSString::stringWithFormat(missionText,pirateName);
                _block = Block_copy(^(GameState *state){
                    if (state.isKrakenReleased)
                        this->didAccomplish();
                });
                break;
            }
            /*
            case MISSION_TYPE_ENEMY_BITTEN:
            {
                _icon = MISSION_ICON_TYPE_24;
               std::string sharkName = NSLocalizedString("VEHICLE_SHARK_NAME",NULL);
               std::string missionText = NSLocalizedString("MISSION_TYPE_ENEMY_BITTEN",NULL);
                this->baseString = NSString::stringWithFormat(missionText,sharkName);
                _block = Block_copy(^(GameState *state){
                    if (state.isEnemyBitten)
                        this->didAccomplish();
                });
                break;
            }
            */
            case MISSION_TYPE_ENEMY_ELECTROCUTED:
            {
                _icon = MISSION_ICON_TYPE_7;
               std::string weaponName = NSLocalizedString("WEAPON_PLASMA_NAME",NULL);
               std::string missionText = NSLocalizedString("MISSION_TYPE_ENEMY_ELECTROCUTED",NULL);
                this->baseString = NSString::stringWithFormat(missionText,weaponName);
                _block = Block_copy(^(GameState *state){
                    if (state.isEnemyElectrocuted)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_LOGIN_FACEBOOK:
            {
                _icon = MISSION_ICON_TYPE_10;
                this->baseString = NSLocalizedString("MISSION_TYPE_LOGIN_FACEBOOK",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted && FacebookManager->sharedManager()->isLogged())
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_PERFECT:
            {
                _icon = MISSION_ICON_TYPE_3;
                __block int level = -1;
                __block int numDodges = -1;
                this->baseString = NSLocalizedString("MISSION_TYPE_PERFECT",NULL);
                _block = Block_copy(^(GameState *state){
                    if (level < 0){
                        if (state->currentLevelNumber() > 0){
                            level = state->currentLevelNumber();
                            numDodges = state->numDodges();
                        }
                    }
                    else{
                        if (state->currentLevelNumber() != level){
                            if (level > 0 && numDodges == state->numDodges() && state->numKills() > 0)
                                this->didAccomplish();
                            else{
                                level = state->currentLevelNumber();
                                numDodges = state->numDodges();
                            }
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_CHANGE_AVATAR:
            {
                _icon = MISSION_ICON_TYPE_5;
                this->baseString = NSLocalizedString("MISSION_TYPE_CHANGE_AVATAR",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted &&, :sharedManager().avatarFirstPlayer != AVATAR_TYPE_MAMBO)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_CHANGE_VEHICLE:
            {
                _icon = MISSION_ICON_TYPE_2;
                this->baseString = NSLocalizedString("MISSION_TYPE_CHANGE_VEHICLE",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted &&, :sharedManager().vehicleFirstPlayer != VEHICLE_TYPE_BOAT)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_PLAY_GAMES:
            {
                _icon = MISSION_ICON_TYPE_1;
                int targetInt = 1;
                int left = targetInt - GameManager::sharedManager().gamesPlayed;
                
                if (targetInt == 1)
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_PLAY_GAME_SINGLE",NULL),targetInt);
                else
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_PLAY_GAME_PLURAL",NULL),targetInt);
                
                if (left != targetInt)
                    this->updateTextWithLeft(left);
                _block = Block_copy(^(GameState *state){
                    int left = targetInt - GameManager::sharedManager().gamesPlayed;
                    if (left != targetInt)
                        this->updateTextWithLeft(left);
                    if (left <= 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_LEVEL_2_WITHOUT_KILLS:
            {
                _icon = MISSION_ICON_TYPE_21;
                this->baseString = NSLocalizedString("MISSION_TYPE_LEVEL_2_WITHOUT_KILLS",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state->currentLevelNumber() == 2 && state->numKills() == 0)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_AURACY_1:
            {
                _icon = MISSION_ICON_TYPE_3;
                float targetFloat = 0.8f;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_AURACY",NULL),(int)(targetFloat*100.0f));
                _block = Block_copy(^(GameState *state){
                    if (state.isGameOver && state.playerShotHits > 0 && (state.playerShotHits / (state.playerShotHits+state.playerShotMisses)) >= targetFloat)
                        this->didAccomplish();
                });
                break;
            }
            case MISSION_TYPE_SHARE_VIDEO:
            {
                _icon = MISSION_ICON_TYPE_10;
                if (KamcordHelper::isEnabled())
                    this->baseString = NSLocalizedString("MISSION_TYPE_SHARE_VIDEO",NULL);
                else
                    this->baseString = NSLocalizedString("MISSION_TYPE_SHARE_PHOTO",NULL);
                break;
            }
            case MISSION_TYPE_UPGRADE_WEAPON:
            {
                _icon = MISSION_ICON_TYPE_7;
                this->baseString = NSLocalizedString("MISSION_TYPE_UPGRADE_WEAPON", NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if ([(WeaponConfig*)WeaponConfig::weaponWithType(WEAPON_TYPE_PISTOL]level) > 0)
                            this->didAccomplish();
                        else{
                           Vector<T>* weapons = [state->avatarForPlayer(1]weapons);
                            for (WeaponConfig *w in weapons)
                                if ([w level] > 0){
                                    this->didAccomplish();
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
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_WITH_DIFFERENT_WEAPONS",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state->numDifferentWeaponsUsedForKilling();
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_2:
            {
                _icon = MISSION_ICON_TYPE_2;
                this->baseString = NSLocalizedString("MISSION_TYPE_REACH_STAGE_2",NULL);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if (state->currentLevelNumber() == 2)
                            this->didAccomplish();
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
                GadgetConfig *gadget1 = NULL;
                GadgetConfig *gadget2 = NULL;
                GadgetConfig *gadget3 = NULL;
                if (_type == MISSION_TYPE_USE_GADGET_1){
                    gadget1 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_POWER_1);
                    gadget2 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_POWER_2);
                    gadget3 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_POWER_3);
                }
                else if (_type == MISSION_TYPE_USE_GADGET_2){
                    gadget1 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_SPEED_1);
                    gadget2 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_SPEED_2);
                    gadget3 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_SPEED_3);
                }
                else if (_type == MISSION_TYPE_USE_GADGET_3){
                    gadget1 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_AMMO_1);
                    gadget2 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_AMMO_2);
                    gadget3 = GadgetConfig::gadgetWithType(GADGET_TYPE_MORE_AMMO_3);
                }
                else if (_type == MISSION_TYPE_USE_GADGET_4){
                    gadget1 = GadgetConfig::gadgetWithType(GADGET_TYPE_MAGNET);
                }
                else if (_type == MISSION_TYPE_USE_GADGET_5){
                    gadget1 = GadgetConfig::gadgetWithType(GADGET_TYPE_AELERATOR);
                }
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_USE_GADGET",NULL),gadget1->name());
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if (gadget1->isEquipped())
                            this->didAccomplish();
                        else if (gadget2 && gadget2->isEquipped())
                            this->didAccomplish();
                        else if (gadget3 && gadget3->isEquipped())
                            this->didAccomplish();
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
                WeaponConfig *weapon = NULL;
                int level = 1;
                if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_1){
                    weapon = WeaponConfig::weaponWithType(WEAPON_TYPE_HEAVY_MACHINE_GUN);
                    level = 2;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_2){
                    weapon = WeaponConfig::weaponWithType(WEAPON_TYPE_FLAME_SHOT);
                    level = 3;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_3){
                    weapon = WeaponConfig::weaponWithType(WEAPON_TYPE_SPREAD_GUN);
                    level = 4;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_4){
                    weapon = WeaponConfig::weaponWithType(WEAPON_TYPE_ROCKET_LAUNCHER);
                    level = 4;
                }
                else if (_type == MISSION_TYPE_UPGRADE_CUSTOM_WEAPON_5){
                    weapon = WeaponConfig::weaponWithType(WEAPON_TYPE_HOMING_MISSILE);
                    level = 5;
                }
                
                if (level == 5)
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_UPGRADE_MAX_CUSTOM_WEAPON",NULL),weapon->name());
                else
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_UPGRADE_CUSTOM_WEAPON",NULL),weapon->name(),level);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        WeaponConfig *loadedWeapon = state->weaponForPlayer(1);
                        if (loadedWeapon->type() == weapon->type() && loadedWeapon->level() >= level)
                            this->didAccomplish();
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
                VehicleConfig *vehicle = NULL;
                if (_type == MISSION_TYPE_USE_VEHICLE_SHARK){
                    vehicle = VehicleConfig::vehicleWithType(VEHICLE_TYPE_SHARK);
                    _icon = MISSION_ICON_TYPE_24;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_POLICE){
                    vehicle = VehicleConfig::vehicleWithType(VEHICLE_TYPE_POLICE);
                    _icon = MISSION_ICON_TYPE_23;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_SURF){
                    vehicle = VehicleConfig::vehicleWithType(VEHICLE_TYPE_SURF);
                    _icon = MISSION_ICON_TYPE_26;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_HIPPO){
                    vehicle = VehicleConfig::vehicleWithType(VEHICLE_TYPE_HIPPO);
                    _icon = MISSION_ICON_TYPE_22;
                }
                else if (_type == MISSION_TYPE_USE_VEHICLE_SUBMARINE){
                    vehicle = VehicleConfig::vehicleWithType(VEHICLE_TYPE_SUBMARINE);
                    _icon = MISSION_ICON_TYPE_25;
                }
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_USE_VEHICLE",NULL),vehicle->name());
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if (vehicle->isEquipped())
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS_1:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 3;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_REACH_STAGE_WITHOUT_WEAPONS",NULL),targetInt,[[WeaponConfig, WEAPON_TYPE_PISTOL]name]);
                _block = Block_copy(^(GameState *state){
                    if (state.isGameStarted){
                        if (state->currentLevelNumber() >= targetInt && state->numLoadedWeapons() == state->numLoadedWeaponsWithType(WEAPON_TYPE_PISTOL)){
                            this->didAccomplish();
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME_1:
            {
                _icon = MISSION_ICON_TYPE_27;
                int targetInt = 10;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_DESTROY_ITEM_BOXES_IN_A_GAME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                        int left = targetInt - state.itemBoxesDestroyed;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_MAX_VELOCITY_TIME:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 30;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_MAX_VELOCITY_TIME",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                        int left = targetInt - fminf(state->timeAccelerating(),state->timeWithoutHit());
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_KILL_MINE_USING_GADGET:
            {
                _icon = MISSION_ICON_TYPE_16;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_KILL_MINE_USING_GADGET",NULL),[[GadgetConfig, GADGET_TYPE_MINE_CRUSHER]name]);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                        if (state.minesDestroyedWithGadget > 0)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_REACH_STAGE_WITHOUT_HURT:
            {
                _icon = MISSION_ICON_TYPE_2;
                int targetInt = 4;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_REACH_STAGE_WITHOUT_HURT",NULL),targetInt);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                        if (state->numHits() == 0 && state->currentLevelNumber() >= targetInt)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1:
            {
                _icon = MISSION_ICON_TYPE_12;
                this->baseString = NSLocalizedString("MISSION_TYPE_REBORN_2_TIMES_ON_STAGE_1",NULL);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                        if (state->currentLevelNumber() == 1){
                            int left = 2 - state.reborns;
                            this->updateTextWithLeft(left);
                            if (left <= 0)
                                this->didAccomplish();
                        }
                    }
                });
                break;
            }
            case MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS:
            {
                _icon = MISSION_ICON_TYPE_8;
                this->baseString = NSLocalizedString("MISSION_TYPE_PLAY_WITH_ALL_AVATAR_UPGRADED_WEAPONS",NULL);
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver){
                        this->string = this->baseString;
                    }
                    else{
                       Vector<T>* weapons = [state->avatarForPlayer(1]weapons);
                        bool ok = true;
                        for (WeaponConfig *w in weapons){
                            if ([w level] != 5){
                                ok = false;
                                break;
                            }
                        }
                        if (ok)
                            this->didAccomplish();
                    }
                });
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR",NULL);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_10:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR_10",NULL);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_J:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR_J",NULL);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_Q:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR_Q",NULL);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_K:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR_K",NULL);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PAIR_ACES:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PAIR_ACES",NULL);
                break;
            }
            case MISSION_TYPE_CARD_TWO_PAIRS:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_TWO_PAIRS",NULL);
                break;
            }
            case MISSION_TYPE_CARD_THREE_OF_A_KIND:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_THREE_OF_A_KIND",NULL);
                break;
            }
            case MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES",NULL);
                break;
            }
            case MISSION_TYPE_CARD_FLUSH:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_FLUSH",NULL);
                break;
            }
            case MISSION_TYPE_CARD_STRAIGHT:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_STRAIGHT",NULL);
                break;
            }
            case MISSION_TYPE_CARD_FULL_HOUSE:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_FULL_HOUSE",NULL);
                break;
            }
            case MISSION_TYPE_CARD_JOKER:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_JOKER",NULL);
                break;
            }
            case MISSION_TYPE_CARD_JQK:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_JQK",NULL);
                break;
            }
            case MISSION_TYPE_CARD_BLACKJACK:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_BLACK_JACK",NULL);
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
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_CHIPS_IN_A_GAME_SINGLE",NULL),targetInt);
                else
                    this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_CHIPS_IN_A_GAME_PLURAL",NULL),targetInt);
                
                _block = Block_copy(^(GameState *state){
                    if (!state.isGameStarted || state.isGameOver)
                        this->string = this->baseString;
                    else{
                        int left = targetInt - state.pokerChipsTotal;
                        this->updateTextWithLeft(left);
                        if (left <= 0)
                            this->didAccomplish();
                    }
                });
                
                break;
            }
            case MISSION_TYPE_CARD_CASH_IN:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_CASH_IN",NULL);
                break;
            }
            case MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS",NULL);
                break;
            }
            case MISSION_TYPE_CARD_RED_CARDS:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_RED_CARDS",NULL);
                break;
            }
            case MISSION_TYPE_CARD_BLACK_CARDS:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_BLACK_CARDS",NULL);
                break;
            }
            case MISSION_TYPE_CARD_FIGURES:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_FIGURES",NULL);
                break;
            }
            case MISSION_TYPE_CARD_CHIPS_AUMULATION_1:
            case MISSION_TYPE_CARD_CHIPS_AUMULATION_2:
            case MISSION_TYPE_CARD_CHIPS_AUMULATION_3:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 0;
                if (_type == MISSION_TYPE_CARD_CHIPS_AUMULATION_1) targetInt = 12;
                if (_type == MISSION_TYPE_CARD_CHIPS_AUMULATION_2) targetInt = 20;
                if (_type == MISSION_TYPE_CARD_CHIPS_AUMULATION_3) targetInt = 30;
                __block int accumInt = (int)NSUserDefaults::standardUserDefaults()->integerForKey(this->keyAux());
                int left = targetInt - accumInt;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_CHIPS_AUMULATION",NULL),targetInt);
                this->updateTextWithLeft(left);
                _block = Block_copy(^(GameState *state){
                    int totalChips = state.pokerChipsTotal + accumInt;
                    int left = targetInt - totalChips;
                    this->updateTextWithLeft(left);
                    if (left <= 0)
                        this->didAccomplish();
                    if (state.isGameOver){, :standardUserDefaults()->setInteger(totalChips, this->keyAux());
                        accumInt = totalChips;
                    }
                });
                break;
            }
            case MISSION_TYPE_CARD_PLAY_IN_A_ROW:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 5; // Si cambias esto cámbialo también en el update de las cartas
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_PLAY_IN_A_ROW",NULL),targetInt);
                break;
            }
            case MISSION_TYPE_CARD_PRIZES_AUMULATION:
            {
                _icon = MISSION_ICON_TYPE_4;
                int targetInt = 10; // Si cambias esto cámbialo también en el update de las cartas
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_PRIZES_AUMULATION",NULL),targetInt);
                
                int accumInt = (int)NSUserDefaults::standardUserDefaults()->integerForKey(this->keyAux());
                int left = targetInt - accumInt;
                this->updateTextWithLeft(left);
                break;
            }
            case MISSION_TYPE_CARD_ONE_PRIZE:
            {
                _icon = MISSION_ICON_TYPE_4;
                this->baseString = NSLocalizedString("MISSION_TYPE_CARD_ONE_PRIZE",NULL);
                break;
            }
            default:
                break;
        }
        
        if (!this->string)
            this->string = this->baseString;
    }
    return this;
}

void Mission::dealloc() 
{
    this->string = NULL;
    this->baseString = NULL;
    if (_block)
        Block_release(_block);
    super->dealloc();
}

MissionType Mission::type() 
{
    return _type;
}
std::string Mission::keyAccomplished() 
{
    return StringUtils::format("Mission%dAccomplished", _type);
}
std::string Mission::keyRewarded() 
{
    return StringUtils::format("Mission%dRewarded", _type);
}
std::string Mission::keyAux() 
{
    return StringUtils::format("Mission%dAux", _type);
}

void Mission::updateWithGameState(GameState* state) 
{
    if (!_isAccomplished && _block)
        _block(state);
}

void Mission::updateWithCardsVector<T>* cards, PokerCardHand* hand, int numPlays) 
 {
    if (_isAccomplished)
        return;
    
    switch (_type) {
        case MISSION_TYPE_CARD_ONE_PAIR:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_10:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 10){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_J:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 11){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_Q:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 12){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_K:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 13){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_ONE_PAIR_ACES:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_ONE_PAIR){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 1){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_TWO_PAIRS:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_TWO_PAIR)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_THREE_OF_A_KIND:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_THREE_OF_A_KIND)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_THREE_OF_A_KIND_ACES:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_THREE_OF_A_KIND){
               Vector<T>* handCards = hand->handCards();
                for (PokerCardConfig *c in handCards){
                    if (c.number == 1){
                        this->didAccomplish();
                        break;
                    }
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_FLUSH:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_FLUSH)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_STRAIGHT:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_STRAIGHT)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_FULL_HOUSE:
        {
            if (hand->hand() == POKER_CARD_HAND_TYPE_FULL_HOUSE)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_JOKER:
        {
            for (PokerCardConfig *c in cards){
                if (c.suit == POKER_CARD_SUIT_TYPE_JOKER){
                    this->didAccomplish();
                    break;
                }
            }
            break;
        }
        case MISSION_TYPE_CARD_JQK:
        {
            bool j,q,k;
            j = q = k = false;
            for (PokerCardConfig *c in cards){
                j |= c.number == 11;
                q |= c.number == 12;
                k |= c.number == 13;
            }
            if (j && q && k)
                this->didAccomplish();
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
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_RED_CARDS:
        {
            int numRedCards = 0;
            for (PokerCardConfig *c in cards)
                if ((c.suit == POKER_CARD_SUIT_TYPE_DIAMOND) || (c.suit == POKER_CARD_SUIT_TYPE_HEART))
                    numRedCards++;
            if (numRedCards >= 3)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_BLACK_CARDS:
        {
            int numBlackCards = 0;
            for (PokerCardConfig *c in cards)
                if ((c.suit == POKER_CARD_SUIT_TYPE_CLUB) || (c.suit == POKER_CARD_SUIT_TYPE_SPADE))
                    numBlackCards++;
            if (numBlackCards >= 3)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_FIGURES:
        {
            bool ok = true;
            for (PokerCardConfig *c in cards)
                ok &= (c.number == 11 || c.number == 12 || c.number == 13);
            if (ok)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_PLAY_IN_A_ROW:
        {
            int targetInt = 5;
            if (numPlays >= targetInt)
                this->didAccomplish();
            break;
        }
        case MISSION_TYPE_CARD_PRIZES_AUMULATION:
        {
            if (hand->hand() != POKER_CARD_HAND_TYPE_NONE){
                int targetInt = 10;
                int accumInt = (int)NSUserDefaults::standardUserDefaults()->integerForKey(this->keyAux());
                accumInt++;
                NSUserDefaults::standardUserDefaults()->setInteger((NSInteger)accumInt, this->keyAux());
                int left = targetInt - accumInt;
                this->baseString = NSString::stringWithFormat(NSLocalizedString("MISSION_TYPE_CARD_PRIZES_AUMULATION",NULL),targetInt);
                this->updateTextWithLeft(left);
                if (left <= 0)
                    this->didAccomplish();
                break;
            }
        }
        case MISSION_TYPE_CARD_ONE_PRIZE:
        {
            if (hand->hand() != POKER_CARD_HAND_TYPE_NONE){
                this->didAccomplish();
                break;
            }
        }
        default:
            break;
    }
}

void Mission::updateBecauseCashInPokerChips() 
{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_CARD_CASH_IN)
        this->didAccomplish();
}

void Mission::updateBecauseDidPlayWithoutPokerChips() 
{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_CARD_PLAY_WITHOUT_CHIPS)
        this->didAccomplish();
}

void Mission::updateBecauseDidShareVideo() 
{
    if (_isAccomplished)
        return;
    if (_type == MISSION_TYPE_SHARE_VIDEO)
        this->didAccomplish();
}

void Mission::didAccomplish() 
{
    this->setAccomplished(true);
    this->string = this->baseString;
    NSNotificationCenter::defaultCenter()->postNotificationName(MissionAccomplishedNotification, this, NULL);, :didAccomplishMission();
}

bool Mission::isAccomplished() 
{
    return _isAccomplished;
}

void Mission::setAccomplished(bool accomplished) 
{
    _isAccomplished = accomplished;
    
    SDCloudUserDefaults::setBool(accomplished, this->keyAccomplished());, :synchronize();
}

bool Mission::isRewarded() 
{
    return _isRewarded;
}

void Mission::setRewarded(bool rewarded) 
{
    _isRewarded = rewarded;
    SDCloudUserDefaults::setBool(rewarded, this->keyRewarded());, :synchronize();
}

int Mission::rewardCoins() 
{
    return _rewardCoins;
}

int Mission::rewardSpecialCoins() 
{
    return _rewardSpecialCoins;
}

bool Mission::needFishes() 
{
    return !this->isAccomplished() && (_type == MISSION_TYPE_FISHES_IN_A_GAME_1 || _type == MISSION_TYPE_FISHES_IN_A_GAME_2 || _type == MISSION_TYPE_FISHES_IN_A_GAME_3);
}

bool Mission::needRings() 
{
    return _type == MISSION_TYPE_RINGS_IN_A_GAME;
}

bool Mission::isTheFirstUpgradeWeaponMission() 
{
    return _type == MISSION_TYPE_UPGRADE_WEAPON;
}

bool Mission::mustUseVehicleToAccomplish(VehicleType vehicleType) 
{
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
            return false;
            break;
    }
}
std::string Mission::text() 
{
    return this->string;
}

void Mission::updateTextWithLeft(int left) 
{
    if (left > 0){
        if (left == 1)
            this->string = StringUtils::format("%@ %",this->baseString,, :stringWithFormat(NSLocalizedString("MISSION_LEFT_SINGLE", NULL),left));
        else
            this->string = StringUtils::format("%@ %",this->baseString,, :stringWithFormat(NSLocalizedString("MISSION_LEFT_PLURAL", NULL),left));
    }
    else
        this->string = this->baseString;
}
std::string Mission::icon() 
{
    if (this->isAccomplished())
        return this->iconAccomplished();
    else
        return this->iconNotAccomplished();
}
std::string Mission::iconAccomplished() 
{
    switch (_icon) {
        case MISSION_ICON_TYPE_1:
            return "missionIcon01Accomplished.png";
            break;
        case MISSION_ICON_TYPE_2:
            return "missionIcon02Accomplished.png";
            break;
        case MISSION_ICON_TYPE_3:
            return "missionIcon03Accomplished.png";
            break;
        case MISSION_ICON_TYPE_4:
            return "missionIcon04Accomplished.png";
            break;
        case MISSION_ICON_TYPE_5:
            return "missionIcon05Accomplished.png";
            break;
        case MISSION_ICON_TYPE_6:
            return "missionIcon06Accomplished.png";
            break;
        case MISSION_ICON_TYPE_7:
            return "missionIcon07Accomplished.png";
            break;
        case MISSION_ICON_TYPE_8:
            return "missionIcon08Accomplished.png";
            break;
        case MISSION_ICON_TYPE_9:
            return "missionIcon09Accomplished.png";
            break;
        case MISSION_ICON_TYPE_10:
            return "missionIcon10Accomplished.png";
            break;
        case MISSION_ICON_TYPE_11:
            return "missionIcon11Accomplished.png";
            break;
        case MISSION_ICON_TYPE_12:
            return "missionIcon12Accomplished.png";
            break;
        case MISSION_ICON_TYPE_13:
            return "missionIcon13Accomplished.png";
            break;
        case MISSION_ICON_TYPE_14:
            return "missionIcon14Accomplished.png";
            break;
        case MISSION_ICON_TYPE_15:
            return "missionIcon15Accomplished.png";
            break;
        case MISSION_ICON_TYPE_16:
            return "missionIcon16Accomplished.png";
            break;
        case MISSION_ICON_TYPE_17:
            return "missionIcon17Accomplished.png";
            break;
        case MISSION_ICON_TYPE_18:
            return "missionIcon18Accomplished.png";
            break;
        case MISSION_ICON_TYPE_19:
            return "missionIcon19Accomplished.png";
            break;
        case MISSION_ICON_TYPE_20:
            return "missionIcon20Accomplished.png";
            break;
        case MISSION_ICON_TYPE_21:
            return "missionIcon21Accomplished.png";
            break;
        case MISSION_ICON_TYPE_22:
            return "missionIcon22Accomplished.png";
            break;
        case MISSION_ICON_TYPE_23:
            return "missionIcon23Accomplished.png";
            break;
        case MISSION_ICON_TYPE_24:
            return "missionIcon24Accomplished.png";
            break;
        case MISSION_ICON_TYPE_25:
            return "missionIcon25Accomplished.png";
            break;
        case MISSION_ICON_TYPE_26:
            return "missionIcon26Accomplished.png";
            break;
        case MISSION_ICON_TYPE_27:
            return "missionIcon27Accomplished.png";
            break;
        default:
            return "";
            break;
    }
}
std::string Mission::iconNotAccomplished() 
{
    switch (_icon) {
        case MISSION_ICON_TYPE_1:
            return "missionIcon01NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_2:
            return "missionIcon02NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_3:
            return "missionIcon03NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_4:
            return "missionIcon04NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_5:
            return "missionIcon05NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_6:
            return "missionIcon06NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_7:
            return "missionIcon07NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_8:
            return "missionIcon08NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_9:
            return "missionIcon09NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_10:
            return "missionIcon10NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_11:
            return "missionIcon11NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_12:
            return "missionIcon12NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_13:
            return "missionIcon13NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_14:
            return "missionIcon14NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_15:
            return "missionIcon15NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_16:
            return "missionIcon16NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_17:
            return "missionIcon17NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_18:
            return "missionIcon18NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_19:
            return "missionIcon19NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_20:
            return "missionIcon20NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_21:
            return "missionIcon21NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_22:
            return "missionIcon22NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_23:
            return "missionIcon23NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_24:
            return "missionIcon24NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_25:
            return "missionIcon25NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_26:
            return "missionIcon26NotAccomplished.png";
            break;
        case MISSION_ICON_TYPE_27:
            return "missionIcon27NotAccomplished.png";
            break;
        default:
            return "";
            break;
    }
}

int Mission::price() 
{
    return _specialCoins;
}

int Mission::ident() 
{
    return _type;
}

int Mission::order() 
{
    return _order;
}

void Mission::reset() 
{
    this->setAccomplished(false);
    this->setRewarded(false);, :removeObjectForKey(this->keyAux());
}

bool Mission::isEqualMission* object) 
{
    return _type == [((Mission*)object)type];
}
std::string Mission::description() 
{
    return StringUtils::format("%d. (%d) %", this->order(),this->ident(),this->baseString);
}

void Mission::printMissions() 
{
   std::string string = "";
   Vector<T>* ranges = MilitarRange::ranges();
    for (MilitarRange *r in ranges){
        string = string->stringByAppendingFormat("%@\n",r);
       Vector<T>* missions = Mission::missionsWithRange([r type]);
        for (Mission *m in missions)
            string = string->stringByAppendingFormat("\t%@\n",m);
    }
    NSLog("%",string);
}

bool Mission::checkMissions() 
{
    auto allMissions = new Vector<T>;
   Vector<T>* ranges = MilitarRange::ranges();
    for (MilitarRange *r in ranges){
       Vector<T>* missions = Mission::missionsWithRange([r type]);
        allMissions->addObjectsFromArray(missions);
    }
    
    bool ok = true;
    int count = (int)allMissions->count();
    for (int i = 0; i < count; i++){
        Mission *m1 = allMissions->objectAtIndex(i);
        for (int j = i+1; j < count; j++){
            Mission *m2 = allMissions->objectAtIndex(j);
            if (m1->isEqual(m2)){
                NSLog("Misiones, \n%@\n%",m1,m2);
                ok = false;
            }
        }
    }
    return ok;
}


