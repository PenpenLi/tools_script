//
//  MissionForm.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "MissionForm.h"
#include "BReader.h"
#include "GameState.h"
#include "SoundManager.h"
#include "ShopCoinsLayer.h"
#include "ShopSpecialCoinsLayer.h"
#include "NumberFormat.h"
#include "GameManager.h"
#include "ParticleManager.h"
#include "Sfx.h"
#include "Analytics.h"
#include "GameManager.h"
#include "CashNode.h"
#include "Achievement.h"





#define TAG_SKIP_MISSION_SPRITE 3
#define TAG_SKIP_MISSION_LABEL 4
#define TAG_SKIP_MISSION_MENU 5
#define TAG_SKIP_MISSION_BUTTON 6
#define TAG_SKIP_MISSION_PRICE_LABEL 7
#define TAG_SKIP_MISSION_PRICE_SPRITE 8
#define TAG_SKIP_MISSION_TITLE 9
#define TAG_SKIP_MISSION_CLOSE_ITEM 10

class MissionSkipTouchLayer : public Layer {public:



-(void)didBuyMission;
-(void)didClose;


class MissionSkipNode : public Node {public: <CashNodeDelegate>{
    Mission *_mission;
    bool _bought;
}
@property (assign) id<MissionSkipNodeDelegate> delegate;


@implementation MissionSkipNode
MissionForm* MissionForm::nodeWithMission(Mission* mission) 
{
    MissionSkipNode *node = (MissionSkipNode*)BReader::nodeGraphFromFile("MissionSkip.ccbi");
    node->loadMission(mission);
    return node;
}

void MissionForm::dealloc() 
{
    this->delegate = NULL;
    _mission->release();
    super->dealloc();
}

void MissionForm::didLoadFromB() 
{
    _bought = false;
    this->menu().touchPriority = kMenuHandlerPriority - 12;
    this->title().string = NSLocalizedString("MISSION_SKIP_TITLE", NULL);
    this->missionLabel()->setWidth(this->missionLabel().contentSize.width);
    ((Sprite*)this->closeItem().selectedImage).blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
    
    MissionSkipTouchLayer *layer = MissionSkipTouchLayer::node();
    this->addChild(layer, -1);
    layer.touchEnabled = true;
    layer.touchMode = kTouchesOneByOne;
    layer.touchSwallow = true;
    layer.touchPriority = this->menu().touchPriority+1;
    
    // Alijo
    MenuItem *button = this->button();
    CashNode *cashNode = CashNode::specialCoinCashNode();
    this->addChild(cashNode);
   cashNode->setPosition(ccp(button->getPosition().x, button->getPosition().y - button->getContentSize().height));
    cashNode.delegate = this;
    cashNode->setTouchPriority((int)layer.touchPriority-1);
}

void MissionForm::loadMission(Mission* mission) 
{
    _mission = mission->retain();
    
    this->missionSprite()->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mission->icon()));
    this->missionLabel().string = mission->text();
    
    LabelBMFont *priceLabel = this->priceLabel();
    Sprite *priceSprite = this->priceSprite();
    
    priceLabel.string = NumberFormat::formatInteger(mission->price());
    
    float totalWidth = priceSprite->getContentSize().width*priceSprite->getScale() + priceLabel->getContentSize().width*priceLabel->getScale();
   priceSprite->setPosition(ccpAdd(this->button().position, ccp(priceSprite->getContentSize().width*priceSprite->getScale()*0.5f - totalWidth*0.5f,0.0f)));
   priceLabel->setPosition(ccp(priceSprite->getPosition().x + priceSprite->getContentSize().width*0.5f*priceSprite->getScale() + priceLabel->getContentSize().width*priceLabel->getScale()*0.5f,priceSprite->getPosition().y));
}

Sprite* MissionForm::missionSprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SKIP_MISSION_SPRITE);
}

LabelBMFont* MissionForm::missionLabel() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_MISSION_LABEL);
}

