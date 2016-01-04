#include "MyScrollView.h"
#include "math.h"
#include <string>
#include "home/UiConfig.h"

#define NULL_OFFSET -88888.0f

bool MyScrollView::init()
{
	if (ScrollView::init())
    {
		//member variable init
		//_swallowsTouches = false;
		//_touchMode=Touch::DispatchMode::ALL_AT_ONCE;
		//_touchListener = nullptr;

        return true;
    }
    
    return false;
}
//
//void MyScrollView::onEnter()
//{
//	Layer::onEnter();
//
//}
//
//void MyScrollView::onExit()
//{
//	Layer::onExit();
//
//}
//
//bool MyScrollView::importUI()
//{
//	bool bRet = false;
//	do{
//
//		bRet = true;
//	}while(0);
//
//	return bRet;
//}

void MyScrollView::scrollToPoint()
{
	startAutoScrollChildrenWithDestination(ccp(-213.0f,0),1.0f,true);
}

void MyScrollView::logSizePositionInfo()
{
	//log("~~~size : %f %f \n  ~~~~Container : %f %f ",_size.width,_size.height,_innerContainer->getSize().width,_innerContainer->getSize().height);
	
	Vector<Node*> t_children = this->getChildren();

	for( auto& element : t_children)
	{
		ImageView* t_child = static_cast<ImageView*>(element);
		log("~~~role 3 Frame : x = %f  y = %f ",t_child->getWorldPosition().x,t_child->getWorldPosition().y);
	}
}

void MyScrollView::updateOffset(float time)
{
	Point t_innerContainerPos = this->getInnerContainer()->getPosition();
	//判断是否在边界回弹范围  是则不处理  否则进行回弹操作
	if(t_innerContainerPos.x>-(gap_role*2+0.5f)&&t_innerContainerPos.x<(0+0.5f))		//0.5f is allowable D-value
	{
	//
	//设置为0时才能有效进行回弹操作 -原因未知
	_slidTime = 0.0f;

	float t_updateOffset = NULL_OFFSET;
	float t_midPointX = this->getSize().width/2;
	float t_roleNum = 0.0f;
	
	Vector<Node*> t_children = this->getChildren();

	for( auto& element : t_children)
	{
		ImageView* t_child = static_cast<ImageView*>(element);
		float t_offset = t_midPointX - t_child->getWorldPosition().x;

		std::string t_frameName = t_child->getName();

		if(NULL_OFFSET == t_updateOffset)
		{
			t_updateOffset = t_offset;
			if(STR_roleName001==t_frameName)
			{
				t_roleNum = 0.0f;
			}
			else if(STR_roleName002==t_frameName)
			{
				t_roleNum = 1.0f;
			}
			else if(STR_roleName003==t_frameName)
			{
				t_roleNum = 2.0f;
			}
		}
		if(abs(t_updateOffset)>abs(t_offset))
		{
			t_updateOffset = t_offset;
			if(STR_roleName001==t_frameName)
			{
				t_roleNum = 0.0f;
			}
			else if(STR_roleName002==t_frameName)
			{
				t_roleNum = 1.0f;
			}
			else if(STR_roleName003==t_frameName)
			{
				t_roleNum = 2.0f;
			}
		}
	}

	startAutoScrollChildrenWithDestination(ccp(-t_roleNum*gap_role,0),time,true);
	}
}

int MyScrollView::getFrontRoleId()
{
	float t_updateOffset = NULL_OFFSET;
	float t_midPointX = this->getSize().width/2;
	int t_roleId = 0;
	
	Vector<Node*> t_children = this->getChildren();

	for( auto& element : t_children)
	{
		ImageView* t_child = static_cast<ImageView*>(element);
		float t_offset = t_midPointX - t_child->getWorldPosition().x;

		std::string t_frameName = t_child->getName();

		if(NULL_OFFSET == t_updateOffset)
		{
			t_updateOffset = t_offset;
			if(STR_roleName001==t_frameName)
			{
				t_roleId = 0;
			}
			else if(STR_roleName002==t_frameName)
			{
				t_roleId = 1;
			}
			else if(STR_roleName003==t_frameName)
			{
				t_roleId = 2;
			}
		}
		if(abs(t_updateOffset)>abs(t_offset))
		{
			t_updateOffset = t_offset;
			if(STR_roleName001==t_frameName)
			{
				t_roleId = 0;
			}
			else if(STR_roleName002==t_frameName)
			{
				t_roleId = 1;
			}
			else if(STR_roleName003==t_frameName)
			{
				t_roleId = 2;
			}
		}
	}

	return t_roleId;
}
void MyScrollView::refreshRolePostion(int roleId)
{
	startAutoScrollChildrenWithDestination(ccp(-static_cast<float>(roleId)*gap_role,0),0.1f,true);
}