//
//  CollideInterface.h
//  HiFly
//
//  Created by chris li on 14-5-14.
//
//

#ifndef HiFly_CollideInterface_h
#define HiFly_CollideInterface_h

#include "cocos2d.h"

USING_NS_CC;

class CollideInterface
{
public:
    virtual Vector<Node*>* getCollideNode() = 0;
    
    virtual Rect getCollideRect(Node *myNode, int targetNodeType) = 0;
    
    virtual void handleCollide(Node *myNode, int targetNodeType) = 0;
    
    virtual int getCollideNodeType(Node *myNode) = 0;
};

#endif