LabelBMFont* MissionForm::priceLabel() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_MISSION_PRICE_LABEL);
}

Sprite* MissionForm::priceSprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SKIP_MISSION_PRICE_SPRITE);
}

LabelBMFont* MissionForm::title() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_MISSION_TITLE);
}

Menu* MissionForm::menu() 
{
    return (Menu*)this->getChildByTag(TAG_SKIP_MISSION_MENU);
}

MenuItem* MissionForm::button() 
{
    return (MenuItem*)this->menu()->getChildByTag(TAG_SKIP_MISSION_BUTTON);
}

MenuItemSprite* MissionForm::closeItem() 
{
    return (MenuItemSprite*)this->menu()->getChildByTag(TAG_SKIP_MISSION_CLOSE_ITEM);
}

void MissionForm::didTapBuyItem() 
{
    bool enoughMoney = GameManager::sharedManager().specialCoins >= _mission->price();
    
    if (enoughMoney){, :sharedManager()->playEffect(SOUND_BUTTON_BUY);
        _mission->setAccomplished(true);
        _mission->setRewarded(true);, :sharedManager().specialCoins -= _mission->price();, :sharedManager().specialCoinsSpent += _mission->price();
        GameManager->sharedManager()->save();
        _bought = true;
        
        this->missionSprite()->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_mission->icon()));
        this->runMissionParticlesWithPosition(this->missionSprite().position);
        
        this->menu()->setEnabled(false);
        DelayTime *delay = DelayTime::create(2.0f);
        CallFunc *block = CallFunc::create([this]() {
            if (this->delegate)
                [this->delegate didBuyMission];
            this->removeFromParent();
        });
        Sequence *sequence = Sequence::create(delay,block,NULL);
        this->runAction(sequence);
        
        // Analíticas
        // Flurry
        {
           std::string ident = StringUtils::format("Misión %d (%d)", _mission->order(),_mission->ident());
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Mission Skipped");
            event->addParam(AnalyticsParam::paramStringWithString(ident, "Misión"));
            event->addParam(AnalyticsParam::paramSelectedCharacter());
            event->addParam(AnalyticsParam::paramSelectedVehicle());
            event->addParam(AnalyticsParam::paramMilitarRange());
            event->addParam(AnalyticsParam::paramIsFirstSession());, :logFlurryEvent(event);
        }
        
        // Game Analytics
        {
           std::string ident = StringUtils::format("Misión %d (%d)", _mission->order(),_mission->ident());
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Mission");
            event->addParam(AnalyticsParam::paramStringWithString(ident key:"Skipped"));, :logGameAnalyticsDesignEvent(event);, :logGameAnalyticsExpenseAndResumeEventsWithCoins(0 specialCoins:_mission->price(), "Skipped");
        }
        
        // Misiones
        MilitarRange *range = MilitarRange::currentRange();
       Vector<T>* rangeMissions = Mission::missionsWithRange(range->type());
        bool allAccomplished = true;
        
        for (Mission *m in rangeMissions)
            allAccomplished &= [m isAccomplished];
        
        if (allAccomplished){
            // Analíticas
            // Flurry
            {
                AnalyticsEvent *event = AnalyticsEvent::eventWithName("Range completed");
                event->addParam(AnalyticsParam::paramSelectedCharacter());
                event->addParam(AnalyticsParam::paramSelectedVehicle());
                event->addParam(AnalyticsParam::paramMilitarRange());
                event->addParam(AnalyticsParam::paramIsFirstSession());, :logFlurryEvent(event);
            }
        }
        
        // Logros
        Achievement::didSkipMission();
    }
    else{
        ShopSpecialCoinsLayer *layer = ShopSpecialCoinsLayer::layerComesFrom("mission skip", false);
        [this->parent addChild:layer z:100];
        SoundManager::sharedManager()->playEffect(SOUND_BUTTON_OPEN);
    }
}

