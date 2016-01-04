#include "TextFloatLabelLayer.h"

USING_NS_CC;

TextFloatLabelLayer* g_TextFloatLabelLayer = nullptr;

TextFloatLabelLayer* TextFloatLabelLayer::getInstance()
{
    if (nullptr == g_TextFloatLabelLayer)
    {
        g_TextFloatLabelLayer = new TextFloatLabelLayer();
        
        if (!g_TextFloatLabelLayer->init())
        {
            g_TextFloatLabelLayer = nullptr;
        }
    }
    
    return g_TextFloatLabelLayer;
}

bool TextFloatLabelLayer::init()
{
	if (Layer::init())
    {
		
		return true;
	}
    
    return false;
}

void TextFloatLabelLayer::show(std::string msg)
{
    if (isShow()) {return;}
    
	auto currentScene = Director::getInstance()->getRunningScene();
    
	auto screenSize = Director::getInstance()->getWinSize();

	auto textLabel = Text::create();
	
	textLabel->setFontSize(40);

	textLabel->setText(msg);
	textLabel->setPosition(Point(screenSize.width/2,screenSize.height/2));

	this->addChild(textLabel);

	currentScene->addChild(this,99);

	auto seq = Sequence::create(DelayTime::create(0.5f),FadeOut::create(1.0f),CallFunc::create(this,callfunc_selector(TextFloatLabelLayer::hide)),nullptr);
	textLabel->runAction(seq);
}
    
void TextFloatLabelLayer::hide()
{
	this->removeAllChildren();
	this->removeFromParent();
}
    
bool TextFloatLabelLayer::isShow()
{
	if (this->getParent())
	{
		return true;
	}

	return false;
}