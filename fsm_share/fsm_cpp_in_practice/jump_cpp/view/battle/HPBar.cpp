//
//  HPBar.cpp
//  test
//
//  Created by Artuira on 14-2-10.
//
//

#include "HPBar.h"
#include "util/GameSound.h"

HPBar::HPBar()
{
    m_iLastScale = 100;
    m_iCurrentScale = 0;
}

HPBar::~HPBar()
{
   
}

//init
bool HPBar::init()
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName("paly_slip.png");
	Sprite* bg = Sprite::createWithSpriteFrame(frame);
    bg->setAnchorPoint(Point::ZERO);
    bg->setPosition(Point(0,-2));
    
	frame = SpriteFrameCache::getInstance()->spriteFrameByName("paly_slip_2.png");
	Sprite* bar = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->spriteFrameByName("paly_slip_3.png");
	m_ccsOctopusSprite = Sprite::createWithSpriteFrame(frame);

    //bar->setVisible(false);
    bar->setAnchorPoint(Point::ZERO);
    bar->setPosition(Point(0,0));
    m_ccsBar = bar;
    
    this->addChild(bg, 1);
    this->addChild(bar, 3);
	this->addChild(m_ccsOctopusSprite,4);

    Size size = bar->getContentSize();
    m_fGrid = size.width / 100;
    m_fWidth = size.width;
    m_fHeight = size.height;
    
	m_ccsOctopusSprite->setPosition(ccp(m_fWidth,m_fHeight - 13));

	setHP(100);

    return true;
}

//static create methods
HPBar* HPBar::createHPBar()
{
    HPBar* pRet = new HPBar();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

//update
void HPBar::setHP(int scale)
{
    
    int p = 100 - scale;
    log("p:%d",p);
    m_ccsBar->setTextureRect(Rect(0, 0, m_fWidth - p*m_fGrid, m_fHeight));
    m_ccsOctopusSprite->setPositionX((m_fWidth + 10) - p*m_fGrid);
}

void HPBar::setHPre()
{ 
}