void MissionForm::didTapCloseItem() 
{
    if (this->delegate){
        if (_bought)
            [this->delegate didBuyMission];
        else
            [this->delegate didClose];
    }
    SoundManager::sharedManager()->playEffect(SOUND_BUTTON_CLOSE);
    this->removeFromParent();
}

void MissionForm::didTapCoinsCashNode(CashNode* node) 
{
    ShopCoinsLayer *layer = ShopCoinsLayer::layerComesFrom("mission skip", false);
    [this->parent addChild:layer z:100];
    SoundManager::sharedManager()->playEffect(SOUND_BUTTON_OPEN);
}

void MissionForm::didTapSpecialCoinsCashNode(CashNode* node) 
{
    ShopSpecialCoinsLayer *layer = ShopSpecialCoinsLayer::layerComesFrom("mission skip", false);
    [this->parent addChild:layer z:100];
    SoundManager::sharedManager()->playEffect(SOUND_BUTTON_OPEN);
}

void MissionForm::runMissionParticlesWithPosition(Point position) 
{
    {
        ParticleSystem *particles = ParticleManager::sharedManager()->particlesWithFile(PARTICLES_BIG_SMOKE);
        this->addChild(particles, 10);
       particles->setPosition(position);
       particles->setAutoRemoveOnFinish(true);
    }
    
    {
        ccBlendFunc blend1 = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        ccBlendFunc blend2 = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        ParticleSystem *particles = ParticleManager::sharedManager()->doubleBlendParticlesWithFile(PARTICLES_BIG_EXPLOSION, blend1, blend2);
        this->addChild(particles, 10);
       particles->setPosition(position);
       particles->setAutoRemoveOnFinish(true);
    }
    
    {
        Sfx *sfx = Sfx::bigHaloExplosion();
        this->addChild(sfx, 10);
       sfx->setPosition(position);
    }
    
    {
        Sfx *sfx = Sfx::ringExplosion();
        this->addChild(sfx, 10);
       sfx->setPosition(position);
    }
    
    {
        ParticleSystem *particles = ParticleManager::sharedManager()->particlesWithFile(PARTICLES_MELT_UPDATE_RANGE);
        this->addChild(particles, 10);
       particles->setPosition(position);
       particles->setAutoRemoveOnFinish(true);
        particles.posVar = ccpMult(particles.posVar, 0.2f);
        particles.life *= 0.5f;
        particles.lifeVar *= 0.5f;
        particles.startSize *= 0.2f;
        particles.startSizeVar *= 0.2f;
        particles.endSize *= 0.2f;
        particles.endSizeVar *= 0.2f;
        particles.speed *= 0.5f;
        particles.speedVar *= 0.5f;
        particles.blendAdditive = true;
    }
}



@implementation MissionSkipTouchLayer
bool MissionForm::ccTouchBegan(UITouch * touch, UIEvent * event) 
 {
    [((MissionSkipNode*)this->parent)didTapCloseItem];
    return true;
}








#define TAG_LABEL_TITLE 1
#define TAG_SPRITE_MISSION_1 2
#define TAG_SPRITE_MISSION_2 3
#define TAG_SPRITE_MISSION_3 4
#define TAG_SPRITE_MISSION_4 5
#define TAG_LABEL_MISSION_1 6
#define TAG_LABEL_MISSION_2 7
#define TAG_LABEL_MISSION_3 8
#define TAG_LABEL_MISSION_4 9
#define TAG_LABEL_LEVEL 10
#define TAG_SPRITE_RANGE 11
#define TAG_MENU 12
#define TAG_MENU_ITEM_SKIP_1 13
#define TAG_MENU_ITEM_SKIP_2 14
#define TAG_MENU_ITEM_SKIP_3 15
#define TAG_MENU_ITEM_SKIP_4 16
#define TAG_SKIP_LABEL_1 17
#define TAG_SKIP_LABEL_2 18
#define TAG_SKIP_LABEL_3 19
#define TAG_SKIP_LABEL_4 20
#define TAG_BACKGROUND 21
#define TAG_LABEL_NO_MORE_MISSIONS 22
#define TAG_SPRITE_LINE_1 23
#define TAG_SPRITE_LINE_2 24
#define TAG_SPRITE_LINE_3 25
#define TAG_LABEL_CONTENT 26

