#include "MyButton.h"

void MyButton::setOnPressEnable(bool bRet)
{
	Button::setTouchEnabled(bRet);
	if (bRet)
		onPressStateChangedToNormal();
	else
		onPressStateChangedToDisabled();
}

//void MyButton::setTouchEnabled(bool enable)
//{
//	Button::setTouchEnabled(enable);
//	setOnPressEnable(enable);
//}
//bool MyButton::init()
//{
//    if (Layer::init())
//    {
//	//member variable init
//
//	//Ui res
//	importUI();
//
//        return true;
//    }
//    
//    return false;
//}
//
//void MyButton::onEnter()
//{
//	Layer::onEnter();
//
//}
//
//void MyButton::onExit()
//{
//	Layer::onExit();
//
//}
//
//bool MyButton::importUI()
//{
//	bool bRet = false;
//	do{
//
//		bRet = true;
//	}while(0);
//
//	return bRet;
//}