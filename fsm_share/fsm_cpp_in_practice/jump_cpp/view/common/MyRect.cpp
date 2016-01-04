#include "MyRect.h"

Rect MyRect::createWithPointAndSize(Point origin,Size size)
{
	Rect t_myRect;
	t_myRect.origin = origin;
	t_myRect.size = size;
	return t_myRect;
}

//bool MyRect::init()
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
//void MyRect::onEnter()
//{
//	Layer::onEnter();
//
//}
//
//void MyRect::onExit()
//{
//	Layer::onExit();
//
//}
//
//bool MyRect::importUI()
//{
//	bool bRet = false;
//	do{
//
//		bRet = true;
//	}while(0);
//
//	return bRet;
//}