@interface MissionForm () <MissionSkipNodeDelegate>


@implementation MissionForm{
   Vector<T>* _missions;
    float _missionLabelWidth;
    float _maxLabelScale;
    bool _skip;
}
MissionForm* MissionForm::formWithSkipButton(bool skip) 
{
    MissionForm *form = (MissionForm*)BReader::nodeGraphFromFile("Missions.ccbi");
    form->loadWithSkip(skip);
    return form;
}
MissionForm* MissionForm::formWithSkipButton(bool skip, float width) 
 {
    MissionForm *node = (MissionForm*)BReader::nodeGraphFromFile("Missions.ccbi");
    
    float originalWidth = node->getContentSize().width;
   node->setContentSize(Size(width, node->getContentSize().height));
    
    float childScale = width / originalWidth;
    node->setMissionLabelWidth(node->missionLabelWidth()*childScale);
    
    node->background().scaleX *= childScale;
    node->menu().contentSize = Size(node->menu().contentSize.width * childScale, node->menu().contentSize.height);
    
    for (Node *child in node->children())
       child->setPosition(ccp(child->getPosition().x * childScale, child->getPosition().y));
    
    for (Node *child in [node->menu()children])
       child->setPosition(ccp(child->getPosition().x * childScale, child->getPosition().y));
    
    node->loadWithSkip(skip);
    return node;
}

void MissionForm::dealloc() 
{
    _missions->release();
    super->dealloc();
}

LabelBMFont* MissionForm::title() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_TITLE);
}

Sprite* MissionForm::mission1Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_MISSION_1);
}

Sprite* MissionForm::mission2Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_MISSION_2);
}

Sprite* MissionForm::mission3Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_MISSION_3);
}

Sprite* MissionForm::mission4Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_MISSION_4);
}

LabelBMFont* MissionForm::mission1Label() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_MISSION_1);
}

LabelBMFont* MissionForm::mission2Label() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_MISSION_2);
}

LabelBMFont* MissionForm::mission3Label() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_MISSION_3);
}

LabelBMFont* MissionForm::mission4Label() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_MISSION_4);
}

LabelBMFont* MissionForm::levelLabel() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL_LEVEL);
}

Sprite* MissionForm::rangeSprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_RANGE);
}

Sprite* MissionForm::background() 
{
    return (Sprite*)this->getChildByTag(TAG_BACKGROUND);
}

Menu* MissionForm::menu() 
{
    return (Menu*)this->getChildByTag(TAG_MENU);
}

MenuItem* MissionForm::skipItem1() 
{
    return (MenuItem*)this->menu()->getChildByTag(TAG_MENU_ITEM_SKIP_1);
}

MenuItem* MissionForm::skipItem2() 
{
    return (MenuItem*)this->menu()->getChildByTag(TAG_MENU_ITEM_SKIP_2);
}

MenuItem* MissionForm::skipItem3() 
{
    return (MenuItem*)this->menu()->getChildByTag(TAG_MENU_ITEM_SKIP_3);
}

MenuItem* MissionForm::skipItem4() 
{
    return (MenuItem*)this->menu()->getChildByTag(TAG_MENU_ITEM_SKIP_4);
}

LabelBMFont* MissionForm::skipLabel1() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_LABEL_1);
}

LabelBMFont* MissionForm::skipLabel2() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_LABEL_2);
}

LabelBMFont* MissionForm::skipLabel3() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_LABEL_3);
}

LabelBMFont* MissionForm::skipLabel4() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SKIP_LABEL_4);
}

Sprite* MissionForm::line1Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_LINE_1);
}

Sprite* MissionForm::line2Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_LINE_2);
}

Sprite* MissionForm::line3Sprite() 
{
    return (Sprite*)this->getChildByTag(TAG_SPRITE_LINE_3);
}

