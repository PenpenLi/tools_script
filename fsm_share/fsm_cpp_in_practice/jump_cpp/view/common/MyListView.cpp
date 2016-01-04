#include "MyListView.h"

bool MyListView::init()
{
	if (ListView::init())
    {
	//member variable init

	//Ui res
	//importUI();

        return true;
    }
    
    return false;
}

void MyListView::setDirectionParent(SCROLLVIEW_DIR dir)
{
	_direction = dir;
}

void MyListView::updateInnerContainerSizeV()
{
	size_t length = _items.size();
	float totalHeight = (length - 1) * _itemsMargin;
	for (auto& item : _items)
	{
		totalHeight += item->getSize().height;
	}
	float finalWidth = _size.width;
	float finalHeight = totalHeight;
	setInnerContainerSize(Size(finalWidth, finalHeight));
}
//
//void MyListView::onEnter()
//{
//	Layer::onEnter();
//
//}
//
//void MyListView::onExit()
//{
//	Layer::onExit();
//
//}
//
//bool MyListView::importUI()
//{
//	bool bRet = false;
//	do{
//
//		bRet = true;
//	}while(0);
//
//	return bRet;
//}