Node* MissionForm::labelContentNode() 
{
    return this->getChildByTag(TAG_LABEL_CONTENT);
}

float MissionForm::missionLabelWidth() 
{
    return _missionLabelWidth;
}

void MissionForm::setMissionLabelWidth(float width) 
{
    _missionLabelWidth = width;
}

void MissionForm::didLoadFromB() 
{
    this->menu().touchPriority = kMenuHandlerPriority - 11;
    _missionLabelWidth = 140; //this->labelContentNode().contentSize.width/this->mission1Label().scale;
    _maxLabelScale = this->mission1Label().scale;
}

void MissionForm::loadWithSkip(bool skip) 
{
    _skip = skip;
    
    SpriteFrameCache *cache = SpriteFrameCache::sharedSpriteFrameCache();
    
    this->title().string = NSLocalizedString("MISSION_TITLE", NULL);
    
    MilitarRange *range = MilitarRange::currentRange();
    this->levelLabel().string =, :stringWithFormat(NSLocalizedString("MISSION_LEVEL",NULL),range->level());
    this->rangeSprite()->setDisplayFrame(cache->spriteFrameByName(range->spriteNameMini()));
    
    this->skipLabel1().string =
        this->skipLabel2().string =
        this->skipLabel3().string =
        this->skipLabel4().string = NSLocalizedString("MISSION_SKIP_SHORT", NULL);
    CGSize skipLabelSize = this->skipLabel1().contentSize;
    CGSize skipItemSize = this->skipItem1().contentSize;
    float skipLabelScale = fminf(this->skipLabel1().scale, skipItemSize.width*0.9f / skipLabelSize.width);
    this->skipLabel1().scale =
        this->skipLabel2().scale =
        this->skipLabel3().scale =
        this->skipLabel4().scale = skipLabelScale;
    _missions =::(->())->();
    
    this->reloadMissions();
}

void MissionForm::reloadMissions() 
{
    this->removeChildByTag(TAG_LABEL_NO_MORE_MISSIONS);
    SpriteFrameCache *cache = SpriteFrameCache::sharedSpriteFrameCache();
    
    if (_missions && _missions->count() == 4){
        Mission *mission1 = _missions->objectAtIndex(0);
        Mission *mission2 = _missions->objectAtIndex(1);
        Mission *mission3 = _missions->objectAtIndex(2);
        Mission *mission4 = _missions->objectAtIndex(3);
        
        this->mission1Sprite()->setDisplayFrame(cache->spriteFrameByName(mission1->icon()));
        this->mission2Sprite()->setDisplayFrame(cache->spriteFrameByName(mission2->icon()));
        this->mission3Sprite()->setDisplayFrame(cache->spriteFrameByName(mission3->icon()));
        this->mission4Sprite()->setDisplayFrame(cache->spriteFrameByName(mission4->icon()));
        
        LabelBMFont *mission1Label = this->mission1Label();
        LabelBMFont *mission2Label = this->mission2Label();
        LabelBMFont *mission3Label = this->mission3Label();
        LabelBMFont *mission4Label = this->mission4Label();
        mission1Label.string = mission1->text();
        mission2Label.string = mission2->text();
        mission3Label.string = mission3->text();
        mission4Label.string = mission4->text();

        _missionLabelWidth = this->labelContentNode().contentSize.width/mission1Label->getScale();
        if (_skip) {
            _missionLabelWidth -= this->skipItem1().contentSize.width;
        }
        mission1Label->setWidth(_missionLabelWidth);
        mission2Label->setWidth(_missionLabelWidth);
        mission3Label->setWidth(_missionLabelWidth);
        mission4Label->setWidth(_missionLabelWidth);
//        Node *labelContentNode = this->labelContentNode();
//        
//       mission1Label->setScale(fminf(_maxLabelScale, labelContentNode->getContentSize().height/mission1Label->getContentSize().height));
//       mission2Label->setScale(fminf(_maxLabelScale, labelContentNode->getContentSize().height/mission2Label->getContentSize().height));
//       mission3Label->setScale(fminf(_maxLabelScale, labelContentNode->getContentSize().height/mission3Label->getContentSize().height));
//       mission4Label->setScale(fminf(_maxLabelScale, labelContentNode->getContentSize().height/mission4Label->getContentSize().height));
//        
//        mission1Label->setWidth(_missionLabelWidth*(_maxLabelScale/mission1Label->getScale()));
//        mission2Label->setWidth(_missionLabelWidth*(_maxLabelScale/mission2Label->getScale()));
//        mission3Label->setWidth(_missionLabelWidth*(_maxLabelScale/mission3Label->getScale()));
//        mission4Label->setWidth(_missionLabelWidth*(_maxLabelScale/mission4Label->getScale()));
        
        if (_skip){
            this->skipItem1().visible = this->skipItem1().isEnabled = this->skipLabel1().visible = !mission1->isAccomplished();
            this->skipItem2().visible = this->skipItem2().isEnabled = this->skipLabel2().visible = !mission2->isAccomplished();
            this->skipItem3().visible = this->skipItem3().isEnabled = this->skipLabel3().visible = !mission3->isAccomplished();
            this->skipItem4().visible = this->skipItem4().isEnabled = this->skipLabel4().visible = !mission4->isAccomplished();
        }
        else{
            this->menu().visible = false;
            this->skipLabel1().visible = false;
            this->skipLabel2().visible = false;
            this->skipLabel3().visible = false;
            this->skipLabel4().visible = false;
        }
    }
    else{
        this->menu().visible = false;
        this->skipLabel1().visible = false;
        this->skipLabel2().visible = false;
        this->skipLabel3().visible = false;
        this->skipLabel4().visible = false;
        this->mission1Sprite().visible = false;
        this->mission2Sprite().visible = false;
        this->mission3Sprite().visible = false;
        this->mission4Sprite().visible = false;
        this->mission1Label().visible = false;
        this->mission2Label().visible = false;
        this->mission3Label().visible = false;
        this->mission4Label().visible = false;
        this->line1Sprite().visible = false;
        this->line2Sprite().visible = false;
        this->line3Sprite().visible = false;
        
        LabelBMFont *label = LabelBMFont::labelWithString(NSLocalizedString("MISSION_NO_MORE", NULL), "fontSmall.fnt");
        this->addChild(label, 1, TAG_LABEL_NO_MORE_MISSIONS);
       label->setPosition(ccp(this->getContentSize().width*0.5f,this->getContentSize().height*0.5f));
    }
}

void MissionForm::didTapSkipItem(MenuItem* item) 
{
    Mission *mission = NULL;
    switch (item.tag) {
        case TAG_MENU_ITEM_SKIP_1:
            mission = _missions->objectAtIndex(0);
            break;
        case TAG_MENU_ITEM_SKIP_2:
            mission = _missions->objectAtIndex(1);
            break;
        case TAG_MENU_ITEM_SKIP_3:
            mission = _missions->objectAtIndex(2);
            break;
        case TAG_MENU_ITEM_SKIP_4:
            mission = _missions->objectAtIndex(3);
            break;
        default:
            break;
    }
    if (mission){, :sharedManager()->playEffect(SOUND_BUTTON_OPEN);
        MissionSkipNode *node = MissionSkipNode::nodeWithMission(mission);
       node->setPosition(this->getPosition());
        node.delegate = this;
        [this->parent addChild:node z:this->zOrder];
        this->visible = false;
    }
}

void MissionForm::showLevel(bool show) 
{
    this->rangeSprite().visible = show;
    this->levelLabel().visible = show;
}

void MissionForm::showTitle(bool show) 
{
    this->title().visible = show;
}

//
// MissionSkipNodeDelegate
//

void MissionForm::didBuyMission() 
{
    this->reloadMissions();
    this->visible = true;
}

void MissionForm::didClose() 
{
    this->visible = true;
}

